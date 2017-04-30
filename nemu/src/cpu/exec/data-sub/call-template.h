#include "cpu/exec/template-start.h"

#define instr call

static void do_execute(){
  if(op_src->type==OP_TYPE_REG||op_src->val==OP_TYPE_MEM)
  {
	  uint32_t eip=cpu.eip;
	  REG(R_ESP)-=DATA_BYTE;
	  cpu.eip=op_src->val-2;
	  if(DATA_BYTE==2)
		  cpu.eip=cpu.eip&0xffff;
	  int l=concat(decode_rm_,SUFFIX)(eip+1);
	  MEM_W(REG(R_ESP),eip+l+1);
	  }
  if(op_src->type==OP_TYPE_IMM)
  { 
   REG(R_ESP)-=DATA_BYTE;
#if DATA_BYTE==2
   swaddr_write(REG(R_ESP),DATA_BYTE,(cpu.eip+DATA_BYTE+1)&0xffff);
   cpu.eip=cpu.eip+op_src->val;
   cpu.eip=cpu.eip&0xffff;
#endif

#if DATA_BYTE==4
   swaddr_write(cpu.esp,DATA_BYTE,cpu.eip+DATA_BYTE+1);
   cpu.eip=cpu.eip+op_src->val;
#endif
  }
  print_asm_template1();
}

make_instr_helper(i);

#include "cpu/exec/template-end.h"
