#ifndef __LGDT_H__
#define __LGDT_H__

make_helper(lgdt_m_v);

#endif
/*#include "cpu/exec.c/helper.h"
#include "cpu/decode/modrm.h"
#include "nemu.h"

make_helper(lgdt){
 ModR_M m;
 m.val=instr_fetch(eip+1);
 int len=load_addr(eip+1,&m,op_src);
 cpu.GDTR.limit = swaddr_read(op_src->addr,2);
 cpu.GDTR.base = swaddr_read(op_src->addr+2,4);
 return len+1;
}
*/
