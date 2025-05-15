# Nom de l'exécutable
NAME = GTA

# Compilateur et options
CC = cc
CFLAGS =

# Fichiers source et objets
SRC =  src/main.c

OBJ = $(SRC:.c=.o)

# Inclusion des headers
LIBS = -Lmlx_linux -lm -lX11 -lXext 

INCLUDES = -I. -Imlx_linux

# Règles
all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(INCLUDES) mlx_linux/libmlx_Linux.a $(OBJ) $(LIBS) -o $(NAME) 

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re

#cc src/main.o mlx_linux/libmlx_Linux.a -Imlx_linux -I. -lm -lX11 -lXext