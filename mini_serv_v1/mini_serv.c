#include "mini_serv.h"

void	handle_connections(int sockfd)
{
	unsigned int        len;
	int                 nb_fd, connfd, max_socket = sockfd, id = 0;
    fd_set              read_sockets, write_sockets;
	struct sockaddr_in  cli;
    t_client            *list = NULL;
    char                *msg = NULL;

    while (1)
    {
        FD_ZERO(&read_sockets);
        FD_ZERO(&write_sockets);
        FD_SET(sockfd, &read_sockets);
        FD_SET(sockfd, &write_sockets);
        for (t_client *cli = list; cli; cli = cli->next)
        {
            FD_SET(cli->socket, &read_sockets);
            FD_SET(cli->socket, &write_sockets);
            if (cli->socket > max_socket)
                max_socket = cli->socket;
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
            else
            {
                t_client *new_client = add_client(&list, connfd, id++);
                client_action(new_client, "arrived\n", list);
            }
			display_cli_id(connfd, id - 1);
        }

        //check the other sockets for activity
        for (t_client *cli = list; cli; cli = cli->next)
        {
            if (FD_ISSET(cli->socket, &read_sockets))
            {
                if (read_from_client(cli->socket, &msg, cli->id) == 0)
                {
                    send_to_all(list, &msg, -1);
                    client_action(cli, "left\n", list);
                    FD_CLR(cli->socket, &read_sockets);
                    FD_CLR(cli->socket, &write_sockets);
                    remove_client(&list, cli->socket);
                    break;
                }
            }
            if (FD_ISSET(cli->socket, &write_sockets) && msg)
            {
				print_writeable_socket(cli->socket);
                send_to_all(list, &msg, cli->socket);
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