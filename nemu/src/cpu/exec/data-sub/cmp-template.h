#include "cpu/exec/template-start.h"

#define instr cmp
static void do_execute(){
  DATA_TYPE val;
  val = op_dest->val-op_src->val;
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
 int len=(DATA_BYTE<<3)-1;
 int s1,s2;
 cpu.EFLAGS.CF=op_dest->val<op_src->val;
 cpu.EFLAGS.SF=val>>len;
 s1=op_dest->val>>len;
 s2=op_src->val>>len;
 cpu.EFLAGS.OF=(s1!=s2&&s2==cpu.EFLAGS.SF);
 cpu.EFLAGS.ZF=!val;
 val^=val>>4;
 val^=val>>2;
 val^=val>>1;
 cpu.EFLAGS.PF=!(val&1);


 // OPERAND_W(op_dest, val);
  print_asm_template2();
}
//#if DATA_TYPE==2||DATA_TYPE==4
make_instr_helper(si2rm)
//#endif

//make_helper(cmp_si2rm_v);
make_instr_helper(r2rm);
make_instr_helper(rm2r);
#include "cpu/exec/template-end.h"
