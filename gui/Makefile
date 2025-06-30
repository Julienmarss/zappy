##
## EPITECH PROJECT, 2025
## Zappy GUI C++
## File description:
## GUI Makefile with Raylib and C++
##

NAME	=	../zappy_gui

CXX	=	g++

RM	=	rm -f

CXXFLAGS	=	-Wall -Wextra -Werror -std=c++17
CXXFLAGS	+=	-I./include

# Debug flags
DEBUG_FLAGS	=	-g3 -DDEBUG

# Raylib flags
RAYLIB_FLAGS	=	-lraylib -lGL -lm -lpthread -ldl -lrt -lX11

SRCS	=	src/main.cpp				\
		src/core/Application.cpp		\
		src/core/GameState.cpp			\
		src/network/NetworkClient.cpp		\
		src/network/ProtocolHandler.cpp		\
		src/graphics/Camera.cpp			\
		src/graphics/Renderer.cpp		\
		src/graphics/UI.cpp			\
		src/game/Map.cpp			\
		src/game/Player.cpp			\
		src/game/Team.cpp

OBJS	=	$(SRCS:.cpp=.o)

all:	$(NAME)

$(NAME):	$(OBJS)
	@echo "Linking $(NAME)..."
	@$(CXX) -o $(NAME) $(OBJS) $(RAYLIB_FLAGS)
	@echo "✅ C++ GUI compiled successfully!"

%.o: %.cpp
	@echo "Compiling $<..."
	@$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	@echo "Cleaning object files..."
	@$(RM) $(OBJS)

fclean:	clean
	@echo "Cleaning executable..."
	@$(RM) $(NAME)

re:	fclean all

debug:	CXXFLAGS += $(DEBUG_FLAGS)
debug:	re
	@echo "✅ Debug build completed!"

install-deps:
	@echo "Installing Raylib dependencies..."
	@if command -v apt-get >/dev/null 2>&1; then \
		sudo apt-get update && sudo apt-get install -y libraylib-dev; \
	elif command -v dnf >/dev/null 2>&1; then \
		sudo dnf install -y raylib-devel; \
	elif command -v pacman >/dev/null 2>&1; then \
		sudo pacman -S raylib; \
	else \
		echo "Package manager not supported. Please install Raylib manually."; \
	fi

test:	debug
	@echo "Testing C++ GUI connection..."
	@./$(NAME) -h localhost -p 4242 || echo "Test failed (normal if server not running)"

.PHONY:	all clean fclean re debug test install-deps