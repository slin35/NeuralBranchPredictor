
CXX = gcc

FLAGS = -static

exec = fib rand prog rsa mzip

all : fib rand prog rsa mzip


fib : fib.c
	$(CXX) $(FLAGS) -o fib fib.c

rand : random_gen.c
	$(CXX) $(FLAGS) -o rand random_gen.c
	
prog : prog.c
	$(CXX) $(FLAGS) -o prog prog.c
	
rsa : rsa.c
	$(CXX) $(FLAGS) -o rsa rsa.c libgmp.a
	
mzip : minigzip.c
	$(CXX) $(FLAGS) -o mzip minigzip.c libz.a
	
clean:
	rm $(exec)
