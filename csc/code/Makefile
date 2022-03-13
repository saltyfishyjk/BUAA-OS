subprocess : fibo.c main.c
	gcc -I ../include -c fibo.c
	gcc -I ../include -c main.c
	gcc fibo.o main.o -o fibo
	mv fibo ../fibo
clean : 
	rm fibo.o
	rm main.o
