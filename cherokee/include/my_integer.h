#ifndef _INTEGER_HH_
# define _INTEGER_HH_

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "my_bool.h"

typedef struct s_integer {
    int value;
}              t_integer;

t_integer   *create_int(int);

#endif
