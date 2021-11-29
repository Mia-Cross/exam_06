#include <errno.h>
#include <fcntl.h>

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>

void	ft_putnbr(int nb);
// void print_fd_set(char *name, fd_set *set, int max_socket);
void display_cli_id(int connfd, int id);
void print_bytes_recv(int author_id, ssize_t bytes_recv, ssize_t bytes_recv_total);
void print_writeable_socket(int socket);

void	exit_fatal(char c);
char *put_prefix(char *msg, int author_id);
int		extract_message(char **buf, char **msg);
char	*str_join(char *buf, char *add);
void client_action(int *cli_fd, int id, const char *action, int max);

int read_from_client(int socket, char **msg, int author_id);
void send_to_all(int *cli_fd, int id, char **msg, int max);

// typedef struct      s_client
// {
//     int             id;
//     int             socket;
//     struct s_client *next;
// }                   t_client;

// t_client *add_client(t_client **list, int socket, int id);
// t_client *get_client(t_client *list, int socket);
// void remove_client(t_client **list, int socket);