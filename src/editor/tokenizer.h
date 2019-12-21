#ifndef EDITOR_TOKENIZER_H
#define EDITOR_TOKENIZER_H

#include <regex.h>

#include <data_struct/string.h>

enum c_token_types {
  KEYWORD_TOKEN,
  DEFAULT_TYPE_TOKEN,
  IDENTIFIER_TOKEN,
  CONSTANT_TOKEN,
  STRING_TOKEN,
  WHITESPACE_TOKEN,
  OPERATOR_TOKEN,
  COMMENT_TOKEN,
  UNKNOWN_TOKEN
};

typedef struct token {
  regex_t regex;
  int type;
} token;

typedef struct tokenizer {
  token tokens[9];
  size_t num_tokens;
} tokenizer;

void token_init(token *tok, const char *str, int type) {
  regcomp(&tok->regex, str, REG_EXTENDED);
  tok->type = type;
}

void tokenizer_init(tokenizer *tok) {
  size_t i = 0;
  token_init(&(tok->tokens[i++]), "^(auto|break|case|const|continue|default|"
                                  "do|else|enum|extern|for|goto|if|register|return|sizeof|static|struct|"
                                  "switch|typedef|union|volatile|while)", KEYWORD_TOKEN);
  token_init(&(tok->tokens[i++]), "^(char|double|float|int|long|short|signed|unsigned|void)", DEFAULT_TYPE_TOKEN);
  token_init(&(tok->tokens[i++]), "^[a-zA-Z_][a-zA-Z_0-9]+", IDENTIFIER_TOKEN);
  token_init(&(tok->tokens[i++]), "^(0[xX][0-9a-fA-F]+[uUlL]?|"
                                  "[0-9]+[uUlL]?|"
                                  "[0-9]+[eE][+-]?[0-9]+[fFlL]?|"
                                  "[0-9]*\\.[0-9]+([eE][+-]?[0-9]+)?[fFlL]?|"
                                  "[0-9]+\\.[0-9]*([eE][+-]?[0-9]+)?[fFlL]?)", CONSTANT_TOKEN);
  token_init(&(tok->tokens[i++]), "^L?\"(\\\\.|[^\"\\\\])*\"", STRING_TOKEN);
  token_init(&(tok->tokens[i++]), "^\\s", WHITESPACE_TOKEN);
  token_init(&(tok->tokens[i++]), "^(\\+=|-=|\\*=|/=|%=|&=|\\^=|\\|=|>>|<<|"
                                  "\\+\\+|--|->|\\.|&&|\\|\\||>=|<=|==|!=|"
                                  ":|=|&|!|~|-|\\+|\\*|/|%|<|>|\\^|\\||\\?)", OPERATOR_TOKEN);
  token_init(&(tok->tokens[i++]), "^//.*", COMMENT_TOKEN);
  token_init(&(tok->tokens[i++]), "^.", UNKNOWN_TOKEN);
  tok->num_tokens = i;
}

void try_match_token(const char *str, int *size, int *type, const token *tok) {
  regmatch_t pmatch;
  int res = regexec(&tok->regex, str, 1, &pmatch, 0);
  if (res) {
    return;
  }
  if (*size < pmatch.rm_eo) {
    *size = pmatch.rm_eo;
    *type = tok->type;
  }
}

void get_token_type(const char *str, int *size, int *type, const tokenizer *tok) {
  size_t i;
  *size = 0;
  for (i = 0; i < tok->num_tokens; ++i) {
    try_match_token(str, size, type, &(tok->tokens[i]));
  }
}

void tokenize(const char *str, int *types, const tokenizer *tok) {
  size_t it = 0;
  int size, type;
  while (str[it] != '\0') {
    get_token_type(str + it, &size, &type, tok);
    int i;
    for (i = 0; i < size; ++i) {
      types[it + i] = type;
    }
    it += size;
  }
}

void tokenize_str(const string *str, int *types, const tokenizer *tok) {
  char s[str->size + 1];
  string_as_c_str(str, s);
  tokenize(s, types, tok);
}

// void debug_tokenize(const char *str, const tokenizer *tok) {
//   size_t it = 0;
//   int size;
//   int type;
//   const char *id2token[] = {"KEYWORD_TOKEN", "DEFAULT_TYPE_TOKEN", "IDENTIFIER_TOKEN", "CONSTANT_TOKEN", "STRING_TOKEN", "WHITESPACE_TOKEN", "UNKNOWN_TOKEN"};
//   while (str[it] != '\0') {
//     get_token_type(str + it, &size, &type, tok);
//     if (type != WHITESPACE_TOKEN) {
//       fwrite(str + it, size, 1, stdout);
//       printf("\t %s\n", id2token[type]);
//     }
//     it += size;
//   }
// }

#endif /* end of include guard: EDITOR_TOKENIZER_H */
