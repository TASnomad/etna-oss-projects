#include	<stdio.h>
#include	<stdlib.h>
#include	<getopt.h>

#include	<args.h>
#include	<default.h>
#include	<gmanager.h>

const char 	*short_options = "hvr:p:c:s:l:";

struct option	long_options[] =
{
	{ "help", no_argument, 0x0, 'h' },
	{ "rep-port", required_argument, 0x0, 'r' },
	{ "pub-port", required_argument, 0x0, 'p' },
	{ "cycle", required_argument, 0x0, 'c' },
	{ "size", required_argument, 0x0, 's' },
	{ "log", required_argument, 0x0, 'l' },
	//{ "verbose", no_argument, 0x0, 'v' },
	{ 0x0, 0x0, 0x0, 0x0 }
};

void		usage()
{
	fprintf(stderr, "Usage: ./server [--cycle duration] [--rep-port xxx] [--pub-port wxy] [--size xyz] [-h | --help] [--log path]\n\n");
	fprintf(stderr, "\tcycle : game cycle duration (duration smaller than 500000 ms are ignored)\n");
	fprintf(stderr, "\trep-port : port used to listen all players request (default port : 4242)\n");
	fprintf(stderr, "\tpub-port : port used to send notifications to players (default port : 4243)\n");
	fprintf(stderr, "\tsize : map size (default & minimal value is 5)\n");
	fprintf(stderr, "\thelp : print this help message\n");
	fprintf(stderr, "\tlog : file used to store all logs from an execution (default file : stdout)\n\n");
	exit(0);
}

void		prepare_log(game_instance_t *m, char *filename)
{
	FILE	*stream;
	if (!filename) {
		stream = stdout;
		fprintf(stream, "No filename found for --log option. Logging in stdout\n");
	} else {
		stream = fopen(filename, "w");
		stream = (!stream) ? stdout : stream; // If a failure occured we use stdout
	}
	m->log_stream = stream;
	my_logger(REGULAR_LOG, "Logger system starting up ... (%s)\n", __func__);
}

int		parse_opts(int argc, char **argv)
{
	game_instance_t *manager;
	int	pub_port;
	int	req_port;
	int	cycle_duration;
	int	map_size;
	int	c;

	manager = get_instance();
	pub_port = DEFAULT_PUB_PORT;
	req_port = DEFAULT_REQ_PORT;
	cycle_duration = DEFAULT_CYCLE_TIME;
	map_size = DEFAULT_MAP_SIZE;
	manager->log_stream = stdout;
	manager->logging_level = REGULAR_LOG;


	while ((c = getopt_long(argc, argv, short_options, long_options, 0x0)) != -1)
	{
		switch (c)
		{
			case 'h': usage(); break;
			case 'r':
				if (optarg)
					req_port = atoi(optarg);
				 break;
			case 'p':
				 if (optarg)
					 pub_port = atoi(optarg);
				 break;
			case 'c':
				 if (optarg) {
					 long tmp = atol(optarg);
					 if (tmp < DEFAULT_CYCLE_TIME) {
						 tmp = DEFAULT_CYCLE_TIME;
					 }
					 cycle_duration = atol(optarg);
				 }
				 break;
			case 's':
				 if (optarg) {
					 int msize;
					 msize = atoi(optarg);
					 if (msize > map_size) {
						 map_size = msize;
					 }
				 }
				 break;
			case 'l':
				 prepare_log(manager, optarg);
				 break;
			case 'v':
				 manager->logging_level = VERBOSE_LOG;
				 break;
		}
	}
	manager->map_size = map_size;
	manager->cycle_duration = cycle_duration;
	manager->socks = init_sockets(req_port, pub_port);
	return (1);
}
