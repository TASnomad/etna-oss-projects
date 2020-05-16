#ifndef __NOTIF_H__
#define __NOTIF_H__

// Definition of all macros & functions used to interact with the ZMQ PUB socket

#include		<enums.h>
#include		<json-c/json.h>

#define	GLOBAL_TOPIC	"Global"

void			notify_game_started();
void			send_notif(NotificationType_t , char *, struct json_object *);

#endif /* !__NOTIF_H__ */
