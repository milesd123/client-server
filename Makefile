g = gcc

all: server client

server: server.c
	$(g) $^ -o $@

client: client.c
	$(g) $^ -o $@

clean:
	rm client
	rm server