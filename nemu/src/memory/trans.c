#include "common.h"
#include "stdlib.h"
#include "time.h"
#include "../../lib-common/x86-inc/mmu.h"
#define ADDRESS_BITS 32
 
#define OFFSET_WIDTH 12
#define LINE_WIDTH 6
#define TAG_WIDTH (ADDRESS_BITS - OFFSET_WIDTH)

#define NR_LINE (1<<LINE_WIDTH)

typedef union{
	struct{
		uint32_t offset : OFFSET_WIDTH;
		uint32_t tag : TAG_WIDTH;
	};
	uint32_t val;
}trans_addr;

typedef struct{
	PTE pte;
	uint32_t valid:1;
	uint32_t tag: TAG_WIDTH;
}trans_block;

trans_block trans[NR_LINE];

void init_trans(){
	int i =0;
	for(i=0;i<NR_LINE;++i)
		trans[i].valid=0;
}
PTE page_read(lnaddr_t);
hwaddr_t trans_read(lnaddr_t addr){
   trans_addr taddr;
   taddr.val=addr;
   uint32_t i;
   for(i=0;i<NR_LINE;++i){
   if(trans[i].valid == 0) break;
   if(trans[i].tag==taddr.tag)
	   return (trans[i].pte.page_frame<<12)+taddr.offset;
   }
  if(i==NR_LINE){
  srand(time(0));
  i = rand()%NR_LINE;
   }
  trans[i].valid=true;
  trans[i].tag=taddr.tag;
  trans[i].pte=page_read(addr);
  return (trans[i].pte.page_frame<<12)+taddr.offset;
}
	
