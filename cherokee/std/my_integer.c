#include "my_integer.h"

t_integer   *create_int(int n)
{
    t_integer   *integer;

    if ((integer = (t_integer *)malloc(sizeof(*integer))) == NULL)
    {
        perror("integer");
        printf("failed to alloc for integer\n");
        return (NULL);
    }
    integer->value = n;
    return (integer);
}