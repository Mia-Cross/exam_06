#include "mini_serv.h"

int read_from_client(int socket, char **msg, int author_id)
{
	ssize_t	bytes_recv = 1;
	ssize_t	bytes_recv_total = 0;
    char    *buf = malloc(101);
	char	*tmpmsg = NULL;

	if (!buf)
		exit_fatal('M');
	while (bytes_recv > 0)
	{
		bzero(buf, 101);
		bytes_recv = recv(socket, buf, 100, 0);
		bytes_recv_total += bytes_recv;
		tmpmsg = str_join(tmpmsg, buf);	
	}
	print_bytes_recv(author_id, bytes_recv, bytes_recv_total);
	while (extract_message(&tmpmsg, &buf))
	{
		buf = put_prefix(buf, author_id);
		*msg = str_join(*msg, buf);
	}
	return (bytes_recv);
}

void send_to_all(t_client *list, char **msg, int author_socket)
{
	write(1, "Sending ", 8);
	ft_putnbr(strlen(*msg));
	write(1, " to all|", 8);
	while (list)
	{
		if (list->socket != author_socket)
			send(list->socket, *msg, strlen(*msg), 0);
		list = list->next;
	}
	free(*msg);
	*msg = NULL;
}