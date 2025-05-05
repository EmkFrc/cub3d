# Nom de l'exécutable
NAME = GTA

# Compilateur et options
CC = cc
CFLAGS = -Wall -Wextra -Werror

# Fichiers source et objets
SRC =  src/main.c

OBJ = $(SRC:.c=.o)

# Inclusion des headers
LIBS = -Lmlx_linux -lm -lXext -lX11

INCLUDES = -I. -Imlx_linux

# Règles
all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(INCLUDES) -o $(NAME) $(OBJ) $(LIBS) mlx_linux/libmlx_Linux.a

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
