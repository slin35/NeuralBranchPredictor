
CXX = gcc

FLAGS = -static


all : fib rand prog


fib : fib.c
	$(CXX) $(FLAGS) -o fib fib.c

rand : random_gen.c
	$(CXX) $(FLAGS) -o rand random_gen.c
	
prog : prog.c
	$(CXX) $(FLAGS) -o prog prog.c
