#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <criterion/criterion.h>
#include "response.h"
#include "http.h"

Test(sample, test1){
    cr_expect(strlen("Test") == 4, "KO");
}

Test(sample, test2){
    t_response   *response;
    t_string     response_str = "HTTP/1.1 200 OK\r\n\
Connection: Keep-Alive\r\n\
Content-Type: text/plain\r\n\
\r\n\
Hello World";

    response = (t_response *)malloc(sizeof(*response));
    if (!response){
        return;
    }
    response->http_version = strdup("1.1");
    response->status = 200;
    response->reason_phrase = defaultStatusText(response->status);
    response->body = strdup("Hello World");
    if ((response->headers = new_list()) == NULL)
    {
        return;
    }
    insert_node(response->headers, create_node(create_header(strdup("Content-Type"), strdup("text/plain"))));
    insert_node(response->headers, create_node(create_header(strdup("Connection"), strdup("Keep-Alive"))));
    t_string str = toResponseString(response);
    cr_expect(strcmp(response_str, str) == 0, "The response isn't correct");
    delete_response(response);
}

Test(sample, test3){
      t_string  response_str = "HTTP/1.1 404 Not Found\r\n\
Accept-Ranges: bytes\r\n\
Age: 3676\r\n\
Cache-Control: max-age=141982\r\n\
Content-Type: text/plain\r\n\
Date: Fri, 28 Feb 2020 12:22:27 GMT\r\n\
Etag: \"5e587f45-139\"\r\n\
Expires: Sun, 01 Mar 2020 03:48:49 GMT\r\n\
Last-Modified: Fri, 28 Feb 2020 02:47:33 GMT\r\n\
Server: ECS (pab/6F81)\r\n\
\r\n";
    t_response   *response;

    response = (t_response *)malloc(sizeof(*response));
    if (!response){
        return;
    }
    response->http_version = strdup("1.1");
    response->status = 404;
    response->reason_phrase = defaultStatusText(response->status);
    response->body = NULL;
    if ((response->headers = new_list()) == NULL)
    {
        return;
    }
    insert_node(response->headers, create_node(create_header(strdup("Server"), strdup("ECS (pab/6F81)"))));
    insert_node(response->headers, create_node(create_header(strdup("Last-Modified"), strdup("Fri, 28 Feb 2020 02:47:33 GMT"))));
    insert_node(response->headers, create_node(create_header(strdup("Expires"), strdup("Sun, 01 Mar 2020 03:48:49 GMT"))));
    insert_node(response->headers, create_node(create_header(strdup("Etag"), strdup("\"5e587f45-139\""))));
    insert_node(response->headers, create_node(create_header(strdup("Date"), strdup("Fri, 28 Feb 2020 12:22:27 GMT"))));
    insert_node(response->headers, create_node(create_header(strdup("Content-Type"), strdup("text/plain"))));
    insert_node(response->headers, create_node(create_header(strdup("Cache-Control"), strdup("max-age=141982"))));
    insert_node(response->headers, create_node(create_header(strdup("Age"), strdup("3676"))));
    insert_node(response->headers, create_node(create_header(strdup("Accept-Ranges"), strdup("bytes"))));
    t_string str = toResponseString(response);
    cr_expect(strcmp(response_str, str) == 0, "The response isn't correct");
    delete_response(response);
}