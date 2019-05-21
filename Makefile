CC = tcc
CFLAGS = -g -Wunsupported -Wwrite-strings -Werror -Wall

#CC = gcc
#CFLAGS = -g -Werror -Wall

main : main.c csvparser.c

clean :
	rm main
