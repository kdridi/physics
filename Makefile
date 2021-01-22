CXX			=   clang++ -g3 -fsanitize=address -std=gnu++2a
CXXFLAGS	=   -W -Wall -Wextra -Werror
CPPFLAGS	=	-Isystem/include
LDFLAGS		=	-Lsystem/lib64 -lbox2d -Lsystem/lib -lixwebsocket -lz

SRC			=	$(wildcard src/*.cpp)
OBJ			=	$(SRC:.cpp=.o)

TARGET		=	server

all			:	$(TARGET)

$(TARGET)	:	$(OBJ)
			$(CXX) -o $@ $^ $(LDFLAGS)

clean		:
			$(RM) $(OBJ)

fclean		:	clean
			$(RM) $(TARGET)

re			:	fclean all