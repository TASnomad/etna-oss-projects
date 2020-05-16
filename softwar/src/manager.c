#include	<stdlib.h>
#include	<stdio.h>
#include	<stdarg.h>

#include	<list.h>
#include	<gmanager.h>


t_list			*init_players_list()
{
	t_list	*l;

	l = 0x0;
	l = ctor_list();
	return (l);
}

game_instance_t		*get_instance()
{
	static game_instance_t	*instance;

	if (!instance)
	{
		if (!(instance = malloc(sizeof(*instance))))
		{
			fprintf(stderr, "Can't malloc manager in: %s\n", __func__);
			return (0x0);
		}
		instance->logging_level = 0;
		instance->ready = 0;
		instance->players = init_players_list();
		instance->energy_cells = ctor_list();
	}
	return (instance);
}

int			my_logger(int level, char *fmt, ...)
{
	game_instance_t	*m;
	va_list		args;

	(void) level;
	va_start(args, fmt);
	m = get_instance();
	if ((m->logging_level == 0) && (level == 1)) {
		return (0);
	}
	return (vfprintf(m->log_stream, fmt, args));
}

void			dtor_manager(game_instance_t *instance)
{
	if (instance->socks)
		clean_sockets(instance->socks);
	if (instance->players)
		dtor_list(&instance->players);
	instance->players = 0x0;
	instance->socks = 0x0;
	fclose(instance->log_stream);
	free(instance);
	instance = 0x0;
}

