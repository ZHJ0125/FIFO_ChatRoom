#*****************************************************************
#   File Name: Makefile
#   Description: Compile to generate executable file.
#   Instructions: \
#	 $ make			-> Generate server and client, and delete the object files.
#	 $ make clean	-> Clear server and client.
#	Others: \
#	 $^		-> Collection of all target-dependent.
#	 -rm	-> Ignore the error message during the execution of "rm".
#	 @		-> Do not echo commands
#   Author: Zhang Houjin
#   Date: 2020/04/06
#*****************************************************************

client_objects = client.o fifo.o
server_objects = server.o fifo.o

.PHONY: all clean
all: client server clean_o

client: $(client_objects)
	@gcc -o client $^

server: $(server_objects)
	@gcc -o server $^

clean_o:
	rm ./*.o

clean:
	-rm client server
