#include "cpu/exec/template-start.h"

#define instr cmp
static void do_execute(){
  uint32_t result;
  result = op_dest->val - op_src->val;
  cpu.EFLAGS.CF=op_dest->val<op_src->val;
  cpu.EFLAGS.SF=(result>>31)&1;

  cpu.EFLAGS.OF=((op_dest->val>>31)^(op_src->val>>31))&((op_dest->val>>31)^(result>>31))&1;
  uint32_t pf=(result&255);
  pf=(pf>>4)&pf;
  pf=(pf>>2)&pf;
  pf=(pf>>1)&pf;

  cpu.EFLAGS.PF=(pf&1);
  cpu.EFLAGS.ZF=(result==0);
  /*

  if(op_dest->val < op_src->val) 
	  cpu.EFLAGS.CF = 1;
  else cpu.EFLAGS.CF = 0;
  
  if(op_dest->val == op_src->val)
	  cpu.EFLAGS.ZF = 1;
  else cpu.EFLAGS.ZF = 0;

 // if(op_dest->val*op_src->val <0 && op_src->val*val>=0)
//	  cpu.EFLAGS.OF = 1;
  //else cpu.EFLAGS.OF=0;

  cpu.EFLAGS.OF=((op_dest->val>>31)^(op_src->val>>31))&&(!((val>>31)^(op_src->val>>31)));
//  cpu.EFLAGS.PF=(count%2==0);
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

 // OPERAND_W(op_dest, val);
  print_asm_template2();
}
#if DATA_BYTE==2||DATA_BYTE==4
make_instr_helper(si2rm)
#endif
make_instr_helper(i2rm)
make_instr_helper(r2rm)
make_instr_helper(rm2r)

#include "cpu/exec/template-end.h"
