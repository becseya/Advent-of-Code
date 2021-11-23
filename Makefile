all: a.out

a.out: main.cpp
	clang-format -i main.cpp
	g++ main.cpp

run: a.out
	@./a.out

clean:
	rm a.out
