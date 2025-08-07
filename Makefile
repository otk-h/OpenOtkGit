CC = gcc
src = main.c OpenOtkGit.c
target = OpenOtkGit

$(target): $(src)
	$(CC) $(src) -o $(target)

clean:
	rm -f $(target)
	rm -f -r "./.OpenOtkGit"