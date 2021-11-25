#include "mini_serv.h"

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
