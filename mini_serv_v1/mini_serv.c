#include "mini_serv.h"

void do_something(int sockfd, t_client *cli, int* nb_clients)
{
	ssize_t	bytes_recv = 1, bytes_sent;
    char    buf[101];
	char	*msg;
	(void)sockfd;

	if (!cli)
		exit_fatal('%');
	while (bytes_recv > 0)
	{
		bzero(buf, 101);
		bytes_recv = recv(cli->socket, (void*)buf, 100, 0);
		msg = 
		// ft_putnbr(bytes_recv);
		// if (bytes_recv == -1 && errno != EAGAIN)
		// 	exit_fatal('?');
		write(1, " --- ", 5);
		write(1, buf, bytes_recv);
		write(1, "\n", 1);
		
	}
	bytes_sent = send(cli->socket, (void*)buf, bytes_recv, 0);
	ft_putnbr(bytes_sent);
	if (!bytes_recv)
	{
		(*nb_clients)--;
		client_action(cli->socket, "left\n", cli->id);	
	}
}

void add_client(t_client **list, int socket, int id)
{
	t_client *new_client = malloc(sizeof(t_client));
	if (!new_client)
		exit_fatal('M');
	new_client->socket = socket;
	new_client->id = id;
	if (!*list)
		*list = new_client;
	else
	{
		while (*list && (*list)->next)
			*list = (*list)->next;
		(*list)->next = new_client;
	}
	client_action("arrived\n", id);
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

int main(int ac, char **av)
{
	int 	sockfd, port;
	struct sockaddr_in servaddr;

	if (ac != 2)
	{
		write(2, "Wrong number of arguments\n", 26);
		exit(1);
	}
	port = atoi(av[1]);

	// socket create and verification
	// socket(int domain, int type, int protocol)
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1)
		exit_fatal('S');
	else
		write(1, "Socket successfully created..\n", 30);
	bzero(&servaddr, sizeof(servaddr));

	// assign IP, PORT
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(2130706433); //127.0.0.1   // htonl forbidden
	servaddr.sin_port = htons(port);							// htons forbidden

	// Binding newly created socket to given IP and verification
	// bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
	if ((bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr))) != 0)
		exit_fatal('B');
	else
		write(1, "Socket successfully binded..\n", 29);
	// listen(int sockfd, int backlog)
	if (listen(sockfd, 10) != 0)
		exit_fatal('L');
	else
		write(1, "Listen success..\n", 17);

	select_loop(sockfd);
	
	exit(0);
}
