#include		<stdio.h>

#include		<game_ctx.h>
#include		<gmanager.h>
#include		<args.h>
#include		<network.h>

int			main(int argc, char **argv)
{
	game_instance_t	*manager;

	parse_opts(argc, argv);
	manager = get_instance();
	serve_game();
	clean_sockets(manager->socks);
	manager->socks = 0x0;
	dtor_list(&manager->energy_cells);
	manager->energy_cells = 0x0,
	dtor_manager(manager);
	return (0);
}
