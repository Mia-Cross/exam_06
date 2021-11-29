#include "mini_serv.h"

void exit_fatal(char c)
{
	write(2, &c, 1);
	write(2, ".Fatal error\n", 13);
	if (errno)
		perror(strerror(errno));
	exit(1);
}

char *put_prefix(char *msg, int author_id)
{
	char *buf = malloc(strlen(msg) + 20);
	if (!buf)
		exit_fatal('M');
	bzero(buf, strlen(msg) + 20);
	int last = sprintf(buf, "client %d: ", author_id);
	strcpy(buf + last, msg);
	free(msg);
	return (buf);
}

void client_action(t_client *cli, const char *action, t_client *list)
{
	char *buf = malloc(50);
	int last;

	if (!buf)
		exit_fatal('M');
	bzero(buf, 50);
	last = sprintf(buf, "server: client %d just ", cli->id);
	strcpy(buf + last, action);
	send_to_all(list, &buf, cli->socket);
}

t_client *add_client(t_client **list, int socket, int id)
{
	t_client *client = *list;
	t_client *new_client = malloc(sizeof(t_client));

	if (!new_client)
		exit_fatal('M');
	new_client->socket = socket;
	new_client->id = id;
	new_client->next = NULL;
	if (!client)
		*list = new_client;
	else
	{
		while (client && client->next)
			client = client->next;
		client->next = new_client;
	}
    write(1, "client++\n", 9);
	// fcntl(socket, F_SETFL, O_NONBLOCK);
	return (new_client);
}

void remove_client(t_client **list, int socket)
{
	if (!list)
		return;
	if ((*list)->socket == socket)
	{
		t_client *to_del = *list;
		*list = (*list)->next;
		close(to_del->socket);
		free(to_del);
		write(1, "client--\n", 9);
		return;
	}
	t_client *client = *list;

	while (client->next && client->next->socket != socket)
		;
	if (client->next && client->next->socket == socket)
	{
		t_client *to_del = client->next;
		client->next = client->next->next;
		close(to_del->socket);
		free(to_del);
		write(1, "client--\n", 9);
	}
}

t_client *get_client(t_client *list, int socket)
{
	while (list)
	{
		if (list->socket == socket)
			return (list);
		list = list->next;
	}
	return (NULL);
}


int extract_message(char **buf, char **msg)
{
	char	*newbuf;
	int	i;

	free(*msg);
	*msg = NULL;
	if (*buf == NULL)
		return (0);
	i = 0;
	while ((*buf)[i])
	{
		if ((*buf)[i] == '\n')
		{
			newbuf = calloc(1, sizeof(*newbuf) * (strlen(*buf + i + 1) + 1));
			if (newbuf == NULL)
				return (-1);
			strcpy(newbuf, *buf + i + 1);
			*msg = *buf;
			(*msg)[i + 1] = 0;
			*buf = newbuf;
			return (1);
		}
		i++;
	}
	return (0);
}

char *str_join(char *buf, char *add)
{
	char	*newbuf;
	int		len;

	if (buf == 0)
		len = 0;
	else
		len = strlen(buf);
	newbuf = malloc(sizeof(*newbuf) * (len + strlen(add) + 1));
	if (newbuf == 0)
		return (0);
	newbuf[0] = 0;
	if (buf != 0)
		strcat(newbuf, buf);
	free(buf);
	strcat(newbuf, add);
	return (newbuf);
}