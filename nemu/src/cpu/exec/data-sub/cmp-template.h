#include "cpu/exec/template-start.h"

#define instr cmp
static void do_execute(){
  DATA_TYPE val;
  val = op_dest->val-op_src->val;

  if(op_dest->val < op_src->val) 
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
  
}
make_instr_helper(si2rm)

#include "cpu/exec/template-end.h"
