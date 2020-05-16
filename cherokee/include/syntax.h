#ifndef _SYNTAX_H_
# define _SYNTAX_H_

#include "./list.h"
#include "./http.h"
#define CRLF "\r\n"

typedef enum s_item_type {
    METHOD, HTTP_VERSION, PATH, HEADER_KEY, HEADER_VALUE, BODY
}            t_item_type;

typedef struct s_lexeme {
    t_item_type     item_type;
    t_string        item;
}              t_lexeme;

typedef t_lexeme t_http_lexeme;
typedef t_list* t_http_lexemes;

t_bool  my_isalpha(char);
t_bool  my_isnum(char);
t_bool  my_isalphanum(char);
t_bool  my_isalpha_special(char);

t_http_lexemes  parse_request(t_string);

void    remove_lexeme(t_node *node);

t_http_lexeme    *createHttpLexeme(t_item_type, t_string);

t_request   *lexeme_to_request(t_http_lexemes);

/*
    Parse http request
*/
t_bool  isMethod(int, t_string, t_http_lexemes);
t_bool  isHttpVersion(int, t_string, t_http_lexemes);
t_bool  isAbsolutePath(int, t_string, t_http_lexemes);
t_bool  isRelativePath(int, t_string, t_http_lexemes);
t_bool  isPath(int, t_string, t_http_lexemes);
t_bool  isHeaderKey(int, t_string, t_http_lexemes);
t_bool  isHeaderValue(int, t_string, t_http_lexemes);
t_bool  isHeader(int, t_string, t_http_lexemes);
t_bool  isHeaderList(int, t_string, t_http_lexemes);
t_bool  isCRLF(int, t_string, t_http_lexemes);
t_bool  isBody(int, t_string, t_http_lexemes);

/* TODO: */
t_bool  isBodyJson(int, t_string, t_http_lexemes);
t_bool  isBodyText(int, t_string, t_http_lexemes);
t_bool  isBodyFormData(int, t_string, t_http_lexemes);
t_bool  isBodyMimeType(int, t_string, t_http_lexemes);

t_bool  matchCRLF(t_string);

#endif
