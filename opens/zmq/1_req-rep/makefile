all:
	gcc 1_zmqserver.c -lzmq -o server
	gcc 1_zmqclient.c -lzmq -o client

.PHONY:clean

clean:
	rm server client