queue = q.out
queue_src = $(wildcard ../queue/cli_queue.c queue_tests.c)

queue_objs = $(queue_src:.c=.o)
flags = -g -Wall -Werror -Wextra -Wpedantic -std=c11 -pthread

$(queue): $(queue_objs)
	gcc $(queue_objs) $(flags) -o $(queue)

%.o: %.c %.h
	gcc -c $(flags) $< -o $@

clean:
	-rm ../queue/*.o
	-rm *.out
	-rm *.o

lint:
	clang-tidy *.c *.h