#include "cpu/exec/template-start.h"

#define instr ret

make_helper(concat(ret_,SUFFIX)) {
	swaddr_t addr = swaddr_read(cpu.esp,4);
    cpu.esp+=4;
    cpu.eip=addr-1;
    print_asm_template1();
     return 1;
}



#include "cpu/exec/template-end.h"
