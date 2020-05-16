#ifndef	__COMMAND_H__
#define	__COMMAND_H__

// RFC utilities defintions
// Macros are represeting various part of messages send or received by the server
//
// NOTE: commands handlers are defined in the file src/command.c

#include <player.h>

#define	CMD_FAILURE	"ko"		// Statement used to represent a failure while executing a command
					// can be used if the command isn't handled by the server OR if the command can(t be executed right now

#define	CMD_SUCCESS	"ok"		// Statement used to represent a successfull execution of a command

#define	IDENTITY_CMD	"identify"	// Login command
#define	SELFID_CMD	"selfid"	// Command to get the gamer ID
#define	SELFSTATS_CMD	"selfstats"	// Command to get the gamer energy amount
#define	INSPECT_CMD	"inspect"	// Command used to get energy amount to a specific player
#define	WATCH_CMD	"watch"
#define	LOOKING_CMD	"looking"	// Command used to return the current looking direction
#define	RIGHT_CMD	"right"		// Move the current looking direction to the right (anticlockwise)
#define	LEFT_CMD	"left"		// Move the current looking direction to the left (clockwise)
#define FORWARD_CMD	"forward"
#define	BACKWARD_CMD	"backward"
#define	LEFTFWD_CMD	"leftfwd"
#define	RIGHTFWD_CMD	"rightfwd"
#define	NEXT_CMD	"next"
#define	JUMP_CMD	"jump"
#define	GATHER_CMD	"gather"
#define	ATTACK_CMD	"attack"

#define CMD_SEP		'|'		// RFC separator

#define	ISE_ERR		"ISE"		// Internal Server Error (unhandled behviour)
#define	ID_TAKEN	"identify already exists"	// Preventing duplicated gamer ID
#define	GAME_FULL	"game full"	// 4 players are already logged

#define	NO_RET		"null"		// No data to return
#define	NO_CMD		"command not found"

#define	COST_ERR	"no more energy"
#define	LOW_ENERGY	"low energy to jump"

#define	HALF_ACT_COST	5		// Equivalent to 0.5 for the RFC
#define	FULL_ACT_COST	10		// Equivalent to 1 for the RFC

/**
 * Utilities function used to check if a given ID is already in the players list
 * params:
 * 1- Players list
 * 2- Gamer ID
 *
 * return:
 * pointer to the existing pplayer otherwise a null pointer
 */
player_t		*is_id_taken(t_list *, char *);

// ---- RFC commands handlers -----
// Those handlers takes 2 params (most of the time)
// 1 - splitted RFC commands
// 2 - Player ID (from zmq socket ID)

// identify command handler
char			*identify(char **);
// selfid command handler
char			*selfid(char **, char *);
// selfstats command handler
char			*selfstats(char **, char *);
// inspect command handler
char			*inspect(char **, char *);
// watch command handler
char			*watch(char **, char *);
char			*looking(char **, char *);
char			*left(char **, char *);
char 			*right(char **, char *);
char			*forward(char **, char *);
char			*backward(char **, char *);
char			*leftfwd(char **, char *);
char			*rightfwd(char **, char *);
char			*next(char **, char *);
char			*jump(char **, char *);
char			*gather(char **, char *);
char			*attack(char **, char *);

// Non standard command used to return all commands handled by the server
char			*commands_to_string();

// ---- End of RFC handlers -----

// Utilities function (equivalent function of string split)
char			**split(const char *, char);
char			*arr_to_json_string(char **);

typedef struct		s_command
{
	char		*name;
	char		*(*cmd)();
} 			t_command;

typedef t_command 	command_t;

/**
 * params:
 * 1- message received from zmq socket
 * 2- Player ID found in the zmq socket
 *
 * return:
 *  string ready to be send
 */
char			*exec_command(char *, char *);

#endif	/* ! __COMMAND_H__ */
