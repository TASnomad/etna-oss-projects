#include "./syntax.h"

//char	*strdup(char *);

t_bool  my_isnum(char c)
{
    return (c >= '0' && c <= '9');
}

t_bool  my_isalpha(char c)
{
    return ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'));
}

t_bool  my_isalphanum(char c)
{
    return (my_isnum(c) || my_isalpha(c));
}

t_bool  my_isalpha_special(char c)
{
    return (my_isalphanum(c) ||
            c == '-' ||
            c == '_' ||
            c == ';' || 
            c == '/' || 
            c == '(' || 
            c == ')' ||
            c == ' ' ||
            c == '.' ||
            c == ',' ||
            c == '*' ||
            c == '\'' ||
            c == '=' ||
            c == '?' ||
            c == '+' ||
            c == ':' ||
            c ==  '\"');
}

t_bool  valid_field_key(char c)
{
    return (my_isalphanum(c) ||
        (c != '(' &&
        c != ')' &&
        c != '<' &&
        c != '>' &&
        c != '@' &&
        c != ',' &&
        c != ';' &&
        c != '\\' &&
        c != '<' &&
        c != '/' &&
        c != '[' &&
        c != ']' &&
        c != '?' &&
        c != '=' &&
        c != '{' &&
        c != '}' &&
        c != ' ' &&
        c != '\n' &&
        c != '\t' && 
        c != '\r')
    );
}

void    delete_http_lexeme(t_node *node)
{
    t_http_lexeme   *lexeme;

    lexeme = (t_http_lexeme *)node->data;
    free(lexeme->item);
    free(lexeme);
}

t_http_lexeme    *createHttpLexeme(t_item_type type, t_string data)
{
    t_http_lexeme   *lexeme;

    if ((lexeme = (t_http_lexeme *)malloc(sizeof(*lexeme))) == NULL)
    {
        return (NULL);
    }
    lexeme->item = data;
    lexeme->item_type = type;
    return (lexeme);
}

void    remove_lexeme(t_node *node)
{
    t_http_lexeme   *lexeme;

    lexeme = (t_http_lexeme *)node->data;
    free(lexeme->item);
    free(lexeme);
}


t_http_lexemes  parse_request(t_string message)
{
    int             a;
    t_http_lexemes  lexemes;

    a = 0;
    if (((lexemes = new_list())) == NULL)
    {
        return (NULL);
    }
    if (!isMethod(a, message, lexemes))
    {
        printf("message invalid !\n");
        delete_list_exec_action(lexemes, &remove_lexeme);
        return (NULL);
    }
    return (lexemes);
}

t_bool  isMethod(int start, t_string message, t_http_lexemes lexemes)
{
    int     ret;
    char    *tab[10];
    int     i;

    i = 0;    
    tab[0] = "GET";
    tab[1] = "POST";
    tab[2] = "DELETE";
    tab[3] = "PUT";
    tab[4] = "PATCH";
    tab[5] = "HEAD";
    tab[6] = "OPTIONS";
    tab[7] = "TRACE";
    tab[8] = "CONNECT";
    tab[9] = 0;
    for (i = 0; i < 10; i++)
    {
        ret = match_str(tab[i], message, start);
        if (ret != - 1)
        {
            insert_node(lexemes, create_node(createHttpLexeme(METHOD, substring(message, start, ret))));
            if (!isPath(start + ret + 1, message, lexemes))
            {
                return (FALSE);
            }
            return (TRUE);
        }
    }
   return (FALSE);
}

t_bool  isHttpVersion(int start, t_string message, t_http_lexemes lexemes)
{
    int i;
    int ret;
    int j;

    i = 0;
    j = 0;
    if ((ret = match_str("HTTP/", message, start)))
    {
        if (ret != -1)
        {
            i += ret;
            j = i;
            while (message[start + i] && message[start + i] != '.')
            {
                if (!my_isnum(message[start + i]))
                {
                    return (FALSE);
                }
                i++;
            }
            if ((i - j) != 1)
            {
                return (FALSE);
            }
            i++;
            j = i;
            while (message[start + i] && !matchCRLF(message + start + i))
            {
                if (!my_isnum(message[start + i]))
                {
                    return (FALSE);
                }
                i++;
            }
            if ((i - j) != 1)
            {
                return (FALSE);
            }
            insert_node(lexemes, create_node(createHttpLexeme(HTTP_VERSION, substring(message, start + ret, start + i))));
            if (!isHeaderList(start + i + 2, message, lexemes))
            {
                return (FALSE);
            }
            return (TRUE);
        }
        return (FALSE);
    }
    return (FALSE);
}

t_bool  isAbsolutePath(int start, t_string message, t_http_lexemes lexemes)
{
    int i;

    i = 0;
    if (match_str("http://", message, start) != -1)
    {
        while (message[i + start] && message[i + start] != ' ')
        {
            i++;
        }
        if (i > 0)
        {
            insert_node(lexemes, create_node(createHttpLexeme(PATH, substring(message, start, start + i))));
            if (!isHttpVersion(start + i + 1, message, lexemes))
            {
                return (FALSE);
            }
            return (TRUE);
        }
    }
    return (FALSE);
}

t_bool  isRelativePath(int start, t_string message, t_http_lexemes lexemes)
{
    int i;

    i = 0;
    if (message[start] && message[start] == '/')
    {
        while (message[start + i] && message[start + i] != ' ')
        {
            i++;
        }
        if (i > 0)
        {
            insert_node(lexemes, create_node(createHttpLexeme(PATH, substring(message, start, start + i))));
            if (!isHttpVersion(start + i + 1, message, lexemes))
            {
                return (FALSE);
            }
            return (TRUE);
        }
    }
    return (FALSE);
}

t_bool  isPath(int start, t_string message, t_http_lexemes lexemes)
{
    return (isRelativePath(start, message, lexemes) || isAbsolutePath(start, message, lexemes));
}

t_bool  isBody(int start, t_string message, t_http_lexemes lexemes)
{
    int i;

    i = 0;
    while (message[start + i])
    {
        i++;
    }
    insert_node(lexemes, create_node(createHttpLexeme(BODY, substring(message, start, start + i))));
    return (TRUE);
}

t_bool  isHeaderKey(int start, t_string message, t_http_lexemes lexemes)
{
    int i;
    int flag;

    flag = 1;
    i = 0;
    while (flag && message[start] && message[start + i] && message[start + i] != ':')
    {
        if (!valid_field_key(message[start + i]))
        {
            flag = 0;
        }
        i++;
    }
    if (i > 0 && flag)
    {
        insert_node(lexemes, create_node(createHttpLexeme(HEADER_KEY, substring(message, start, start + i))));
        if (isHeaderValue(start + i + 1, message, lexemes))
        {
            return (TRUE);
        }
    }
    return (FALSE);
}

t_bool  isHeaderValue(int start, t_string message, t_http_lexemes lexemes)
{
    int i;

    i = 0;
    if (message[start] == ' ')
    {
        start++;
    }
    else
    {
        if (isHeader(start + 2, message, lexemes))
        {
            return (TRUE);
        }
    }
    while (message[start] && my_isalpha_special(message[start + i]) && !matchCRLF(message + start + i))
    {
        i++;
    }
    if (i > 0)
    {
        insert_node(lexemes, create_node(createHttpLexeme(HEADER_VALUE, substring(message, start, start + i))));
        if (message[start + i] == '\n' && !message[start + i + 1])
        {
            return (TRUE);
        }
        if (message[start + i] == 0)
        {
            return (TRUE);
        }
        else if (isHeader(start + i + 2, message, lexemes))
        {
            return (TRUE);
        }
    }
    else
    {
        if (isHeader(start + 2 + i, message, lexemes))
        {
            return (TRUE);
        }
    }
    return (isCRLF(start + 2 + i, message, lexemes));
}

t_bool  isHeader(int start, t_string message, t_http_lexemes lexemes)
{
    return (isHeaderKey(start, message, lexemes));
}

t_bool  isCRLF(int start, t_string message, t_http_lexemes lexemes)
{
    if (matchCRLF(message + start))
    {
        return (isBody(start + 2, message, lexemes));
    }
    return (FALSE);
}

t_bool  isHeaderList(int start, t_string s, t_http_lexemes lexemes)
{
    return (isHeader(start, s, lexemes) || isCRLF(start, s, lexemes));
}

t_request   *lexeme_to_request(t_http_lexemes lexemes)
{
    t_request       *request;
    t_http_lexeme   *lexeme;
    t_node         *begin;
    char            *tmp;

    if ((request = (t_request *)malloc(sizeof(*request))) == NULL)
    {
        return (NULL);
    }
    if ((request->headers = new_list()) == NULL)
    {
        free(request);
        return (NULL);
    }
    tmp = NULL;
    begin = (t_node *)lexemes->head;
    while (lexemes->head)
    {
        lexeme = (t_http_lexeme *)lexemes->head->data;
        if (lexeme->item_type == HTTP_VERSION)
        {
            request->http_version = strdup(lexeme->item);
        }
        else if (lexeme->item_type == PATH)
        {
            request->path = strdup(lexeme->item);
        }
        else if (lexeme->item_type == METHOD)
        {
            request->method = strdup(lexeme->item);
        }
        else if (lexeme->item_type == BODY)
        {
            request->body = strdup(lexeme->item);
        }
        if (lexeme->item_type == HEADER_VALUE)
        {
            tmp = strdup(lexeme->item);
            lexemes->head = lexemes->head->next;
            lexeme = (t_http_lexeme *)lexemes->head->data;
            insert_node(request->headers, create_node(create_header(strdup(lexeme->item), tmp)));
        }
        else if (lexeme->item_type == HEADER_KEY)
        {
           insert_node(request->headers, create_node(create_header(strdup(lexeme->item), strdup(""))));
        }
        lexemes->head = lexemes->head->next;
    }
    lexemes->head = (t_node *)begin;
    delete_list_exec_action(lexemes, &delete_http_lexeme);
    return (request);
}

t_bool  matchCRLF(t_string s)
{
    return (match_str(CRLF, s, 0) > 0);
}
