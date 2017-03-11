#include "nemu.h"
#include "stdio.h"
#include "stdlib.h"
 //Type 'man regex' for more information about POSIX regex functions.
#include <sys/types.h>
#include <regex.h>

enum {
	NOTYPE = 256, 
	EQ = 257,
    dec = 258,
	hex = 259,
	dand = 260,
	dor = 261,
    eb = 263,
	es = 264,
	lm = 265,
	rm = 266,
	DEREF = 267,
	REG = 268,
	/* TODO: Add more token types */

};

static struct rule {
	char *regex;
	int token_type;
} rules[] = {

  	/* TODO: Add more rules.
	 * Pay attention to the precedence level of different rules.
 	  */
    {"=",'='},
	{"<",'<'},
	{"%",'%'},
	{">",'>'},
	{"\\|",'|'},
	{"&",'&'}, 
	{" +",	NOTYPE},				// spaces
	{"\\+", '+'},					// plus
	{"==", EQ},						// equal
	{"<=",es},
	{">=",eb},
	{"/",'/'},
	{"-",'-'},
	{"<<",lm},
	{">>",rm},
	{"\\*",'*'},                    // *
	{"\\^",'^'},             		// ^
	{"\\(",'('},                    // left bracket
	{"\\)",')'},                    // right bracket
    {"&&",dand},
	{"\\|\\|",dor},
	{"!",'!'}, 
    {"[0-9]{1,10}",dec},                  // decimalist
	{"0x[0-9A-Fa-f]{0,8}",hex},            // hexi
	{"\\$[a-z]{2,3}",REG},

};

#define NR_REGEX (sizeof(rules) / sizeof(rules[0]) )

static regex_t re[NR_REGEX];

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
	int i;
	char error_msg[128];
	int ret;

	for(i = 0; i < NR_REGEX; i ++) {
		ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
 		if(ret != 0) {
			regerror(ret, &re[i], error_msg, 128);
			Assert(ret == 0, "regex compilation failed: %s\n%s", error_msg, rules[i].regex);
		}
 	}
}

typedef struct token {
	int type;
	char str[32];
} Token;

Token tokens[32];
int nr_token;

static bool make_token(char *e) {
	int position = 0;
	int i;
	regmatch_t pmatch;
	nr_token = 0;
   
      while(e[position] != '\0'){	
		/* Try all rules one by one. */
    		for(i = 0; i < NR_REGEX; i ++)  {
    			if(regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
				char *substr_start = e + position;
				int substr_len = pmatch.rm_eo;
          
	Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s", i, rules[i].regex, position,substr_len,substr_len, substr_start);
	      position += substr_len;
          
     				/* TODO: Now a new token is recognized with rules[i]. Add codes 
				 * to record the token in the array ``tokens''. For certain 
				 * types of tokens, some extra actions should be performed.
  			 	 */ 

  	 			switch(rules[i].token_type) {
					case '+':
					case '-':
					case '*':
					case '/':
					case '(':
					case ')':
					case '!':
					case '%':
					case '^':
					case '|':
					case '&':
					case dor:
					case dand:
					case lm:
					case rm:
					tokens[nr_token].type = rules[i].token_type;
					nr_token ++;
					break;
				 	case hex:			 		
					case dec:
					tokens[nr_token].type = rules[i].token_type;
					strcpy(tokens[nr_token].str,substr_start);
					nr_token++;
	            	break;
					  default :panic("please implement me");
				    
         		 		}		
	           	break;
          	} 
   	}
     	   	if(i == NR_REGEX) {
 		printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
			return false;
 		}
     }
	return true; 
}

 bool check_parentheses(p,q){
   int i=0,j=0;
     while(p<=q){
    if(tokens[p].type == '(' ) i++;
	if(tokens[p].type == '(' && p != 0 && i == 0) return false; 
	if(tokens[p].type == ')') i--;
	if(tokens[p].type == ')' && i == 0)  return false;
	else j++;
    p++;
    }
	 printf("j=%d",j);
	 printf("q=%d",q);
   if(i == 0 && j-1 != q) return true;
   else return false;
}
int dominant(int p,int q){
   int i = p;
   int max = 0;
     while(p<=q){
      if(tokens[p].type != dec || tokens[p].type != hex){
	     max = p;
       for(i=0;i<= q;i++){
        if(tokens[i].type >= tokens[max].type && tokens[i].type!=dec && tokens[i].type != hex)
            max = i; 
 	      }
          }  
        p++;
       }
   return max;
   }

uint32_t eval(int p,int q){
        if(p > q) {
		printf("Bad expression");
		return 0;
    	}
 	  else if(p == q){
		  if(tokens[p].type == dec){
			  int sum;
			  sum = atoi(tokens[p].str);
			 printf("sumsum %d",sum);
		     return sum;
 		  }
		  else {
		  int hexnum;
		  sscanf(tokens[p].str,"%x",&hexnum);
		  return hexnum;
 		  }
          }  
	   else if(check_parentheses(p,q)== true){
              	return eval(p +1,q - 1);
 	   } 
  	 else{               //dominant operator
		int op;
		int val1,val2;
		op = dominant(p , q);
		val1 = eval(p , op - 1);
		val2 = eval(op + 1, q);
		printf("op %d val %d  %d\n",op,val1,val2);
 	 	switch(tokens[op].type){
			case '+':return  val1 + val2; 
			case '-':return  val1 - val2;
			case '/':return  val1 / val2;
			case '*':return  val1 * val2;
			case '&':return  val1 & val2;
			case '|':return  val1 | val2; 
			case dand:return  val1 && val2; 
			case dor:return  val1 || val2;
			case '^':return  val1 ^ val2;
			case '%':return  val1 % val2;
			default:
			break;
 	     	}
	}
	 return 0;
}
uint32_t expr(char *e, bool *success) {
 	if(!make_token(e)) {
		*success = false;
		return 0;
     	}

    printf("the expression: %d",eval(0,nr_token-1));	
 	/* TODO: Insert codes to evaluate the expression. */
 // panic("please implement me");
	return 0;
}
