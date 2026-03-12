NAME = node_mlx.node
CXX = c++
## TODO: make this dynamic
NODEJS_DEV_ROOT ?= /home/pmihangy/.nvm/versions/node/v24.13.0/include/node
MLX_INCLUDE_PATH = ./minilibx
CXXFLAGS = -std=c++20 -I$(NODEJS_DEV_ROOT) -I$(MLX_INCLUDE_PATH) -fPIC -shared
MLX_LINKING = -Lminilibx -lmlx -lXext -lX11
SRC_FILES = node_mlx.cpp
RM = rm -rf

all: $(NAME)

$(NAME): $(SRC_FILES)
	$(CXX) $(CXXFLAGS) $(SRC_FILES) -o $(NAME) $(MLX_LINKING)

fclean:
	$(RM) $(NAME)

re: fclean all

.PHONY: all fclean re
