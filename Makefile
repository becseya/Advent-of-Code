
ifndef T
T := $(shell find -name main.cpp | sort | tail -n 1)
endif

DIR_T := $(shell dirname ${T})

all: a.out

a.out: ${T}
	clang-format -i ${T}
	g++ ${T}

run: a.out
	@cat ${DIR_T}/input.txt | ./a.out

clean:
	rm a.out
