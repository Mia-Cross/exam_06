#include "mini_serv.h"

void	select_loop(int sockfd)
{
	unsigned int        len;
	int                 nb_fd, connfd = -1, max_socket = 0, nb_clients = 0;
    fd_set              read_sockets, write_sockets, current_sockets;
	struct sockaddr_in  cli;
    t_client            *list = NULL;

    while (1)
    {
        FD_ZERO(&current_sockets );   /* Empty the FD Set */
        if (connfd == -1)
        {
            FD_SET(sockfd, &current_sockets);
            if (sockfd > max_socket)
                max_socket = sockfd;
        }
        else
        {
            FD_SET(connfd, &current_sockets);
            if (connfd > max_socket)
                max_socket = connfd;
        }
        FD_ZERO(&read_sockets);   /* Empty the FD Set */
        FD_ZERO(&write_sockets);   /* Empty the FD Set */
        ft_memcpy(&current_sockets, &read_sockets, sizeof(current_sockets));
        ft_memcpy(&current_sockets, &write_sockets, sizeof(current_sockets));
        
        // print_fd_set("READ", &read_sockets, max_socket);
        // print_fd_set("WRITE", &write_sockets, max_socket);

        /* Selection handling */
        nb_fd = select(max_socket + 1, &read_sockets, &write_sockets, NULL, NULL);
        if (nb_fd < 0)
            exit_fatal('>');
        else if (nb_fd > 0)
        {
            for (int i = 0; i < max_socket + 1; i++)
            {
                if (FD_ISSET(i, &read_sockets))
                {
                    if (i == sockfd && connfd < 0)
                    {
                        len = sizeof(cli);
                        connfd = accept(sockfd, (struct sockaddr *)&cli, &len);
                        if (connfd < 0)
                            exit_fatal('A');
                        else
                            add_client(&list, connfd, nb_clients++);
                        FD_SET(connfd, &current_sockets);
                        // FD_SET(connfd, &read_sockets);
                        if (connfd > max_socket)
                            max_socket = connfd;
                    }
                    else if (i == connfd && FD_ISSET(i, &write_sockets))
                    {
                        write(1, "--> lez go ", 11);
                        ft_putnbr(connfd);
                        write(1, " -->\n", 5);
                        do_something(sockfd, get_client(list, connfd), &nb_clients);
                        FD_CLR(i, &current_sockets);
                        FD_CLR(i, &read_sockets);
                    }
                }
            }
        }
	}
}