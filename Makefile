NAME = minisynth
S = src/
SRC =	$Smain.c \
		$Sload.c

HDR = minisynth.h
OBJ = $(SRC:%.c=%.o)

LIBFT = libft/
LIBA = libft/libft.a

CC = clang
CFLAGS = -Wall -Wextra -Werror
RM = /bin/rm -f

all: $(NAME)

$(NAME): $(SRC) $(OBJ) $(LIBA)
	$(CC) -o $@   $(SRC) $(LIBA) -flto -O3

mb: $(SRC) $(OBJ) $(LIBA)
	$(CC) -o $@  $(SRC) $(LIBA) -flto -O3

$(LIBA):
	make -C $(LIBFT)

%.o: %.c $(LIBA) $(HDR)
	gcc $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJ)
	make -C $(LIBFT) clean

fclean: clean
	$(RM) $(NAME)
	make -C $(LIBFT) fclean

re: fclean all
