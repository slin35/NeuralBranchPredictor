#include <unistd.h>
#include <string.h>

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





int main() {
    int r[2800 + 1];
    int i, k;
    int b, d;
    int c = 0;
    char num_str[6];
    int len;

    for (i = 0; i < 2800; i++) {
        r[i] = 2000;
    }

    for (k = 2800; k > 0; k -= 14) {
        d = 0;

        i = k;
        for (;;) {
            d += r[i] * 10000;
            b = 2 * i - 1;

            r[i] = d % b;
            d /= b;
            i--;
            if (i == 0) break;
            d *= i;
        }
        

	itoa(c + d / 10000, num_str);
	len = strlen(num_str);
	
	write(STDOUT_FILENO, "0000", 4 - len );
        write(STDOUT_FILENO, num_str, len);
        c = d % 10000;
    }

    return 0;
}
