#ifndef __WATCHPOINT_H__
#define __WATCHPOINT_H__

#include "common.h"

typedef struct watchpoint {
	int NO;
	struct watchpoint *next;
    int old_value, new_value;
	int breakpoint;//1 is breaking
    char e[30];//expression
	/* TODO: Add more members if necessary */


} WP;

#endif
