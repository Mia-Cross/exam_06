#include "mini_serv.h"

int read_message(t_client *cli, t_client *list, int sockfd)
{
	ssize_t	bytes_recv = 1;
    char    buf[101];
	char	*msg = NULL;
	// int last;

	if (!cli)
		exit_fatal('%');
	while (bytes_recv > 0)
	{
		bzero(buf, 101);
		bytes_recv = recv(cli->socket, buf, 100, 0);
		msg = str_join(msg, buf);
		// if (bytes_recv == -1 && errno != EAGAIN)
		// 	exit_fatal('?');		
	}
	write(1, "<", 1);
	ft_putnbr(bytes_recv);
	write(1, "|", 1);
	send_to_all(sockfd, buf, list, cli);
	return (bytes_recv);
}

void send_to_all(int sockfd, char *msg, t_client *list, t_client *cli)
{
	int bytes_sent, last;
	char *buf = malloc(strlen(msg) + 20);

	if (!buf)
		exit_fatal('M');
	bzero(buf, strlen(msg) + 20);
	// ft_putnbr(cli->id);
	last = sprintf(buf, "client %d: ", cli->id);
	strcpy(buf + last, msg);
	// free(msg);
	bytes_sent = send(cli->socket, buf, strlen(buf), 0);
	bytes_sent = send(sockfd, buf, strlen(buf), 0);
	write(1, ">", 1);
	ft_putnbr(bytes_sent);
	write(1, "|", 1);
	recv(sockfd, buf, bytes_sent, 0);
	while (list)
	{
		if (list->socket != cli->socket)
			send(list->socket, buf, bytes_sent, 0);
		list = list->next;
	}
	free(buf);
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

// void do_something(int sockfd, t_client *cli, int* nb_clients)
// {
// 	ssize_t	bytes_recv = 1, bytes_sent;
//     char    buf[101];
// 	char	*msg = NULL;
// 	int last;
// 	// (void)sockfd;

// 	if (!cli)
// 		exit_fatal('%');
// 	while (bytes_recv > 0)
// 	{
// 		bzero(buf, 101);
// 		bytes_recv = recv(cli->socket, buf, 100, 0);
// 		printf("msg before = [%s]\n", msg);
// 		printf("buf before = [%s]\n", buf);
// 		msg = str_join(msg, buf);
// 		printf("msg after = [%s]\n", msg);
// 		write(1, "<", 1);
// 		ft_putnbr(bytes_recv);
// 		// if (bytes_recv == -1 && errno != EAGAIN)
// 		// 	exit_fatal('?');		
// 	}
// 	if (bytes_recv < 0)
// 	{
// 		*nb_clients -= 1;
// 		client_action(cli->socket, "left\n", cli->id);
// 		return;
// 	}
// 	bzero(buf, 101);
// 	last = sprintf(buf, "client %d: ", cli->id);
// 	strcpy(buf + last, msg);
// 	free(msg);
// 	bytes_sent = send(cli->socket, buf, strlen(buf), 0);
// 	bytes_sent = send(sockfd, buf, strlen(buf), 0);
// 	write(1, ">", 1);
// 	ft_putnbr(bytes_sent);
// }
