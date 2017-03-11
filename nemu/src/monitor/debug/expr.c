#include "nemu.h"
#include "stdio.h"
#include "stdlib.h"
#include <math.h>
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
					case '<':
					case '>':
					case dor:
					case dand:
					case lm:
					case rm:
					case es:
					case eb:
					case EQ:	
					if(rules[i+1].token_type == rules[i].token_type){
						if(rules[i].token_type == '<')
						   tokens[nr_token].type = lm;
						else   tokens[nr_token].type = rm;
						nr_token = nr_token +1;
					}
					else{
					tokens[nr_token].type = rules[i].token_type;
					nr_token ++;
					}
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
   if(tokens[p].type == '(' && tokens[q].type == ')') return true;
   else return false;
}
int sign(int a){
char s[]={'(',')','/','*','%','+','-','<','>','&','^','|'};
int i =0;
for(i =0;;i++)
	if(a == s[i]) break;
return i;
}
int dominant(int p,int q){
   int a,b,temp,h=0,k = p,j=0;
   int max = 0;
   int sort[10];
         while(p<=q){
           if(tokens[p].type == '('){
    		   j++;p++;
  	         while(p<=q){
	   if(tokens[p].type == '(')
		   j++;
	   if(tokens[p].type == ')')
		   j--;
	   if(j == 0) break;
	   p++; 
	     } 
 	   } 
	   if(tokens[p].type == ')')   p++;
       
        if(tokens[p+1].type == '(' && p+1 <= q)
        printf("error");
	  p++;
		 }
		 p = k;
		
	 	while(p<=q){
	   if(tokens[p].type != dec && tokens[p].type != hex && tokens[p].type != '(' && tokens[p].type != ')'){
		sort[h] = sign(tokens[p].type);
	    h++;
 	 	}
	   p++;
 	 	}
		while(h<10){
			sort[h] = 0;
			h++;
		}
		for(a=0;a<10;a++)
            for(b=0;b<10-1-a;b++)
			{
				if(sort[b]<sort[b+1]){
					temp = sort[b];
					sort[b] = sort[b+1];
					sort[b+1] = temp;
				}
			
			}
		for(a=0;a+1<10;a++)
			if(sort[a+1] == 0) break;
	    max = a;
   return max;
   }

uint32_t eval(int p,int q){
        if(p > q) {
		printf("Bad expression");
		return 0;
    	}
 	  else if(p == q){
		  if(tokens[p].type == dec){
			  int n;
			  n = atoi(tokens[p].str);
		      return n;
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
  	 else{                //dominant operator
		int op;
		int val1,val2;
		op = dominant(p , q);
		printf("%d\n",op);
		val1 = eval(p , op - 1);
		val2 = eval(op + 1, q);
		printf("%d %d\n",val1,val2);
 	 	switch(tokens[op].type){
			case '+':return  val1 + val2; 
			case '-':return  val1 - val2;
			case '/':return  val1 / val2;
			case '*':return  val1 * val2;
			case '&':return  val1 & val2;
			case '<':return  val1 < val2;
			case '>':return  val1 > val2;
			case  EQ:return  val1 == val2;
			case  eb:return  val1 >= val2;
			case  es:return  val1 <= val2;
			case '|':return  val1 | val2; 
			case dand:return val1 && val2; 
			case dor:return  val1 || val2;
			case '^':return  val1 ^ val2;
			case '%':return  val1 % val2;
			case lm:return val1 >> val2;
			case rm:return val1 << val2;
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
    printf("the expression: %d\n",eval(0,nr_token-1));	
 	/* TODO: Insert codes to evaluate the expression. */
 // panic("please implement me");
	return 0;
}
