#include "nemu.h"
#include "stdlib.h"
#include "stdio.h"
/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>

enum {
	NOTYPE = 256, 
	EQ = 257,
    dec = 258,
	hex = 259,
	dand = 260,
	dor = 261,
	xor = 262,

	/* TODO: Add more token types */

};

static struct rule {
	char *regex;
	int token_type;
} rules[] = {

  	/* TODO: Add more rules.
	 * Pay attention to the precedence level of different rules.
 	  */
/*    {"=",'='},
	{"<",'<'},
	{">",'>'},
	{"\\|",'|'},
*/	{"&",'&'},
	{" +",	NOTYPE},				// spaces
	{"\\+", '+'},					// plus
	{"==", EQ},						// equal
	{"/",'/'},
	{"-",'-'},
    {"\\[",'['},	              	// [ 
	{"\\{",'{'},                    // {
	{"\\*",'*'},                    // *
	{"\\|",'|'},                    // |
	{"\\^",'^'},             		// ^
	{"\\(",'('},                    // left bracket
	{"\\)",')'},                    // right bracket
  /*  {"&&",dand},
	{"||",dor},
	{"!",'!'},
    */
    {"[0-9]{1,10}",dec},                  // decimalist
	{"0x[0-9A-Fa-f]{0,8}",hex},            // hex

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
				    case ' ':
					break;	
					case '\0':
					break;
					case '+':
					tokens[nr_token].type = '+';
					break;
					case '-':
					tokens[nr_token].type = '-';
					break;
					case '*':
					tokens[nr_token].type = '*';
					break;
					case '/':
					tokens[nr_token].type = '/';
					break;
					case '(':
					tokens[nr_token].type = '(';
					break;
					case ')':
					tokens[nr_token].type = ')';
					break;
				 	case hex:
                    tokens[nr_token].type = hex;
                    break;			 		
					case dec:
		sprintf(tokens[nr_token].str,"%.*s",substr_len,substr_start);
	 /*				tokens[nr_token].type = dec;
					for(j=0;j < substr_len;j++)
				    	tokens[nr_token].str[j] = substr_start[j];
					tokens[nr_token].str[j] = '\0';
	*/				break;
					default: panic("please implement me");
     		 		}		
	    if(rules[i].token_type != ' ')
		          nr_token++;
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

static bool check_parentheses(p,q){
   int i=1;
    while(p<=q){
    if(tokens[p].type == '(') i++;
	if(tokens[p].type == ')') i--;
	if(tokens[p].type == ')' && i == 0)  return false;
    p++;
   }
   printf("Ipp");
   Log("ddddddddddd");
   if(i == 1) return true;
   else return false;
}
int dominant(int p,int q){
   int i = p;
   int max = 0;
   while(p<q){
   if(tokens[p].type != dec){
	   max = p;
   for(i=p;i<= p;i++)
   if(tokens[i].type >= tokens[i+1].type && tokens[p+1].type!=dec)
     max = p; 
   }
   p++;
}
   return max;
   }

 int eval(int p,int q){
     if(p > q) {
		printf("Bad expression");
		return false;
    	}
	else if(p == q){
	     int i,j,sum=0,n=1;
         
	     for(i=0;;i++)
         if(tokens[p].str[i]== '\0') break;
	     j =i-1;
	     for(i = j;i<=0;i--){
	     sum = sum + (tokens[p].str[i] - '0')*n;
	     n = n*10;
 
 	 	 }
		 Log("%d",sum);
	     return sum;
     } 
 	else if(check_parentheses(p,q))
            	return eval(p +1,q - 1);
 	else{               //dominant operator
    	int op;
        int val1,val2; 
        op = dominant(p,q);
		printf("%d",op);
		Log("opopopop");
    	val1 = eval(p, op - 1);
    	val2 = eval(op + 1, q);
   	switch(op){
	  case '+':return val1 + val2;
  	  case '-':return val1 - val2;
	  case '/':return val1 / val2;
	  case '*':return val1 * val2;
	  default: assert(0);
  	   }
    }
}


uint32_t expr(char *e, bool *success) {
 	if(!make_token(e)) {
		*success = false;
		return 0;
    	}
/* 	for(i = 0;i < nr_token; i ++){
 	 if(tokens[i].type == '*' 
	&& (i == 0||tokens[i - 1].type == dec)){
	
		tokens[i].type = DEREF;
 	}
	}
*/
   	int num;
	printf("hello");
	num = eval(0,nr_token-1);
    printf("%d",num);
	*success = true;
	return num;  
 	/* TODO: Insert codes to evaluate the expression. */
	panic("please implement me");
}
