#include <isa.h>
#include <memory/vaddr.h>

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <regex.h>
#include <stdlib.h>

enum TOKEN {
  TK_NOTYPE = 256, TK_EQ, TK_NEQ, TK_HEX_NUM, TK_REG, TK_NUM, TK_LAND, TK_DEREF = '*' + TK_NOTYPE, TK_UNMINUS = '-' + TK_NOTYPE, 

  /* TODO: Add more token types */

};

enum PRIORITY {
  PR_LOR, PR_LAND, PR_EQ, PR_NEQ = PR_EQ, PR_PM, PR_TD, PR_SIGOPT
};

static struct rule {
  char *regex;
  int token_type;
} rules[] = {

  /* TODO: Add more rules.
   * Pay attention to the precedence level of different rules.
   */

// GH: own changes
  {"0x([0-9]|[a-f])+", TK_HEX_NUM},
  {"\\$(\\$|[a-z]|[0-9]){2,3}", TK_REG},
	
  {" +", TK_NOTYPE},    // spaces

  {"\\+", '+'},         // plus
	{"-", '-'},         // minus

	{"\\*", '*'},         // times
	{"/", '/'},         // devide
	
	{"\\(", '('},       // l bracket 
  {"\\)", ')'},       // r bracket 

	{"[0-9]+", TK_NUM},       // numbers

  {"==", TK_EQ},        // equal
  {"!=", TK_NEQ},        // not equal
  
  {"&&", TK_LAND},        // and

// GH: own changes

};

#define NR_REGEX (sizeof(rules) / sizeof(rules[0]) )

static regex_t re[NR_REGEX] = {};

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
  int i;
  char error_msg[128];
  int ret;

  for (i = 0; i < NR_REGEX; i ++) {
    ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
    if (ret != 0) {
      regerror(ret, &re[i], error_msg, 128);
      panic("regex compilation failed: %s\n%s", error_msg, rules[i].regex);
    }
  }
}

typedef struct token {
  int type;
  char str[32];
} Token;

static Token tokens[60000] __attribute__((used)) = {};
static int nr_token __attribute__((used))  = 0;

// GH: own changes

static bool make_token(char *e) {
  int position = 0;
  int i;
  regmatch_t pmatch;

  nr_token = 0;

  while (e[position] != '\0') {
    /* Try all rules one by one. */
    for (i = 0; i < NR_REGEX; i ++) {
      if (regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
        // char *substr_start __attribute__((used)) = e + position;
        int substr_len = pmatch.rm_eo;

        // Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s",
            // i, rules[i].regex, position, substr_len, substr_len, substr_start);

        position += substr_len;

        /* TODO: Now a new token is recognized with rules[i]. Add codes
         * to record the token in the array `tokens'. For certain types
         * of tokens, some extra actions should be performed.
         */

        switch (rules[i].token_type) {
					case TK_NOTYPE: 
						break;

					case TK_NUM: {
						tokens[nr_token].type=rules[i].token_type;

						for (int j=0;j<32;j++) 
							tokens[nr_token].str[j]=(j<substr_len)?(*(e+position-substr_len+j)):0;

						nr_token++;
						break;
					}

          case TK_REG: {
            tokens[nr_token].type=rules[i].token_type;

            substr_len--; // change initial pos

						for (int j=0;j<32;j++) 
							tokens[nr_token].str[j]=(j<substr_len)?(*(e+position-substr_len+j)):0; 

						nr_token++;
						break;      
          }

          case TK_HEX_NUM: {
            tokens[nr_token].type=rules[i].token_type;

            substr_len-=2; //change initial pos

						for (int j=0;j<32;j++) 
							tokens[nr_token].str[j]=(j<substr_len)?(*(e+position-substr_len+j)):0; 

						nr_token++;
						break;      
          }

					default:
						tokens[nr_token++].type=rules[i].token_type;
        }

        break;
      }
    }

    if (i == NR_REGEX) {
      printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
      return false;
    }
  }

  return true;
}

// GH: own changes

bool check_parentheses(int p, int q, bool *success) {	
	int cnt=0;
	for (int i=p;i!=q+1 && cnt>=0;i++) if (tokens[i].type=='(') cnt++; else if (tokens[i].type==')') cnt--;
	*success = (cnt==0) && (*success);
	for (int i=p+1;i!=q && cnt>=0;i++) if (tokens[i].type=='(') cnt++; else if (tokens[i].type==')') cnt--;
	return (tokens[p].type=='(' && tokens[q].type==')' && cnt==0); 
}

word_t eval(int p, int q, bool *success) {
  
  // printf("%d %d\n",p,q);

	if (p > q) {
		Log("ERR: BAD EXPRESSION!");
		*success = 0;
		return 0;
	} 
  
  else if (p==q) {
    int radix; 
    switch (tokens[p].type) {
      case TK_HEX_NUM:
        radix = 16;
        break;
      case TK_NUM:
        radix = 10;
        break;
      case TK_REG:
        return isa_reg_str2val(tokens[p].str, success);
        break;
      default:
        Log("ERR: unknown radix");

        *success = 0;
        radix = 10;
        break;
    }

    unsigned int ans = strtol(tokens[p].str, NULL, radix); 
    // printf("%u %ld\n",ans, strtol(tokens[p].str,NULL,10));
		return ans;
	} 
  
  else if (check_parentheses(p,q,success)) 
		return eval(p+1,q-1,success);
	
  
  else if (*success) {

    word_t left_exp = 0, right_exp = 0;
		int cnt=0, i;
    int opt_pos, pos[10]; // index stands for priorities +

    memset(pos, -1, sizeof(int)*10);

		for (i=p;i<=q;i++) switch (tokens[i].type) {
      case TK_DEREF: case TK_UNMINUS: if (cnt==0) pos[PR_SIGOPT] = i; break;

    	case '*': if (cnt==0) pos[PR_TD] = i; break;
			case '/': if (cnt==0) pos[PR_TD] = i; break;

			case '+': if (cnt==0) pos[PR_PM] = i; break;
	 		case '-': if (cnt==0) pos[PR_PM] = i; break;
      
      case TK_EQ:  if (cnt==0) pos[PR_EQ]  = i; break;
	 		case TK_NEQ: if (cnt==0) pos[PR_NEQ] = i; break;


			case '(': cnt++; break;
			case ')': cnt--; break;
			default : 
				break;
		}


		for (i = 0; i < 10; i++) if (pos[i] != -1) break;
    opt_pos = pos[i];

    switch (i) {
      case PR_SIGOPT:
        break;

      default:
        left_exp  = eval(p, opt_pos - 1, success); 
        if (!*success) return 0;
        break;
    }

		right_exp = eval(opt_pos + 1, q, success);
		if (!*success) return 0;


		switch (tokens[opt_pos].type) {
			case '*': return left_exp * right_exp; break;
			case '/': return left_exp / right_exp; break;

			case '+': return left_exp + right_exp; break;
			case '-': return left_exp - right_exp; break;

      case TK_EQ:  return (left_exp == right_exp); break;
      case TK_NEQ: return (left_exp != right_exp); break;

      case TK_LAND: return (left_exp && right_exp); break;

      case TK_DEREF: return vaddr_read(right_exp, 4); break;

      case TK_UNMINUS: return - right_exp; break;

			default: 
        Log("ERR");
        assert(0);
		}
	}
	return 0;
}

word_t expr(char *e, bool *success) {
  if (!make_token(e)) {
    *success = false;
    return 0;
  }
	
	// for (int i=0;i<nr_token;i++) printf("%c\n%s\n",tokens[i].type,tokens[i].str);

  for (int i=0;i<nr_token;i++) if (tokens[i].type == '*' || tokens[i].type == '-') {
    if (i == 0) {
      tokens[i].type += TK_NOTYPE; // + TK_NOTYPE = unary opt
      continue;
    } else {
      switch (tokens[i-1].type) {
      case ')':
        break;
      case TK_HEX_NUM:
        break;
      case TK_REG:
        break;
      case TK_NUM:
        break;
      
      default:
        tokens[i].type += TK_NOTYPE;
        break;
      }      
    }
  }

  /* TODO: Insert codes to evaluate the expression. */

  return eval(0, nr_token-1, success);
}

// GH: ownchanges
