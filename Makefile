MSG = hello
DOMAIN = localhost

.PHONY: run-server server.c
run-server: build-server
	./server

.PHONY: build-server
build-server: server.c
	gcc -Wall -o server server.c

.PHONY: run-client client.c
run-client: build-client
	./client ${MSG} ${DOMAIN}

.PHONY: build-client
build-client: client.c
	gcc -Wall -o client client.c