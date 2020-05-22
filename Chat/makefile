CC = g++

CLIENT = client.cpp
SERVER = server.cpp

PORT_NUM = 8080
SERVER_IP = 40.113.19.1
OBJ_CLIENT = client.o
OBJ_SERVER = server.o

all: client server

client:
	$(CC) $(CLIENT) -o $(OBJ_CLIENT)
server:
	$(CC) $(SERVER) -o $(OBJ_SERVER)
run:
	./$(OBJ_CLIENT) $(SERVER_IP) $(PORT_NUM)
clean:
	rm $(OBJ_CLIENT) $(OBJ_SERVER)