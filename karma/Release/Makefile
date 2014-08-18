CC = g++
CFLAGS = -Wall -g
OBJ_DIR = ./
SRC_DIR = ../
LIBS = -lpthread -lpqxx -lpq

SRC = karma.cpp Helpers.cpp Message.cpp TLV.cpp
OBJS =	$(SRC:.cpp=.o)
CSources=$(addprefix $(SRC_DIR),$(SRC))
CObjects=$(addprefix $(OBJ_DIR),$(OBJS))
TARGET = karma

all: $(CSources) $(TARGET)

$(TARGET):	$(CObjects)
	$(CC) $(CFLAGS) -o $(TARGET) $(CObjects) $(LIBS)

$(OBJ_DIR)%.o: $(SRC_DIR)%.cpp
	$(CC) -c $(CFLAGS) $< -o $@

$(OBJ_DIR)karma.o: $(OBJ_DIR)Helpers.o $(OBJ_DIR)Message.o $(OBJ_DIR)TLV.o
$(OBJ_DIR)Helpers.o: $(OBJ_DIR)Message.o
$(OBJ_DIR)Message.o: $(OBJ_DIR)TLV.o

clean:
	rm -f *~ $(CObjects) $(TARGET)