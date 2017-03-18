#include "monitor/watchpoint.h"
#include "monitor/expr.h"
#include "nemu.h"

#define NR_WP 32
void cpu_exec();
void ui_mainloop();
static WP* new_wp();

static void free_wp(WP *wp);
static WP wp_list[NR_WP];
static WP *head, *free_;

void init_wp_list() {
	int i;
  	for(i = 0; i < NR_WP; i ++) {
		wp_list[i].NO = i;
		wp_list[i].old_value = 0;
		wp_list[i].new_value = 0;
		wp_list[i].e[0] = '\0' ;
		wp_list[i].next = &wp_list[i + 1];
  	}
	wp_list[NR_WP - 1].next = NULL;

	head = NULL;
	free_ = wp_list;
}

/* TODO: Implement the functionality of watchpoint */

static WP* new_wp(){
WP *p;
if(free_ == NULL) 
     	assert(0);
p = free_;
free_ = p -> next;
return p;
}

int set_watchpoint(char *e){
WP *new;
new = new_wp();
strcpy(new->e,e);
if(head == NULL)
{
   	head = new;
	new->next = NULL;
    }
else {
WP *q;
q = head->next;
while(q){
  if(q->next == NULL) break;
  q = q->next;
 }
if(q->next == NULL){
   q->next = new;
   new->next = NULL;
} 
}  
  printf("WATCHPOINT!\n");
  printf("%s",head->e);
  return 0;
}

bool compare_wp(WP* new){
	bool *success = 0 ;
	printf("ffffffff%dggggggggggg%s0\n",new->NO,head->e);
	new->old_value = expr(new->e,success);
	new->new_value = expr(new->e,success);	
	if(new->old_value != new->new_value) {
		new->old_value = new->new_value;
		new->new_value = expr(new->e,success);
		return true;
 	}  
	else   return false;
 }
 static void free_wp(WP *wp){
WP* p;
p = free_;
while(p->next){
 if(p->next == NULL) break;
 p = p->next;
 }  
if (p->next == NULL){
 p->next = wp;
wp->next = NULL;
}
}

bool delete_watchpoint(int NO){
WP *q;
WP *p;
q = head;
p = head->next;
 if(q->NO == NO){
free_wp(head);
q->old_value = 0;
q->new_value = 0;
q->e[0] = '\0';
head = p;
}
while(p){
	if(p->NO == NO) break;
	p = p->next;
	q = q->next;
 }
if(p->NO == NO){
  free_wp(p);
  p->old_value = 0;
  p->new_value = 0;
  p->e[0] = '\0';
  q->next = p->next;
  return true;
}  
else 
return false;
} 

 bool delete_all(){
  WP* p;
while(head->next!= NULL){
  p = head->next;
  head->next = p->next;
  free_wp(p);
  p->old_value = 0;
  p->new_value = 0;
  p->e[0] = '\0';
     }
   if(head->next == NULL){
	  free_wp(head);
	 head->old_value = 0;
	 head->new_value = 0;
     head->e[0] = '\0';
     head = NULL;	 
	  return true;
   }
  else  return false;
}
void list_watchpoint(WP* list){
   printf("watchpoint NO:%d\t old_value:%d\t new_value:%d\t expression:%s\n",list->NO,list->old_value,list->new_value,list->e);
} 


 bool scan_watchpoint(){
	 printf("%s",head->e);
 WP* p;
 p = head;
  while(p  != NULL){
 if(p->e != NULL){
	// printf("ddddd%syyyy\n",p->e);
 	 if(compare_wp(p)){
		 list_watchpoint(p); 
		 return true;
 	 }
 } 
   p = p->next;
        }
   return false;
}
