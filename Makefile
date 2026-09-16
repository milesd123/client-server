# for linux, should be in system include path...
# g = gcc

# for my mac
g = gcc -g -I/opt/homebrew/opt/openssl/include -L/opt/homebrew/opt/openssl/lib -lcrypto 

all: build/server build/client

build/server: build/server.o build/streamcipher.o
	$(g) $^ -o $@

build/client: build/client.o build/streamcipher.o
	$(g) $^ -o $@

build/client.o: sources/client.c
	$(g) -c $^ -o $@

build/server.o: sources/server.c
	$(g) -c $^ -o $@

build/streamcipher.o: sources/streamcipher.c
	$(g) -c $^ -o $@

clean:
	rm build/*