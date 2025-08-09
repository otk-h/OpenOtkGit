CC = gcc
CFLAGS = -I./include

SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)
target = OpenOtkGit

all: $(target) clean_objs

$(target): $(OBJ)
	$(CC) $(OBJ) -o $(target) $(CFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean_objs:
	rm -f $(OBJ)

clean:
	rm -f $(target)
	rm -f $(OBJ)
	rm -f -r "./.OpenOtkGit"
