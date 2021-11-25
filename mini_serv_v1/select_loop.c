#include "mini_serv.h"

void	select_loop(int sockfd)
{
	unsigned int        len;
	int                 nb_fd, connfd = -1, max_socket = 0; // nb_clients = 0
    fd_set              read_sockets, write_sockets;
	struct sockaddr_in  cli;
    // char                *buf = malloc(sizeof(char *) * 100);
    int                 *current_sockets = malloc(sizeof(int *) * 2);

    ft_putnbr(sockfd);
    current_sockets[0] = sockfd;
    current_sockets[1] = -42;
    print_current_set(current_sockets);
    if (sockfd > max_socket)
        max_socket = sockfd;
    while (1)
    {
        read_sockets = tab_to_fd_set(current_sockets);
        write_sockets = tab_to_fd_set(current_sockets);
        // FD_SET(sockfd, &read_sockets);
        write(1, "select_loop() : prepared my sets\n", 33);
        nb_fd = select(max_socket + 1, &read_sockets, &write_sockets, NULL, NULL);
        if (nb_fd == -1)
            exit_fatal('>');
        if (nb_fd > 0)
        {
            print_fd_set("READ", &read_sockets, max_socket);
            print_fd_set("WRITE", &write_sockets, max_socket);
            // print_current_set(current_sockets);
            for (int i = 0; current_sockets[i] != -42; i++)
            {
                if (FD_ISSET(current_sockets[i], &read_sockets))
                {
                    if (current_sockets[i] == sockfd)// && connfd < 0)
                    {
                        len = sizeof(cli);
                        // accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen)
                        connfd = accept(sockfd, (struct sockaddr *)&cli, &len);
                        if (connfd < 0)
                            exit_fatal('A');
                        write(1, "server acccepted the client...\n", 31);
                        // ft_sprintf("server: client %d just arrived\n", nb_clients++);
                        // fcntl(connfd, F_SETFL, O_NONBLOCK);
                        FD_SET(connfd, &read_sockets);  // ??
                        current_sockets = add_fd_to_tab(connfd, current_sockets);
                        if (connfd > max_socket)
                            max_socket = connfd;
                    }
                    else if (FD_ISSET(current_sockets[i], &write_sockets) && current_sockets[i] == connfd)
                    {
                        write(1, "SUCCESS !\n", 10);
                        // FD_CLR(i, &current_sockets);   // ??
                        current_sockets[i] = -1;
                        FD_CLR(connfd, &read_sockets);
                    }
                }
            }
        }
        exit(0);
	}
}