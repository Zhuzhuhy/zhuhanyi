#include "cpu/exec/template-start.h"

#define instr add
static void do_execute(){
  DATA_TYPE val;
  val = op_dest->val+op_src->val;
/*
  if(val < op_dest->val) 
	  cpu.EFLAGS.CF = 1;
  else cpu.EFLAGS.CF = 0;

  if(op_dest->val == op_src->val)
	  cpu.EFLAGS.ZF = 1;
  else cpu.EFLAGS.ZF = 0;

  if(op_dest->val*op_src->val <0 && op_src->val*val>=0)
	  cpu.EFLAGS.OF = 1;
  else cpu.EFLAGS.OF=0;

  if(val<0)
	  cpu.EFLAGS.SF=1;
  else cpu.EFLAGS.SF = 0;

  int i =1,j=0;
  DATA_TYPE num = val;
  while(i<=8){
  if(MSB(num) == 1) j++;
  num = num << 1;
  i++;
  }  
  if(j%2==0) cpu.EFLAGS.PF = 1;
  else cpu.EFLAGS.PF = 0;
 */
// int len=(DATA_BYTE<<3)-1;
// int s1,s2;
 if(((op_src->val>>31)&(op_dest->val>>31))==1)
  cpu.EFLAGS.CF=1;
 else if((((op_src->val>>31)^(op_dest->val>>31))==1)&&((val>>31)==0))
	 cpu.EFLAGS.CF=1;
 else cpu.EFLAGS.CF=0;
 cpu.EFLAGS.SF=(val>>31)&1;
// s1=op_dest->val>>len;
// s2=op_src->val>>len;
 cpu.EFLAGS.OF=(!((op_dest->val>>31)^(op_src->val>>31)))&&((op_dest->val>>31)^(val>>31))&1;
 cpu.EFLAGS.ZF=(val==0);
 uint32_t pf=(val&255);
 pf^=pf>>4;
 pf^=pf>>2;
 pf^=pf>>1;
 cpu.EFLAGS.PF=(pf&1);


  OPERAND_W(op_dest, val);
  print_asm_template2();
}
//#if DATA_TYPE==2||DATA_TYPE==4
make_instr_helper(si2rm)
//#endif
make_instr_helper(r2rm)
#include "cpu/exec/template-end.h"
