#include "cpu/exec/template-start.h"

#define instr je

make_helper(concat(je_i_,SUFFIX)) {
	swaddr_t addr = instr_fetch(cpu.eip+1,1);
    if(cpu.EFLAGS.ZF == 1) 
		cpu.eip = cpu.eip + addr;

     return 2;
}



#include "cpu/exec/template-end.h"
