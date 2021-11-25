

void	select_loop(int sockfd)
{
	int     ret, select_ret, connfd;
	int     max_socket = 0;
    fd_set  current_sockets, read_sockets, write_sockets;
	struct sockaddr_in cli;

    while (1)
    {
		FD_ZERO(current_sockets);
        if ((*it)->getClientSocket() == -1)
		{
			FD_SET((*it)->getSocket(), current_sockets);
			if ((*it)->getSocket() > *max_socket)
				*max_socket = (*it)->getSocket();
		}
		else
		{
			FD_SET((*it)->getClientSocket(), current_sockets);
			if ((*it)->getClientSocket() > *max_socket)
				*max_socket = (*it)->getClientSocket();
		}
        FD_ZERO(read_sockets);
        FD_ZERO(write_sockets);
        memcpy(read_sockets, current_sockets, sizeof(*current_sockets));  // memcpy forbidden
        memcpy(write_sockets, current_sockets, sizeof(*current_sockets)); // memcpy forbidden
        select_ret = select(max_socket + 1, &read_sockets, &write_sockets, NULL, NULL);
        if (select_ret < 0)
            break ;
        if (select_ret > 0)
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
                            write(1, "server acccept the client...\n", 29);
                        fcntl(connfd, F_SETFL, O_NONBLOCK);
                        FD_SET(connfd, &current_sockets);
                        if (connfd > max_socket)
                            max_socket = connfd;
                    }
                    else if (FD_ISSET(i, &write_sockets) && connfd == i)
                    {
                        (*it)->exec_server();
                        FD_CLR(i, &current_sockets);
                        FD_CLR(i, &read_sockets);
                    }
                }
            }
        }
	}
}