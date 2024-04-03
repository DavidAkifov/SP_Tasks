CC = gcc
CFLAGS = -Wall -Wextra

SRCS = Tasks.c TaskTwo.c TaskThree.c TaskFour.c TaskFive.c
EXECS = $(SRCS:.c=)

all: $(EXECS)

%: %.c
	$(CC) $(CFLAGS) $< -o $@ -lpthread

clean:
	rm -f $(EXECS)
