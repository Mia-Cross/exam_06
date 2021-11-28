#include "mini_serv.h"

int read_from_master_socket(int sockfd, char **msg)
{
	ssize_t	bytes_recv = 1;
    char    buf[101];

	while (bytes_recv > 0)
	{
		bzero(buf, 101);
		bytes_recv = recv(sockfd, buf, 100, 0);
		*msg = str_join(*msg, buf);
		
	}
	write(1, "Master received :", 17);
	ft_putnbr(bytes_recv);
	write(1, "|", 1);
	return (bytes_recv);
}

int read_from_client(int socket, char **msg, int author_id)
{
	ssize_t	bytes_recv = 1;
    char    buf[101];

	while (bytes_recv > 0)
	{
		bzero(buf, 101);
		bytes_recv = recv(socket, buf, 100, 0);
		*msg = str_join(*msg, buf);	
	}
	write(1, "Socket ", 7);
	ft_putnbr(socket);
	write(1, " received ", 10);
	ft_putnbr(bytes_recv);
	write(1, "|", 1);
	if (*msg)
		*msg = put_prefix(*msg, author_id);
	return (bytes_recv);
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

// void send_to_all(t_client *list, char *msg, int socket, int author_id)
void send_to_all(t_client *list, char *msg, int size)
{
	// int bytes_sent;

	// bytes_sent = send(socket, msg, strlen(msg), 0);
	// recv(sockfd, msg, bytes_sent, 0);
	// recv(cli->socket, msg, bytes_sent, 0);
	while (list)
	{
		// if (list->id != author_id)
		send(list->socket, msg, size, 0);
		list = list->next;
	}
	write(1, "Sent ", 5);
	ft_putnbr(size);
	write(1, " to all|", 8);
	free(msg);
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