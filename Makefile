.PHONY: all

all:
	rm -f client.out
	gcc clientside_for_frontend.c -o client.out

