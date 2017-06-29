#include "common.h"
#include "memory/cache.h"
#include "cpu/reg.h"

uint32_t dram_read(hwaddr_t, size_t);
void dram_write(hwaddr_t, size_t, uint32_t);
hwaddr_t page_translate(lnaddr_t addr);
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
//	return hwaddr_read(addr, len);
  assert(len==1||len==2||len==4);
  if(cpu.cr0.paging==0) return hwaddr_read(addr,len);
  if((addr&0xfff)+len>0x1000){
	  uint32_t off = addr & 0xfff;
	  hwaddr_t hwaddr1 = page_translate(addr);	
	  hwaddr_t hwaddr2 = page_translate(addr+0x1000-off);
	  return hwaddr_read(hwaddr1,0x1000-off)+(hwaddr_read(hwaddr2,len-0x1000+off)<<((0x1000-off)*8));
 }
   else{
     hwaddr_t hwaddr = page_translate(addr);
	 return hwaddr_read(hwaddr,len);
  
  }
}
void lnaddr_write(lnaddr_t addr, size_t len, uint32_t data) {
//	hwaddr_write(addr, len, data);
  assert(len==1||len==2||len==4);
 // if(cpu.cr0.paging==0) return hwaddr_read(addr,len);
    if((addr&0xfff)+len>0x1000){
	  uint32_t off = addr & 0xfff;
	  hwaddr_t hwaddr1 = page_translate(addr);	
	  hwaddr_t hwaddr2 = page_translate(addr+0x1000-off);
	  hwaddr_write(hwaddr1,0x1000-off,data);
	  hwaddr_write(hwaddr2,len-0x1000+off,data>>((0x1000-off)*8));
}
   else{
     hwaddr_t hwaddr = page_translate(addr);
	 hwaddr_write(hwaddr,len,data);
  
  }
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
