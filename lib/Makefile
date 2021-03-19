# Makefile for libbmp.a

# we find which operating system is used

ifeq ($(OS),Windows_NT)
	SUFFIX = win
else ifeq ($(shell uname -s),Linux)
	SUFFIX = linux
else
	SUFFIX = macos
endif

all: libbmp.a


# Το ίδιο για το libbmp.a
libbmp.a:
	cp libbmp_$(SUFFIX).a libbmp.a

clean:
	$(RM) libbmp.a