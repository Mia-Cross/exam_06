#include "mini_serv.h"

void exit_fatal(char c)
{
	write(2, &c, 1);
	write(2, ".Fatal error\n", 13);
	if (errno)
		perror(strerror(errno));
	exit(1);
}

void client_action(t_client *cli, const char *action, t_client *list, int sockfd)
{
	char buf[50];
	// char *buf = malloc(50);
	int last;

	// if (!buf)
	// 	exit_fatal('M');
	bzero(buf, 50);
	last = sprintf(buf, "server: client %d just ", cli->id);
	strcpy(buf + last, action);
	send_to_all(sockfd, buf, list, cli);
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
	// ft_putnbr(id);
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
		printf("First: removing socket %d,target is %d\n", to_del->socket, socket);
		*list = (*list)->next;
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
		printf("Other: removing socket %d,target is %d\n", to_del->socket, socket);
		client->next = client->next->next;
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

	*msg = 0;
	if (*buf == 0)
		return (0);
	i = 0;
	while ((*buf)[i])
	{
		if ((*buf)[i] == '\n')
		{
			newbuf = calloc(1, sizeof(*newbuf) * (strlen(*buf + i + 1) + 1));
			if (newbuf == 0)
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
// int *add_fd_to_tab(int fd, int *tab)
// {
// 	int i = 0;
// 	for ( ; tab[i]; i++)
// 		;
// 	int *newtab = malloc(sizeof(int*) * (i + 1));
// 	for (int j = 0; j < i; j++)
// 		newtab[j] = tab[j];
// 	newtab[i++] = fd;
// 	newtab[i] = -42;
// 	free(tab);
// 	return (newtab);
// }

// fd_set tab_to_fd_set(int *tab)
// {
// 	fd_set newset;
// 	FD_ZERO(&newset);
// 	print_current_set(tab);
// 	for (int i = 0; tab[i] != -42; i++)
// 		FD_SET(i, &newset);
// 	return (newset);
// }

// int nb_len(int x) {
// 	int len = 0;
// 	while (x /= 10 && x > 0)
// 		len++;
// 	return len;
// }

// void ft_memcpy(void *src, void *dest, int size)
// {
// 	unsigned char *s = src;
// 	unsigned char *d = dest;
// 	for (int i = 0; i < size; i++)
// 		d[i] = s[i];
// }