#include			<enums.h>
#include			<notif.h>
#include			<gmanager.h>
#include			<game_ctx.h>

#include			<json-c/json.h>

void				notify_game_started()
{
	game_instance_t		*manager;
	struct json_object	*notif;
	NotificationType_t	notif_to_send;
	char			*json_str;

	notif_to_send = GAME_START;
	notif = json_object_new_object();
	json_object_object_add(notif, "notification_type", json_object_new_int(notif_to_send));
	json_object_object_add(notif, "data", 0x0);
	manager = get_instance();
	json_str = (char *) json_object_to_json_string(notif);
	zsock_send(manager->socks->pub_sock, "ss", GLOBAL_TOPIC, json_str);
	json_object_put(notif); // Free JSON structure
}

void				send_notif(NotificationType_t type, char *topic, struct json_object *msg)
{
	game_instance_t		*manager;
	struct json_object	*notif;

	manager = get_instance();
	notif = json_object_new_object();
	json_object_object_add(notif, "notification_type", json_object_new_int(type));
	json_object_object_add(notif, "data", msg);
	zsock_send(manager->socks->pub_sock, "ss", topic, json_object_to_json_string(notif));
	json_object_put(notif); // Free JSON structure
}

