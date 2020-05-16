#include "cherokee.h"

void 	delete_data(t_node *node)
{
	free(node->data);
	free(node);
}

int main()
{
	cherokee_t	*server;

	server = cherokee_create_instance(PORT);
	if (!server)
		return (EXIT_FAILURE);
	cherokee_run(server);
	delete_cherokee_instance(server);
	return (EXIT_SUCCESS);
}
