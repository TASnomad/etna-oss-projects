#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <criterion/criterion.h>
#include "syntax.h"
#include "http.h"

Test(sample, test_1) {
    cr_expect(strlen("Test") == 4, "KO");
}

Test(sample, test_2) {
    t_string str = "GET /test HTTP/1.1\r\n\
Host: localhost\r\n\
Connection: keep-alive\r\n\
Upgrade-Insecure-Requests: 1\r\n\
User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_14_4) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/80.0.3987.116 Safari/537.36\r\n\
Sec-Fetch-Dest: document\r\n\
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9\r\n\
Sec-Fetch-Site: none\r\n\
Sec-Fetch-Mode: navigate\r\n\
Sec-Fetch-User: ?1\r\n\
Accept-Encoding: gzip, deflate, br\r\n\
Accept-Language: fr-FR,fr;q=0.9,en-US;q=0.8,en;q=0.7\r\n\
Cookie: PLAY_SESSION=eyJhbGciOiJIUzI1NiJ9.eyJkYXRhIjp7ImNzcmZUb2tlbiI6IjRhZTUyYmQyNzA1YzY4MDkzMTg0YWFiNzJhOWQzMDcyN2QzOTQ4NjItMTU3ODQ5NjcxOTE0NS1hNjFlNmI1Zjg5Njc3M2ZkNWU5MjY5MzcifSwibmJmIjoxNTc4NDk2NzE5LCJpYXQiOjE1Nzg0OTY3MTl9.WQVGQ9dwUWH0p6-sKoul34Lufz_htZbZvuX1-AviJKY; __ssid_optr=ODQ9QV8AWkAXVghXSwVDcQIPQxRUT2kUTFAGDhFXXAtEBgJOFk1GXEEXRRE2GwkDFhUDFAQQZ0tCBUtCUkMDFAEBPwJXW0AACQZGH0NAZ11EWwMbBUJO; JSESSIONID=E44EDC1BA1AE914D49318A22C31E6A37; _ga=GA1.1.103857332.1580577669; googlepersonalization=OuZp8YOuZp8YgA; eupubconsent=BOuZp8YOuZp8YAKAAAENAAAAiAAAAA; euconsent=BOuZp8YOuZp8YAKAABENC7-AAAAt1r_7__7-_9_-_d__9uj3Or_v_f__32ccL59v_h_7v-_7fi_20nV4u_1vft9yfk1-5ctDztp505iakivHmqNeb9n9mz1e5pRP78k89r7337Ew_v8_v-b7BCON_YxA\r\n\
\r\n";
    t_http_lexemes  lexemes = parse_request(str);
    t_request *request = NULL;

    cr_expect(lexemes != NULL, "The request is invalid");
    request = lexeme_to_request(lexemes);
    cr_expect(request != NULL, "Error request");
    cr_expect(strcmp(request->http_version, "1.1") == 0, "Http version invalid");
    cr_expect(strcmp(request->method, "GET") == 0, "Method invalid");
    cr_expect(strcmp(request->path, "/test") == 0, "Path invalid");
    t_header *header1 = find_header(request->headers, "Host");

    cr_expect(strcmp(header1->key, "Host") == 0, "Header invalid");
    cr_expect(strcmp(header1->value, "localhost") == 0, "Header invalid");

   t_header *header2 = find_header(request->headers, "Connection");

    cr_expect(strcmp(header2->key, "Connection") == 0, "Header invalid");
    cr_expect(strcmp(header2->value, "keep-alive") == 0, "Header invalid");

    t_header *header3 = find_header(request->headers, "Upgrade-Insecure-Requests");
    cr_expect(strcmp(header3->key, "Upgrade-Insecure-Requests") == 0, "Header invalid");
    cr_expect(strcmp(header3->value, "1") == 0, "Header invalid");

    t_header *header4 = find_header(request->headers, "User-Agent");
    cr_expect(strcmp(header4->key, "User-Agent") == 0, "Header invalid");
    cr_expect(strcmp(header4->value, "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_14_4) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/80.0.3987.116 Safari/537.36") == 0, "Header invalid");

    t_header *header5 = find_header(request->headers, "Sec-Fetch-Dest");
    cr_expect(strcmp(header5->key, "Sec-Fetch-Dest") == 0, "Header invalid");
    cr_expect(strcmp(header5->value, "document") == 0, "Header invalid");

    t_header *header6 = find_header(request->headers, "Accept");
    cr_expect(strcmp(header6->key, "Accept") == 0, "Header invalid");
    cr_expect(strcmp(header6->value, "text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9") == 0, "Header invalid");

    t_header *header7 = find_header(request->headers, "Sec-Fetch-Site");
    cr_expect(strcmp(header7->key, "Sec-Fetch-Site") == 0, "Header invalid");
    cr_expect(strcmp(header7->value, "none") == 0, "Header invalid");

    t_header *header8 = find_header(request->headers, "Sec-Fetch-Mode");
    cr_expect(strcmp(header8->key, "Sec-Fetch-Mode") == 0, "Header invalid");
    cr_expect(strcmp(header8->value, "navigate") == 0, "Header invalid");

    t_header *header9 = find_header(request->headers, "Sec-Fetch-User");
    cr_expect(strcmp(header9->key, "Sec-Fetch-User") == 0, "Header invalid");
    cr_expect(strcmp(header9->value, "?1") == 0, "Header invalid");

    t_header *header10 = find_header(request->headers, "Accept-Encoding");
    cr_expect(strcmp(header10->key, "Accept-Encoding") == 0, "Header invalid");
    cr_expect(strcmp(header10->value, "gzip, deflate, br") == 0, "Header invalid");

    t_header *header11 = find_header(request->headers, "Accept-Language");
    cr_expect(strcmp(header11->key, "Accept-Language") == 0, "Header invalid");
    cr_expect(strcmp(header11->value, "fr-FR,fr;q=0.9,en-US;q=0.8,en;q=0.7") == 0, "Header invalid");

    t_header *header12 = find_header(request->headers, "Cookie");
    cr_expect(strcmp(header12->key, "Cookie") == 0, "Header invalid");
    cr_expect(strcmp(header12->value, "PLAY_SESSION=eyJhbGciOiJIUzI1NiJ9.eyJkYXRhIjp7ImNzcmZUb2tlbiI6IjRhZTUyYmQyNzA1YzY4MDkzMTg0YWFiNzJhOWQzMDcyN2QzOTQ4NjItMTU3ODQ5NjcxOTE0NS1hNjFlNmI1Zjg5Njc3M2ZkNWU5MjY5MzcifSwibmJmIjoxNTc4NDk2NzE5LCJpYXQiOjE1Nzg0OTY3MTl9.WQVGQ9dwUWH0p6-sKoul34Lufz_htZbZvuX1-AviJKY; __ssid_optr=ODQ9QV8AWkAXVghXSwVDcQIPQxRUT2kUTFAGDhFXXAtEBgJOFk1GXEEXRRE2GwkDFhUDFAQQZ0tCBUtCUkMDFAEBPwJXW0AACQZGH0NAZ11EWwMbBUJO; JSESSIONID=E44EDC1BA1AE914D49318A22C31E6A37; _ga=GA1.1.103857332.1580577669; googlepersonalization=OuZp8YOuZp8YgA; eupubconsent=BOuZp8YOuZp8YAKAAAENAAAAiAAAAA; euconsent=BOuZp8YOuZp8YAKAABENC7-AAAAt1r_7__7-_9_-_d__9uj3Or_v_f__32ccL59v_h_7v-_7fi_20nV4u_1vft9yfk1-5ctDztp505iakivHmqNeb9n9mz1e5pRP78k89r7337Ew_v8_v-b7BCON_YxA") == 0, "Header invalid");
    
    delete_list_exec_action(lexemes, &remove_lexeme);
    delete_request(request);
}

Test(sample, test_3) {
    t_string str = "POST /test HTTP/1.1\r\n\
Content-Type: application/json\r\n\
User-Agent: PostmanRuntime/7.22.0\r\n\
Accept: */*\r\n\
Cache-Control: no-cache\r\n\
Host: localhost\r\n\
Accept-Encoding: gzip, deflate, br\r\n\
Content-Length: 29\r\n\
Connection: keep-alive\r\n\
\r\n\
{\n\
        \"message\": \"Hello World\"\n\
}\n";
    t_http_lexemes  lexemes = parse_request(str);
    t_request *request = NULL;

    cr_expect(lexemes != NULL, "The request is invalid");
    request = lexeme_to_request(lexemes);
    cr_expect(request != NULL, "Error request");
    cr_expect(strcmp(request->http_version, "1.1") == 0, "Http version invalid");
    cr_expect(strcmp(request->method, "POST") == 0, "Method invalid");
    cr_expect(strcmp(request->path, "/test") == 0, "Path invalid");

    t_header *header1 = find_header(request->headers, "Content-Type");
    cr_expect(strcmp(header1->key, "Content-Type") == 0, "Header invalid");
    cr_expect(strcmp(header1->value, "application/json") == 0, "Header invalid");

    t_header *header2 = find_header(request->headers, "User-Agent");
    cr_expect(strcmp(header2->key, "User-Agent") == 0, "Header invalid");
    cr_expect(strcmp(header2->value, "PostmanRuntime/7.22.0") == 0, "Header invalid");

    t_header *header3 = find_header(request->headers, "Accept");
    cr_expect(strcmp(header3->key, "Accept") == 0, "Header invalid");
    cr_expect(strcmp(header3->value, "*/*") == 0, "Header invalid");

    t_header *header4 = find_header(request->headers, "Cache-Control");
    cr_expect(strcmp(header4->key, "Cache-Control") == 0, "Header invalid");
    cr_expect(strcmp(header4->value, "no-cache") == 0, "Header invalid");

    t_header *header5 = find_header(request->headers, "Host");
    cr_expect(strcmp(header5->key, "Host") == 0, "Header invalid");
    cr_expect(strcmp(header5->value, "localhost") == 0, "Header invalid");

    t_header *header6 = find_header(request->headers, "Accept-Encoding");
    cr_expect(strcmp(header6->key, "Accept-Encoding") == 0, "Header invalid");
    cr_expect(strcmp(header6->value, "gzip, deflate, br") == 0, "Header invalid");

    t_header *header7 = find_header(request->headers, "Content-Length");
    cr_expect(strcmp(header7->key, "Content-Length") == 0, "Header invalid");
    cr_expect(strcmp(header7->value, "29") == 0, "Header invalid");

    t_header *header8 = find_header(request->headers, "Connection");
    cr_expect(strcmp(header8->key, "Connection") == 0, "Header invalid");
    cr_expect(strcmp(header8->value, "keep-alive") == 0, "Header invalid");
    cr_expect(strcmp(request->body, "{\n\
        \"message\": \"Hello World\"\n\
}\n") == 0, "Body invalid");
    delete_list_exec_action(lexemes, &remove_lexeme);
    delete_request(request);
}

Test(sample, test_4) {
    t_string str = "GET /hello.html HTTP/1.1\r\n\
User-Agent: Mozilla/4.0 (compatible; MSIE5.01; Windows NT)\r\n\
Host: www.tutorialspoint.com\r\n\
Accept-Languagjkjke: en-usjjkl\r\n\
Accept-Encoding: gzip, deflate\r\n\
Test: \r\n\
Connection: Keep-Alive\r\n\
Salut:          \r\n\
Allow: GET\r\n\
Test2: \r\n\
Test3:\r\n\
X-Powered-By: Express\r\n\
\r\n";
    t_http_lexemes  lexemes = parse_request(str);
    t_request *request = NULL;

    cr_expect(lexemes != NULL, "The request is invalid");
    request = lexeme_to_request(lexemes);
    cr_expect(request != NULL, "Error request");
    cr_expect(strcmp(request->http_version, "1.1") == 0, "Http version invalid");
    cr_expect(strcmp(request->method, "GET") == 0, "Method invalid");
    cr_expect(strcmp(request->path, "/hello.html") == 0, "Path invalid");

    t_header *header1 = find_header(request->headers, "User-Agent");
    cr_expect(strcmp(header1->key, "User-Agent") == 0, "Header invalid");
    cr_expect(strcmp(header1->value, "Mozilla/4.0 (compatible; MSIE5.01; Windows NT)") == 0, "Header invalid");

    t_header *header2 = find_header(request->headers, "Host");
    cr_expect(strcmp(header2->key, "Host") == 0, "Header invalid");
    cr_expect(strcmp(header2->value, "www.tutorialspoint.com") == 0, "Header invalid");

    t_header *header3 = find_header(request->headers, "Accept-Languagjkjke");
    cr_expect(strcmp(header3->key, "Accept-Languagjkjke") == 0, "Header invalid");
    cr_expect(strcmp(header3->value, "en-usjjkl") == 0, "Header invalid");

    t_header *header4 = find_header(request->headers, "Accept-Encoding");
    cr_expect(strcmp(header4->key, "Accept-Encoding") == 0, "Header invalid");
    cr_expect(strcmp(header4->value, "gzip, deflate") == 0, "Header invalid");

    t_header *header5 = find_header(request->headers, "Test");
    cr_expect(strcmp(header5->key, "Test") == 0, "Header invalid");
    cr_expect(strlen(header5->value) == 0, "Header invalid");

    t_header *header6 = find_header(request->headers, "Connection");
    cr_expect(strcmp(header6->key, "Connection") == 0, "Header invalid");
    cr_expect(strcmp(header6->value, "Keep-Alive") == 0, "Header invalid");

    t_header *header7 = find_header(request->headers, "Salut");
    cr_expect(strcmp(header7->key, "Salut") == 0, "Header invalid");
    cr_expect(strcmp(header7->value, "         ") == 0, "Header invalid");

    t_header *header8 = find_header(request->headers, "Allow");
    cr_expect(strcmp(header8->key, "Allow") == 0, "Header invalid");
    cr_expect(strcmp(header8->value, "GET") == 0, "Header invalid");

    t_header *header9 = find_header(request->headers, "Test2");
    cr_expect(strcmp(header9->key, "Test2") == 0, "Header invalid");
    cr_expect(strlen(header9->value) == 0, "Header invalid");

    t_header *header10 = find_header(request->headers, "Test3");
    cr_expect(strcmp(header10->key, "Test3") == 0, "Header invalid");
    cr_expect(strlen(header10->value) == 0, "Header invalid");

    t_header *header11 = find_header(request->headers, "X-Powered-By");
    cr_expect(strcmp(header11->key, "X-Powered-By") == 0, "Header invalid");
    cr_expect(strcmp(header11->value, "Express") == 0, "Header invalid");
}

Test(sample, test_5){
    t_string str = "GET  /hello.html HTTP/1.1\r\n\
User-Agent: Mozilla/4.0 (compatible; MSIE5.01; Windows NT)\r\n\
Host: www.tutorialspoint.com\r\n\
Accept-Languagjkjke:    en-usjjkl\r\n\
Accept-Encoding: gzip, deflate\r\n\
Test: \r\n\
Connection: Keep-Alive\r\n\
Salut:          \r\n\
Allow: GET\r\n\
Test2: \r\n\
X-Powered-By: Express\r\n\
\r\n";
    t_http_lexemes  lexemes = parse_request(str);

    cr_expect(lexemes == NULL, "The request is valid");
}

Test(sample, test_6){
    t_string str = "GET /hello.html HTTP/1.\r\n\
User-Agent: Mozilla/4.0 (compatible; MSIE5.01; Windows NT)\r\n\
Host: www.tutorialspoint.com\r\n\
Accept-Languagjkjke:    en-usjjkl\r\n\
Accept-Encoding: gzip, deflate\r\n\
Test: \r\n\
Connection: Keep-Alive\r\n\
Salut:          \r\n\
Allow: GET\r\n\
Test2: \
X-Powered-By: Express\r\n\
\r\n";
    t_http_lexemes  lexemes = parse_request(str);

    cr_expect(lexemes == NULL, "The request is valid");
}

Test(sample, test_7){
    t_string str = "GET  /hello.html HTTP/.1\r\n\
User-Agent: Mozilla/4.0 (compatible; MSIE5.01; Windows NT)\r\n\
Host: www.tutorialspoint.com\r\n\
Accept-Languagjkjke:    en-usjjkl\r\n\
Accept-Encoding: gzip, deflate\r\n\
Test: \r\n\
Connection: Keep-Alive\r\n\
Salut:          \r\n\
Allow: GET\r\n\
Test2: \r\n\
X-Powered-By: Express\r\n\
\r\n";
    t_http_lexemes  lexemes = parse_request(str);

    cr_expect(lexemes == NULL, "The request is valid");
}

Test(sample, test_8){
    t_string str = "GET /hello.html HTTP/1.1\r\n\
User-Agent: Mozilla/4.0 (compatible; MSIE5.01; Windows NT)\r\n\
Host: www.tutorialspoint.com\r\n\
Accept-Languagjkjke:    en-usjjkl\r\n\
Accept-Encoding: gzip, deflate\r\n\
Test: \r\n\
Connection: Keep-Alive\r\n\
Salut:          \r\n\
Allow: GET\r\n\
Test2: \r\n\
X-Powered-By: Express\r\n\
\r\n";
    t_http_lexemes  lexemes = parse_request(str);

    cr_expect(lexemes != NULL, "The request is invalid");
}

Test(sample, test_9){
        t_string str = "GET /hello.html HTTP/1.1\r\n\
\r\n";
        t_http_lexemes lexemes = parse_request(str);

        cr_expect(lexemes != NULL, "The request is invalid");
}