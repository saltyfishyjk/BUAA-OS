all : hello_os.c
	gcc hello_os.c -o os_hello
clean : 
	rm os_hello
