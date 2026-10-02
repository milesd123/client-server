# for my mac
# g = gcc -g -I/opt/homebrew/opt/openssl/include -L/opt/homebrew/opt/openssl/lib -lcrypto 

# linux link w libssl (install libssl-dev)
g = gcc -fpermissive
l = -lssl -lcrypto

all: build/server build/client

build/server: build/server.o build/blockcipher.o 
	$(g) $^ -o $@ $(l)

build/client: build/client.o build/blockcipher.o
	$(g) $^ -o $@ $(l)

build/client.o: sources/client.c
	$(g) -c $^ -o $@

build/server.o: sources/server.c
	$(g) -c $^ -o $@

build/blockcipher.o: sources/blockcipher.c
	$(g) -c $^ -o $@

clean:
	rm build/*