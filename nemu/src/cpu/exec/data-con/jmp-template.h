#include "cpu/exec/template-start.h"

#define instr jmp

static void do_execute() {

    cpu.eip+=(DATA_TYPE_S)op_src->val;
    if(DATA_BYTE==2)
		cpu.eip&=0xffff;
    print_asm_template1();
}
make_instr_helper(i);

#if DATA_BYTE ==4
make_helper(ljmp){
  uint32_t op1 = instr_fetch(eip+1,4);
  uint16_t op2 = instr_fetch(eip+5,2);

  cpu.CS.val = op2-7;
  printf("%x\n",cpu.CS.index);
  cpu.eip=(uint32_t)op1;
  print_asm("ljmp %x:%xH",op2,op1);
  return 0;
}
#endif

#include "cpu/exec/template-end.h"
