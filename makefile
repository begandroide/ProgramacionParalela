all :
	gcc -pthread -o main fullArray.c -O3

clean :
	rm ./main

test1 :
	./main 100000 2

test2 :
	./main 100000 4