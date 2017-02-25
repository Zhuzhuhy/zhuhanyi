#include "monitor/monitor.h"
#include "monitor/expr.h"
#include "monitor/watchpoint.h"
#include "nemu.h"

#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
void cpu_exec(uint32_t);

/* We use the ``readline'' library to provide more flexibility to read from stdin. */
char* rl_gets() {
	static char *line_read = NULL;

	if (line_read) {
		free(line_read);
		line_read = NULL;
	}

	line_read = readline("(nemu) ");

	if (line_read && *line_read) {
		add_history(line_read);
	}

	return line_read;
}

static int cmd_c(char *args) {
	cpu_exec(-1);
	return 0;
}

static int cmd_q(char *args) {
	return -1;
}
static int cmd_si(char *args);
static int cmd_info(char *args);
static int cmd_x(char *args);
static int cmd_help(char *args);

static struct {
	char *name;
	char *description;
	int (*handler) (char *);
} cmd_table [] = {
	{ "help", "Display informations about all supported commands", cmd_help },
	{ "c", "Continue the execution of the program", cmd_c },
	{ "q", "Exit NEMU", cmd_q },
	{ "si","Step Mode Execution",cmd_si },
	{ "info","Print",cmd_info },
	{ "x","Memory scan",cmd_x },
	/* TODO: Add more commands */

};

#define NR_CMD (sizeof(cmd_table) / sizeof(cmd_table[0]))

static int cmd_si(char *args) {

    char *str;
    int num;	
    str = strtok(args," ");
    num = atoi(str);
	if(num == -1) cpu_exec(-1);
	else if(num < 0 ) printf("Error\n");
	else  cpu_exec(num);
	return 0;
}	
static int cmd_info(char *args) {
	char *ch;
	int i = 0;
	ch = strtok(args," ");
	if(strcmp(ch,"r") == 0){
 	while(i < 8){   
		printf("%s :%08x %d \n",regsl[i],cpu.gpr[i]._32,cpu.gpr[i]._32);
		i++;
	}
	printf("eip :%08x %d \n",cpu.eip,cpu.eip);
	}
	else printf("Error\n");
	return 0;
}


static int cmd_x(char *args){
     char *locate_start,*locate_len;
	 int  num, num_s,count = 0,locate;
	 locate_len = strtok(args," ");
	 locate_start = strtok(NULL," ");
     num = atoi(locate_len);
	 sscanf(locate_start,"%x",&num_s);
	 printf("start addr:%x  len:%d\n",num_s,num);
 	 while(count < num){
     locate = swaddr_read(num_s,2);
	 printf("%02x  ",locate);
	 count++;
	 num_s++;
	 if(count == 5) printf("\n");
	 }
	 printf("\n");
	 return 0;
}

static int cmd_help(char *args) {
	/* extract the first argument */
	char *arg = strtok(NULL, " ");
	int i;

 	if(arg == NULL) {
		/* no argument given */
  		for(i = 0; i < NR_CMD; i ++) {
			printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
 		}
  	}
	else {
 		for(i = 0; i < NR_CMD; i ++) {
 			if(strcmp(arg, cmd_table[i].name) == 0) {
				printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
				return 0;
			}
   		}
		printf("Unknown command '%s'\n", arg);
   	}
	return 0;
}

void ui_mainloop() {
	while(1) {
		char *str = rl_gets();
		char *str_end = str + strlen(str);

		/* extract the first token as the command */
		char *cmd = strtok(str, " ");
		if(cmd == NULL) { continue; }

		/* treat the remaining string as the arguments,
		 * which may need further parsing
 		 */
		char *args = cmd + strlen(cmd) + 1;
 		if(args >= str_end) {
			args = NULL;
		}

#ifdef HAS_DEVICE
		extern void sdl_clear_event_queue(void);
		sdl_clear_event_queue();
#endif

		int i;
 		for(i = 0; i < NR_CMD; i ++) {
 			if(strcmp(cmd, cmd_table[i].name) == 0) {
				if(cmd_table[i].handler(args) < 0) { return; }
				break;
			}
		}

		if(i == NR_CMD) { printf("Unknown command '%s'\n", cmd); }
 	}
}
