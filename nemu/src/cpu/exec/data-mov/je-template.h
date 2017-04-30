#include "cpu/exec/template-start.h"

#define instr je

//make_helper(concat(je_i_,SUFFIX)) {

static void do_execute(){
   
    if(cpu.EFLAGS.ZF == 1) 
		cpu.eip = cpu.eip + op_src->val;
    print_asm("je %x",cpu.eip+1+DATA_BYTE);
    
}
make_instr_helper(i);


#include "cpu/exec/template-end.h"
