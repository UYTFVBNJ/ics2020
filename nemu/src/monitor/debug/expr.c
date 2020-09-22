#include <isa.h>

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <regex.h>
#include <stdlib.h>
enum {
  TK_NOTYPE = 256, TK_EQ,

  /* TODO: Add more token types */

};

static struct rule {
  char *regex;
  int token_type;
} rules[] = {

  /* TODO: Add more rules.
   * Pay attention to the precedence level of different rules.
   */

// GH: own changes
	
  {" +", TK_NOTYPE},    // spaces

  {"\\+", '+'},         // plus
	{"-", '-'},         // minus
	{"\\*", '*'},         // times
	{"/", '/'},         // devide
	
	{"\\(", '('},       // l bracket 
  {"\\)", ')'},       // r bracket 

	{"[0-9]+", '0'},       // numbers

  {"==", TK_EQ},        // equal

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

static Token tokens[32] __attribute__((used)) = {};
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
        char *substr_start = e + position;
        int substr_len = pmatch.rm_eo;

        Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s",
            i, rules[i].regex, position, substr_len, substr_len, substr_start);

        position += substr_len;

        /* TODO: Now a new token is recognized with rules[i]. Add codes
         * to record the token in the array `tokens'. For certain types
         * of tokens, some extra actions should be performed.
         */

        switch (rules[i].token_type) {
					case TK_NOTYPE: 
						break;
					case '0': {
						tokens[nr_token].type=rules[i].token_type;
						for (int j=0;j<substr_len && j<32;j++) 
							tokens[nr_token].str[j]=*(e+position-substr_len+j);
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

bool check_parentheses(int p, int q) {
	if (tokens[q].type!='(' || tokens[q].type!=')') return 0;
	int cnt=0;
	for (p=p+1;p!=q && cnt>=0;p++) if (tokens[p].type=='(') cnt++; else if (tokens[p].type==')') cnt--;
	return (p==q && cnt==0); 
}

word_t eval(int p, int q) {
	if (p > q) {
		puts("BAD EXPRESSION!");
		return 0;
	} else if (p==q) {
		return strtol(tokens[q].str,NULL,10); 
	}
	return 0;
}

word_t expr(char *e, bool *success) {
  if (!make_token(e)) {
    *success = false;
    return 0;
  }
	
	for (int i=0;i<nr_token;i++) printf("%d\n%s\n",tokens[i].type,tokens[i].str);

  /* TODO: Insert codes to evaluate the expression. */
  TODO();

  return 0;
}

// GH: ownchanges
