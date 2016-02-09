#OBJS specifies which files to we are compiling
OBJS = bunny.cpp

#CC specifies the compiler we are using
CC = g++

#COMPILER_FLAGS specifies the additional options we are using to compile
#-w flagg suppresses warnings
COMPILER_FLAGS = 

#LINKER_FLAGS specifies libraries we're using
LINKER_FLAGS = -lSDL2 -lSDL2_image

#OBJ_NAME species the name of the executable we want to create
OBJ_NAME = bunny

#This is the command that feeds these variables into the compiler
all : $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)

