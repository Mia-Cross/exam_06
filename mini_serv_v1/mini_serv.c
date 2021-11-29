#include "mini_serv.h"
#define MAX_CLI 32720

void	handle_connections(int sockfd)
{
	unsigned int        len;
	int                 nb_fd, connfd, max_socket = sockfd, id = 0;
    fd_set              read_sockets, write_sockets;
	struct sockaddr_in  cli;
    char                *msg = NULL;
    int                 cli_fd[MAX_CLI];

    bzero(cli_fd, MAX_CLI);
    while (1)
    {
        FD_ZERO(&read_sockets);
        FD_ZERO(&write_sockets);
        FD_SET(sockfd, &read_sockets);
        FD_SET(sockfd, &write_sockets);
        for (int i = 0; i < id; i++)
        {
            FD_SET(cli_fd[i], &read_sockets);
            FD_SET(cli_fd[i], &write_sockets);
            if (cli_fd[i] > max_socket)
                max_socket = cli_fd[i];
        }
        // print_fd_set("READ", &read_sockets, max_socket);
        // print_fd_set("WRITE", &write_sockets, max_socket);
        nb_fd = select(max_socket + 1, &read_sockets, &write_sockets, NULL, NULL);
        if (nb_fd < 0)
            exit_fatal('>');
        
        //check if something happened on the server socket (incoming connexion)
        if (FD_ISSET(sockfd, &read_sockets)) 
        {
            len = sizeof(cli);
            connfd = accept(sockfd, (struct sockaddr *)&cli, &len);
            if (connfd < 0)
                exit_fatal('A');
            cli_fd[id++] = connfd;
            client_action(cli_fd, id - 1, "arrived\n", id);
			display_cli_id(connfd, id - 1);
        }

        //check the other sockets for activity
        for (int i = 0; i < id; i++)
        {
            if (FD_ISSET(cli_fd[i], &read_sockets))
            {
                if (read_from_client(cli_fd[i], &msg, i) == 0)// && msg)
                {
                    send_to_all(cli_fd, i, &msg, 0);
                    client_action(cli_fd, i, "left\n", id);
                    FD_CLR(cli_fd[i], &read_sockets);
                    FD_CLR(cli_fd[i], &write_sockets);
                    close(cli_fd[i]);
                    cli_fd[i] = 0;
                    write(1, "client--\n", 9);
                    break;
                }
            }
            if (FD_ISSET(cli_fd[i], &write_sockets) && msg)
            {
				print_writeable_socket(i);
                send_to_all(cli_fd, i, &msg, id);
            }
        }
    }
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

	handle_connections(sockfd);
	
	exit(0);
}