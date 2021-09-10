
all: mpi # shmem # bb

mpi:
	gcc -g -Wall -Werror -O3 server.c -lrt -o server
	gcc -g -Wall -Werror -O3 client1.c -lrt -o client1
	gcc  timer.c -lrt -o timer

shmem:
	gcc -g -Wall -Werror -O3 server_shmem.c -lrt -o server_shmem
	gcc -g -Wall -Werror -O3 client_shmem.c -lrt -o client_shmem

# bb:
# 	gcc -g -Wall -Werror -O3 server_bb.c -lrt -o server_bb
# 	gcc -g -Wall -Werror -O3 client_bb.c -lrt -o client_bb
