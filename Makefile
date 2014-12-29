run: clean
	@clang++ -std=c++11 -stdlib=libc++ -Wall -o main.o main.cpp -lcurl
	@./main.o ${url}

clean:
	@find . -name *.o -delete
