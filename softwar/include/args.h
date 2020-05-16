#ifndef __ARGS_H__
#define __ARGS_H__

// Functions & constants definitions used to parse cmdline arguments

#include	<getopt.h>
#include 	<default.h>
#include	<gmanager.h>

void		prepare_log(game_instance_t *, char *);
void		usage();
int		parse_opts(int, char **);

extern const char	*short_options;
extern struct option	long_options[];

#endif /* ! __ARGS_H__ */
