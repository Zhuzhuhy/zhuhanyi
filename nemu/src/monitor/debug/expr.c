#include "nemu.h"
#include "stdlib.h"
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
				    case 0:
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
					tokens[nr_token].type = dec;
		sprintf(tokens[nr_token].str,"%.*s",substr_len,substr_start);
	            	break;
					default: panic("please implement me");
       		 		}		
		          nr_token++;
		break;
        	} 
 	}
     	   	if(i == NR_REGEX) {
 		printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
			return false;
		}
   }
	return 0; 
}

static bool check_parentheses(p,q){
   int i=0,j=0;
    while(p<=q){
    if(tokens[p].type == '(') i++;
	if(tokens[p].type == ')') i--;
	if(tokens[p].type == ')' && i == 0)  return false;
	else j++;
    p++;
    }
	printf("%d",j);
	Log("check");
   if(i == 0 && j-1 != q) return true;
   else return false;
}
int dominant(int p,int q){
   int i = p;
   int max = 0;
    while(p<=q){
    if(tokens[p].type != dec){
	   max = p;
       for(i=0;i+1<= q;i++)
            if(tokens[i].type >= tokens[i+1].type && tokens[i+1].type!=dec)
            max = i; 
        }
   p++;
     }
   printf("%d",max);
   Log("ffff");
   return max;
   }

 int eval(int p,int q){
        if(p > q) {
		printf("Bad expression");
		return 0;
    	}
 	  else if(p == q){
	     int i=0,j,sum=0,n=1;
         
 	     while(1){
             if(tokens[p].str[i]== '\0') break;
			 i++;
 	  	 }
	     j =i-1;
 	       for(i = j;i>=0;i--){
	     sum = sum + (tokens[p].str[i] - '0')*n;
	     n = n*10;
 	 	 }
	     return sum;
         }  
	   else if(check_parentheses(p,q)== true){
		       Log("do it");
              	return eval(p +1,q - 1);
	 } 
 	 else{               //dominant operator
		int op;
		int val1,val2;
		int num; 	
		op = dominant(p , q);
		val1 = eval(p , op - 1);
		val2 = eval(op + 1, q);
		printf("op%d val%d %d",op,val1,val2);
		Log("dftyyyyyfd");
 	 	switch(tokens[op].type){
			case '+':num = val1 + val2; 
                     break;
			case '-':num = val1 - val2;
					 break;
			case '/':num = val1 / val2;
					 break;
			case '*':num = val1 * val2;
					 break;
			default: assert(0);
 		}
	 
	printf("%d",num);
	Log("78954521");
   	}
	return 0;
}


uint32_t expr(char *e, bool *success) {
 	if(!make_token(e)) {
		*success = false;
		return 0;
     	}
    Log("hello"); 
	*success = true; 
	int num=0;
	Log("start");
	num = eval(0,nr_token-1);
    Log("the expression: %d",num);	
 	/* TODO: Insert codes to evaluate the expression. */
	panic("please implement me");
	return 0;
}
