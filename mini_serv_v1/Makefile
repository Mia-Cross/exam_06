NAME = mini_serv

COMPILER = clang
CFLAGS = -Wall -Wextra -Werror -g -fsanitize=address

SRCS =	mini_serv.c recv_and_send.c utils.c debug.c

OBJDIR = .obj
OBJS = $(addprefix $(OBJDIR)/, $(SRCS:.c=.o))
DPDCS = $(OBJS:.o=.d)

-include $(DPDCS)

opti:
	@make -j all

all : 
	@make $(NAME)

$(NAME) : $(OBJS)
	@printf "[$(NAME)] "
	$(COMPILER) $(CFLAGS) -o $(NAME) ${OBJS}
	@echo Compiled $(NAME) successfully !

$(OBJDIR)/%.o : %.c | $(OBJDIR)
	$(COMPILER) $(CFLAGS) -c $< -o $@

$(OBJDIR) :
	@mkdir -p .obj 

clean :
	@(rm -rf $(OBJDIR))

fclean :  clean
	@(rm -rf $(NAME))
	@(rm -f a.out)

re : fclean opti