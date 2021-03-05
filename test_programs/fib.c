
#include <unistd.h>
#include <string.h>
#include <stdlib.h>




 /* reverse:  reverse string s in place */
 void reverse(char s[]){
     int i, j;
     char c;

     for (i = 0, j = strlen(s)-1; i<j; i++, j--) {
         c = s[i];
         s[i] = s[j];
         s[j] = c;
     }
}  

/* itoa:  convert n to characters in s */
 void itoa(int n, char s[])
 {
     int i, sign;

     if ((sign = n) < 0)  /* record sign */
         n = -n;          /* make n positive */
     i = 0;
     do {       /* generate digits in reverse order */
         s[i++] = n % 10 + '0';   /* get next digit */
     } while ((n /= 10) > 0);     /* delete it */
     if (sign < 0)
         s[i++] = '-';
     s[i] = '\0';
     reverse(s);
}  



int main(int argc, char** argv) {

    int t1 = 0;
    int t2 = 1;
    int nextTerm;
	int n = 50;
    
    if (argc > 1){
    	n = atoi(argv[1]);
    }
   
    char x[100];

    for (int i = 1; i <= n; ++i) {
    
    	itoa(t1,x);
        write(STDOUT_FILENO, x, strlen(x));
        write(STDOUT_FILENO, " ", 1);
        
        nextTerm = t1 + t2;
        t1 = t2;
        t2 = nextTerm;
    }

	write(STDOUT_FILENO, "\n", 1);

    return 0;
}
