#include "mini_serv.h"

void print_fd_set(char *name, fd_set *set, int max_socket)
{
    write(1, name, strlen(name));
    write(1, "_SET = [ ", 9);
    for (int i = 0; i < max_socket + 1; i++)
    {
        if (FD_ISSET(i, set))
        {
            ft_putnbr(i);
            write(1, ", ", 2);
        }
    }
    write(1, "]\n", 2);
}

// void print_current_set(int *tab)
// {
//     write(1, "CURRENT_SET = [ ", 16);
//     for (int i = 0; tab[i] != -42; i++)
//     {
//         ft_putnbr(i);
//         write(1, ", ", 2);
//     }
//     write(1, "]\n", 2);
// }

void	ft_putnbr(int nb)
{
	char c;
	long p;

	if (nb < 0)
	{
		if (nb == -2147483648)
		{
			write(1, "-2147483648", 11);
			return ;
		}
		write(1, "-", 1);
		p = -1 * nb;
	}
	if (nb >= 0)
		p = nb;
	if (p > 9)
		ft_putnbr(p / 10);
	c = p % 10 + 48;
	write(1, &c, 1);
	// write(1, "\n", 1);
}