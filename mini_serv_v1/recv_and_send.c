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
		bytes_recv = recv(socket, buf, 100, MSG_DONTWAIT);
		bytes_recv_total += bytes_recv;
		tmpmsg = str_join(tmpmsg, buf);	
	}
	print_bytes_recv(author_id, bytes_recv, bytes_recv_total);
	while (extract_message(&tmpmsg, &buf))
	{
		buf = put_prefix(buf, author_id);
		*msg = str_join(*msg, buf);
	}
	free(buf);
	free(tmpmsg);
	return (bytes_recv);
}

void send_to_all(int *cli_fd, int id, char **msg, int max)
{
	if (!*msg)
		return;
	write(1, "Sending ", 8);
	ft_putnbr(strlen(*msg));
	write(1, " to all|", 8);
	for (int i = 0; i < max; i++)
	{
		if (cli_fd[i] && id != i)
			send(cli_fd[i], *msg, strlen(*msg), 0);
	}
	free(*msg);
	*msg = NULL;
}