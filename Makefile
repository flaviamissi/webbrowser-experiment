run: clean
	@clang++ -std=c++11 -stdlib=libc++ -Wall -o main.o main.cpp -lcurl
	@./main.o

clean:
	@find . -name *.o -delete
