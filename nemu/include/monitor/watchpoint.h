#ifndef __WATCHPOINT_H__
#define __WATCHPOINT_H__

#include "common.h"

typedef struct watchpoint {
	int NO;
	struct watchpoint *next;
    int old_value;
    int new_value;
    char e[30];//expression
	/* TODO: Add more members if necessary */


} WP;

#endif
