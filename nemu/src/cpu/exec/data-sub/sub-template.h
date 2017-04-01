#include "cpu/exec/template-start.h"

#define instr sub
static void do_execute(){
  op_dest->val = op_dest->val - op_src->val;
  OPERAND_W(op_dest, op_dest->val);
  print_asm_template2();
}
make_instr_helper(i2rm)

#include "cpu/exec/template-end.h"
