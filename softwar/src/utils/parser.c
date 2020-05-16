#include		<command.h>	// RFC parsing command header

#include		<string.h>	// strncmp, strdup
#include		<stdlib.h>	// malloc, free, strtok
#include		<json-c/json.h>


char			*arr_to_json_string(char **arr)
{
	char		*res;
	json_object	*tmp;
	int		i;

	i = 0;
	tmp = 0x0;
	res = 0x0;
	if (!arr)
		return (0x0);
	tmp = json_object_new_array();
	while (i < 4)
	{
		json_object_array_add(tmp, (!*(arr + i)) ? json_object_new_string(strdup("")) : json_object_new_string(strdup(*(arr + i))));
		i++;
	}
	res = strdup(json_object_to_json_string(tmp));
	json_object_put(tmp);
	return (res);
}

char			**split(const char *str, char del)
{
	char		**ret;
	char		*tmp;
	char		*s;
	char		*token;
	int		occurs;
	int		i;

	occurs = i = 0;
	token = tmp = 0x0;
	s = strdup(str);
	tmp = (char *) str;
	while (*tmp)
	{
		if (*tmp == del)
			occurs++;
		tmp++;
	}
	ret = (char **) malloc((occurs + 1) * sizeof(char *));
	if (ret)
	{
		token = strtok(s, &del);
		while (token)
		{
			*(ret + i++) = strdup(token);
			token = strtok(0x0, &del);
		}
		*(ret + i) = 0x0;
	}
	return (ret);
}
