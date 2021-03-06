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
	NE = 269,
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
	{"%",'%'},
	{"\\|",'|'},
	{"&",'&'}, 
	{" +",	NOTYPE},				// spaces
	{"\\+", '+'},					// plus
	{"==", EQ},						// equal
	{"<=",es},
	{">=",eb},
	{"/",'/'},
	{"!=",NE},
	{"~",'~'},
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
	{"0x[0-9a-fA-F]{1,8}",hex},
    {"[0-9]{1,10}",dec},                  // decimalist        // hex
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
					case '~':
					case dor:
					case dand:
					case lm:
					case rm:
					case es:
					case eb:
					case EQ:
					case NE:	
					tokens[nr_token].type = rules[i].token_type;
					nr_token ++;
					break;
				 	case hex:			 		
					case dec:
					tokens[nr_token].type = rules[i].token_type;
					strcpy(tokens[nr_token].str,substr_start);
					nr_token++;
	            	break;
					case REG:
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
   int i,j=0,a=0;

   if(tokens[p].type == '(' && tokens[q].type == ')') 
   {
	   for(i =p+1;i<q;i++)
	   {
		  if(tokens[i].type == '(')  j++;
		  if(tokens[i].type == ')' && j!=0 ) j--;
		  if(tokens[i].type == ')' && j==0 && a==i-1) return true;
		  if(tokens[i].type == ')' && j==0) return false;
		  else a++;
	   }
	   if(j==0 )  return true;
	   else return false;   
    }
   else return false;
}
int sign(int a){
  int i=0;
switch(a){
  case '(':
  case ')':
	  i =1;
	  break;
  case '!':
  case '~':
	  i =2;
	  break;
  case '/':
  case '*':
  case '%':
	  i =3;
	  break;
  case '+':
  case '-':
      i = 4;
	  break;
  case '&':
  case '^':
  case '|':
	  i = 5;
	  break;
  default:
		  break;
  } 
  return i;
}


int dominant(int p,int q){
	int i=p,a=0,b=0,k=0;
       while(i<=q){    
	if(tokens[i].type != dec && tokens[i].type != hex) break;
    i++;
     }
       if(tokens[i].type != dec && tokens[i].type !=hex) {
   		a = sign(tokens[i].type);
	    k =i;	 	
  	 }

       while(i<=q){
  		 if(tokens[i].type != dec && tokens[i].type !=hex){
		       b = sign(tokens[i].type);
			 if(tokens[i].type == '(')
 	 		 { 
				 while(i<=q)
 				 {
				if(tokens[i].type == ')')		  break;
				i++;
  	      		  }
			 
  			 }
	          if(b>=a)
 			  {
				  k = i;
				  a = b;
  	 		  }
   		 }
		 i++;
      }
//     printf("kkk=%d",k);
	 return k;
}
uint32_t eval(int p,int q){
        if(p > q) {
		printf("Bad expression\n");
		return 0;
      	}
  	  else if(p == q){
  		  if(tokens[p].type == dec){
			  int n;
			  n = atoi(tokens[p].str);
		      return n;
  		  }
		  if(tokens[p].type == hex) {
		  int hexnum;
		  sscanf(tokens[p].str,"%x",&hexnum);
		  return hexnum;
   		  }
 		  if(tokens[p].type == REG){
            if(strcmp(tokens[p].str,"$eip")==0)
				  return cpu.eip;
		    	  
            if(strcmp(tokens[p].str,"$eax")==0)
				  return cpu.eax;
            if(strcmp(tokens[p].str,"$ecx")==0)
				  return cpu.ecx;
            if(strcmp(tokens[p].str,"$edx")==0)
				  return cpu.edx;
            if(strcmp(tokens[p].str,"$ebx")==0)
				  return cpu.ebx;
            if(strcmp(tokens[p].str,"$esp")==0)
				  return cpu.esp;
            if(strcmp(tokens[p].str,"$ebp")==0)
				  return cpu.ebp;
            if(strcmp(tokens[p].str,"$esi")==0)
				  return cpu.esi;
            if(strcmp(tokens[p].str,"$edi")==0)
				  return cpu.edi;
  	  }
 	  }
 	   else if(check_parentheses(p,q)== true){
              	return eval(p +1,q - 1);
   	   } 
   	 else{                //dominant operator
		int op;
		int val1,val2,val3;
//		printf("%d %d",p,q);
		op = dominant(p , q);
		if(tokens[op].type == tokens[op-1].type || tokens[op].type == tokens[op+1].type)
		{ 	
		int count=0,j=0;
		while(p<=q){
      		if(tokens[op].type == tokens[p].type) count++;
			else j++;
			p++;
 		}
		if(j<=1){
		if(count % 2 == 0) return atoi(tokens[q].str);
		else    return -1*atoi(tokens[q].str);
		}
 		else{
		if(count % 2 == 0) return atoi(tokens[p].str)+atoi(tokens[q].str);
        //many kinds
		}
  		}
      if(tokens[p+1].type == tokens[p+2].type){
 		  val1 = atoi(tokens[p].str);
		  val2 = atoi(tokens[q].str);
 	  switch(tokens[p+1].type){
		  case '&': return val1 && val2;
		  case '|': return val1 || val2;
		  case '=': return val1 == val2;
		  case '<': return val1 << val2;
		  case '>': return val1 >> val2;
		 default:
					break;
	  }
 	  }
 		if(op == 0){
		val3 = eval(op+1,q);
		switch(tokens[op].type)
	  	{
         case '!':return ! val3;
		 case '*':
		 sscanf(tokens[p].str,"%x",&op);
		 return swaddr_read(op,val3);
		 case '~':return ~ val3;
		 case '-':return -1 * val3; 
         default:
		 break;
  		}
   		}
		if(op != 0){
		val1 = eval(p , op - 1);
		val2 = eval(op + 1, q);
//	    printf("%d %d %d\n",op,val1,val2);
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
			case  NE:return  val1 != val2;
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
     	}
	 return 0;
}
uint32_t expr(char *e, bool *success) {
 	if(!make_token(e)) {
		*success = false;
		return 0;
      	}
//  printf("the expression: %d\n",eval(0,nr_token-1));	
 	/* TODO: Insert codes to evaluate the expression. */
 // panic("please implement me");
    int num = eval(0,nr_token-1);  
	return num;
}
