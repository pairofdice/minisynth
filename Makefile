NAME = minisynth
S = src/
SRC =	$Smain.c \
		$Sload.c \
		$Snotes.c

HDR = minisynth.h
OBJ = $(SRC:%.c=%.o)

LIBFT = libft/
LIBA = libft/libft.a

L_SDL = -l SDL2-2.0.0 -l SDL2_image-2.0.0
INCLUDES	=	-I./frameworks/SDL2.framework/Versions/A/Headers \
				-I./frameworks/SDL2_ttf.framework/Versions/A/Headers \
				-I./frameworks/SDL2_image.framework/Versions/A/Headers \
				-I./frameworks/SDL2_mixer.framework/Headers \
				-F./frameworks/
FRAMEWORKS	=	-F./frameworks \
				-rpath ./frameworks \
				-framework OpenGL -framework AppKit -framework OpenCl \
				-framework SDL2 -framework SDL2_ttf -framework SDL2_image \
				-framework SDL2_mixer

CC = clang
CFLAGS =
# -Wall -Wextra -Werror
RM = /bin/rm -f

all: $(NAME)

$(NAME): $(SRC) $(OBJ) $(LIBA)
	$(CC) -o $@   $(SRC) $(LIBA) $(FRAMEWORKS) -fsanitize=address -g

mb: $(SRC) $(OBJ) $(LIBA)
	$(CC) -o $@  $(SRC) $(LIBA) -flto -O3

$(LIBA):
	make -C $(LIBFT)

%.o: %.c $(LIBA) $(HDR)
	gcc $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	$(RM) $(OBJ)
	make -C $(LIBFT) clean

fclean: clean
	$(RM) $(NAME)
	make -C $(LIBFT) fclean

re: fclean all
