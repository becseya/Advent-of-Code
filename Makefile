
ifndef T
T := $(shell find -name main.cpp | sort | tail -n 1)
endif

all: a.out

a.out: ${T}
	clang-format -i ${T}
	g++ ${T}

run: a.out
	@./a.out

clean:
	rm a.out
