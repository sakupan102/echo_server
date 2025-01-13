MSG = hello
DOMAIN = localhost
CFLAGS += -I/usr/include/kqueue
LDFLAGS += -lkqueue


.PHONY: run-server server.c
run-server: build-server
	./server

.PHONY: build-server
build-server: server.c
	gcc ${CFLAGS} -Wall -o server server.c ${LDFLAGS}

.PHONY: run-client client.c
run-client: build-client
	./client ${DOMAIN}

.PHONY: build-client
build-client: client.c
	gcc -Wall -o client client.c