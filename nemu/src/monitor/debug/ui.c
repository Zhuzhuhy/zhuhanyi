#include "monitor/monitor.h"
#include "monitor/expr.h"
#include "monitor/watchpoint.h"
#include "nemu.h"

#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
void cpu_exec(uint32_t);
int set_watchpoint(char *e);
int set_breakpoint(char *e);
bool delete_watchpoint(int NO);
bool scan_watchpoint();
bool scan_watchpoint_all();
bool delete_all();

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
static int cmd_p(char *args);
static int cmd_w(char *args);
static int cmd_d(char *args);
static int cmd_b(char *args);

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
	{ "p","Expression evaluation",cmd_p },
	{ "w","Set a watchpoint",cmd_w},
	{ "d","Delete a watchpoint",cmd_d},
	{ "b","Set a breakpoint",cmd_b},
	/* TODO: Add more commands */
};

#define NR_CMD (sizeof(cmd_table) / sizeof(cmd_table[0]))

static int cmd_w(char *arge){
	char *str;
	str = strtok(NULL," ");
	if(set_watchpoint(str) == 0)  printf("set success\n");
	else      printf("Error\n");
	return 0;
}
static int cmd_b(char *arge){
	char *str;
	str = strtok(NULL," ");
	if(set_breakpoint(str) == 0)  printf("set success\n");
	else      printf("Error\n");
	return 0;
}
static int cmd_d(char *arge){
  char *str;
  str = strtok(NULL," ");
if(str[0] >='0' && str[0]<='9'){
  int num;
  sscanf(str,"%d",&num);
  if(delete_watchpoint(num)) printf("Delete %d watchpoint\n",num);
  else printf("Error\n");
  }
else{
	if(strcmp(str,"all") == 0){
      if(delete_all()) printf("Delete all\n");
      else  printf("Error\n");
 	}
	else printf("Error\n");
} 
return 0;
}

static int cmd_p(char *args){
	bool *success=0;
	char *str;
    str = strtok(NULL," ");
//	expr(str,success);
    int result = expr(str,success);
	printf("the result:%d\n",result);
	return 0;
}
static int cmd_si(char *args) {

    char *str;
    int num;	
    str = strtok(args," ");/*strtok() cut off*/
    num = atoi(str);/*turn number*/
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
		i++;/*cpu_state,reg.h*/
  	}
	printf("eip :%08x %d \n",cpu.eip,cpu.eip);
     	}
 	else if(strcmp(ch,"w")==0) {
		          if(scan_watchpoint_all()) printf("print success\n");
	              else printf("NO WATCHPOINT!\n");
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
	 sscanf(locate_start,"%x",&num_s); /*turn string to locate*/
	 printf("start addr:%s  len:%d\n",locate_start,num);
 	 while(count < num){
  	 locate = swaddr_read(num_s,1);/*return virtual memory*/
     if(count%5 == 0) 
		 printf("\n%02x : ",num_s);
	 printf("%02x  ",locate);
	 num_s++;
	 count++;
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
