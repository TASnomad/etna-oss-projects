#include "my_string.h"

int     match_str(t_string s1, t_string s2, int index)
{
    int i;

    i = 0;
    while (s1[i])
    {
        if (s1[i] != s2[i + index])
        {
            return (-1);
        }
        i++;
    }
    return (i);
}

t_string    substring(t_string s, int begin, int end)
{
    t_string    ret;
    int         i;

    i = 0;
    if ((ret = (t_string)malloc(sizeof(*ret) * (end - begin + 1))) == NULL)
    {
        perror("substring:");
        return (NULL);
    }
    while (i < (end - begin))
    {
        ret[i] = s[begin + i];
        i++;
    }
    ret[i] = 0;
    return (ret);
}

int         append_alloc_string(t_string s1, t_string s2, int start)
{
    int     i;

    i = 0;
    while (s2[i])
    {
        s1[i + start] = s2[i];
        i++;
    }
    return (start + i);
}

t_string    number_to_string(int a)
{
    t_string    ret;
    int         div;
    int         i;
    int         size;

    i = size = 0;
    div = 1;
    while ((a / div) > 9)
    {
        div *= 10;
        size++;
    }
    if ((ret = (t_string)malloc(sizeof(*ret) * (size + 1))) == NULL)
    {
        return (NULL);
    }
    while (i <= size)
    {
        ret[i] = (a / div) + 48;
        a = a % div;
        div = div / 10;
        i++;    
    }
    ret[i] = 0;
    return (ret);
}
