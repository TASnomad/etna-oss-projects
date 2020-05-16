#ifndef _STRING_HH_
# define _STRING_HH_

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "my_bool.h"

typedef char* t_string;

int         match_str(t_string, t_string, int);
t_string    substring(t_string, int, int);

int         append_alloc_string(t_string, t_string, int start);

void        delete_string(t_string *);

t_string    number_to_string(int);

#endif
