

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

while(TRUE)  
    {  
        //clear the socket set 
        FD_ZERO(&readfds);  
        //add master socket to set 
        FD_SET(master_socket, &readfds);  
        max_sd = master_socket;  
             
        //add child sockets to set 
        for ( i = 0 ; i < max_clients ; i++)  
        {  
            //socket descriptor 
            sd = client_socket[i];  
            //if valid socket descriptor then add to read list 
            if(sd > 0)  
                FD_SET( sd , &readfds);  
            //highest file descriptor number, need it for the select function 
            if(sd > max_sd)  
                max_sd = sd;  
        }  
     
        //wait for an activity on one of the sockets , timeout is NULL , 
        //so wait indefinitely 
        activity = select( max_sd + 1 , &readfds , NULL , NULL , NULL);  
        if ((activity < 0) && (errno!=EINTR))  
            printf("select error");  
             
        //If something happened on the master socket , 
        //then its an incoming connection 
        if (FD_ISSET(master_socket, &readfds))  
        {  
            if ((new_socket = accept(master_socket, 
                    (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)  
                exit(EXIT_FAILURE);  
             
            //inform user of socket number - used in send and receive commands 
            printf("New connection , socket fd is %d , ip is : %s , port : %d 
                  \n" , new_socket , inet_ntoa(address.sin_addr) , ntohs
                  (address.sin_port));  
           
            //send new connection greeting message 
            if( send(new_socket, message, strlen(message), 0) != strlen(message) )  
                perror("send");            
            puts("Welcome message sent successfully");  
                 
            //add new socket to array of sockets 
            for (i = 0; i < max_clients; i++)  
            {  
                //if position is empty 
                if( client_socket[i] == 0 )  
                {  
                    client_socket[i] = new_socket;  
                    printf("Adding to list of sockets as %d\n" , i);  
                    break;  
                }  
            }  
        }  
             
        //else its some IO operation on some other socket
        for (i = 0; i < max_clients; i++)  
        {  
            sd = client_socket[i];  
            if (FD_ISSET( sd , &readfds))  
            {  
                //Check if it was for closing , and also read the 
                //incoming message 
                if ((valread = read( sd , buffer, 1024)) == 0)  
                {  
                    //Somebody disconnected , get his details and print 
                    getpeername(sd , (struct sockaddr*)&address , \
                        (socklen_t*)&addrlen);  
                    printf("Host disconnected , ip %s , port %d \n" , 
                          inet_ntoa(address.sin_addr) , ntohs(address.sin_port));  
                         
                    //Close the socket and mark as 0 in list for reuse 
                    close( sd );  
                    client_socket[i] = 0;  
                }  
                //Echo back the message that came in 
                else 
                {  
                    //set the string terminating NULL byte on the end 
                    //of the data read 
                    buffer[valread] = '\0';  
                    send(sd , buffer , strlen(buffer) , 0 );  
                }  
            }  
        }  
    }  
         
    return 0;  
}  