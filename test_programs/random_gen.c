
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
//#include <stdio.h>



int main(int argc, char** argv){

	if (argc < 2){
		srand(0);
	}else{
		srand(atoi(argv[1]));
	}

	int x = 0;
	int n = 1;
	while (x < 1000 && x > -1000){
	
		int r = rand()%1000;
	
		//printf("%d\n",r);
	
		if ( r >= 500){
			x += n;
		}else{
			x -= n;
		}	
	
		//n++;
	}    
	if (x > 0){
		write(STDOUT_FILENO, "Overflow\n", strlen("Overflow\n"));
	}else{
		write(STDOUT_FILENO, "Underflow\n", strlen("Underflow\n"));
	}
	return 0;


}
