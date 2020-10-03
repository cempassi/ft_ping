# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: cempassi <cempassi@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/07/22 04:47:46 by cempassi          #+#    #+#              #
#    Updated: 2020/09/20 13:20:42 by cempassi         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = Clang
COMPILE = $(CC) -c
DEBUG = $(CC) -g -c

NAME = ft_ping
NAMEDB = ft_pingdb
LIB = $(PATHL)libft.a
LIBDB = $(PATHL)libftdb.a

# Reset
NC = \033[0m

# Regular Colors
BLACK = \033[0;30m
RED = \033[0;31m
GREEN = \033[32m
YELLOW = \033[0;33m
BLUE = \033[0;34m
PURPLE = \033[0;35m
CYAN = \033[0;36m
WHITE = \033[0;37m

# One Line Output
ONELINE =\e[1A\r

#Utilitaries
CLEANUP = rm -rf
MKDIR = mkdir -p
CLEAR = clear

#Folders
PATHO = objs/
PATHI = includes/
PATHL = libft/
PATHLI = libft/includes

#Includes
INCS += ft_ping.h

#Sources
SRCM += ping.c
SRCM += init.c
SRCM += main.c
SRCM += packet_io.c
SRCM += utils.c
SRCM += signals.c
SRCM += display.c

#Debug Files
DSYM += $(NAME).dSYM
DSYM += $(DBNAME).dSYM

#Object Files
OBJM = $(patsubst %.c, $(PATHO)%.o, $(SRCM))
OBJMD = $(patsubst %.c, $(PATHO)db%.o, $(SRCM))
OBJS = $(patsubst %.c, $(PATHO)%.o, $(SRCS))
OBJD = $(patsubst %.c, $(PATHO)db%.o, $(SRCS))

#Compilation Flags
WFLAGS += -Wall
WFLAGS += -Werror
WFLAGS += -Wextra
DFLAGS += -fsanitize=address
IFLAGS = -I$(PATHI) -I$(PATHLI)
CFLAGS = $(WFLAGS)

vpath %.c srcs
vpath %.h includes

all : $(NAME)

run : $(NAME)
	./$<
	
debug : $(NAMEDB)

#Normal compilation
$(NAME):  $(LIB) $(PATHO) $(OBJM) $(OBJS) $(INCS)
	$(CC)  -o $@ $(LIB) $(OBJS) $(OBJM)
	printf "$(GREEN)$@ is ready.\n$(NC)"
	
$(OBJS) : $(PATHO)%.o : %.c $(INCS)
	$(COMPILE) $(CFLAGS) $(IFLAGS) $< -o $@
	printf "$(BLUE)Compiling $<\n$(NC)"

$(OBJM) : $(PATHO)%.o : %.c $(INCS)
	$(COMPILE) $(CFLAGS) $(IFLAGS) $< -o $@
	printf "$(BLUE)Compiling $<\n$(NC)"

#Debug
$(NAMEDB): $(CLEAR) $(LIBDB) $(PATHO) $(OBJD) $(OBJMD) $(INCS)
	$(CC)  $(DFLAGS) -o $@ $(LIBDB) $(OBJD) $(OBJMD)
	
$(OBJD) : $(PATHO)db%.o : %.c $(INCS)
	$(DEBUG) $(DFLAGS) $(CFLAGS) $(IFLAGS) $< -o $@
	printf "$(BLUE)Compiling $< for debug\n$(NC)"

$(OBJMD) : $(PATHO)db%.o : %.c $(INCS)
	$(DEBUG) $(DFLAGS) $(CFLAGS) $(IFLAGS) $< -o $@
	printf "$(BLUE)Compiling $< for debug\n$(NC)"

#Libraries
$(LIB) : FORCE
	$(MAKE) -C $(PATHL)

$(LIBDB) : FORCE
	$(MAKE) -C $(PATHL) debug

$(PATHO) :
	$(MKDIR) $(PATHO)

$(CLEAR):
	$@

clean : $(CLEAR)
	$(MAKE) -C $(PATHL) clean
	$(CLEANUP) $(OBJS)
	$(CLEANUP) $(OBJM)
	$(CLEANUP) $(PATHO)
	printf "$(RED)All *.o files for $(NAME) removed\n$(NC)"
	$(CLEANUP) $(DSYM)
	printf "$(RED)All $(DSYM) removed\n$(NC)"

fclean : clean
	$(MAKE) -C $(PATHL) fclean
	$(CLEANUP) $(NAME)
	$(CLEANUP) $(NAMEDB)
	printf "$(RED)$(NAME) deleted\n$(NC)"
FORCE : 

re : fclean all

.PHONY: all clean fclean debug cleandb
.SILENT:
