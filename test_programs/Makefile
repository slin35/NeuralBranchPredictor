
CXX = gcc

FLAGS = -static

exec = fib rand prog rsa mzip pi md5

all : fib rand prog rsa mzip pi md5


fib : fib.c
	$(CXX) $(FLAGS) -o fib fib.c

rand : random_gen.c
	$(CXX) $(FLAGS) -o rand random_gen.c
	
prog : prog.c
	$(CXX) $(FLAGS) -o prog prog.c
	
rsa : rsa.c
	$(CXX) $(FLAGS) -o rsa rsa.c
	
mzip : minigzip.c
	$(CXX) $(FLAGS) -o mzip minigzip.c libz.a
	
pi : pi.c
	$(CXX) $(FLAGS) -o pi pi.c
	
md5 : md5.c
	$(CXX) $(FLAGS) -mcmodel=medium -o md5 md5.c 
	
clean:
	rm $(exec)
