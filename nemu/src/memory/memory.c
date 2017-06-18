#include "common.h"
#include "memory/cache.h"
#include "cpu/reg.h"

uint32_t dram_read(hwaddr_t, size_t);
void dram_write(hwaddr_t, size_t, uint32_t);
lnaddr_t seg_translate(swaddr_t,size_t,SELECTOR);
/* Memory accessing interfaces */

uint32_t hwaddr_read(hwaddr_t addr, size_t len) {
//	return dram_read(addr, len) & (~0u >> ((4 - len) << 3));
  uint32_t result =0x0,data;
  int i=0;
  size_t l=len;
  while(len){
    CacheReadByte(addr+i,(uint8_t *)(&data));
	result |=(data<<(i*8)) & (0xff<<(8*i));
	i++;
	len--;
  
  }
	return result & (~0u >> ((4 - l) << 3));
}

void hwaddr_write(hwaddr_t addr, size_t len, uint32_t data) {
//	dram_write(addr, len, data);
  int i =0;
  while(len){
    CacheWriteByte(addr+i,(uint8_t)(data>>(8*i)));
	i++;
	len--;
  
  }
}

uint32_t lnaddr_read(lnaddr_t addr, size_t len) {
	return hwaddr_read(addr, len);
}

void lnaddr_write(lnaddr_t addr, size_t len, uint32_t data) {
	hwaddr_write(addr, len, data);
}

uint32_t swaddr_read(swaddr_t addr, size_t len) {
#ifdef DEBUG
	assert(len == 1 || len == 2 || len == 4);
#endif
	lnaddr_t lnaddr = seg_translate(addr,len,current_sreg);
	return lnaddr_read(lnaddr, len);
}

/*void swaddr_write(swaddr_t addr, size_t len, uint32_t data) {
#ifdef DEBUG
	assert(len == 1 || len == 2 || len == 4);
#endif
	lnaddr_t lnaddr=seg_translate(addr, len, current_sreg);
	return lnaddr_read(lnaddr,len);
}
*/
void swaddr_write(swaddr_t addr, size_t len,uint32_t data){

#ifdef DEBUG
	assert(len == 1 || len == 2 || len == 4);
#endif
	lnaddr_t lnaddr = seg_translate(addr, len, current_sreg);
	lnaddr_write(lnaddr,len,data);
}
