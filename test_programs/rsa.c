#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <stdint.h>
#include <string.h>
#include <gmp.h>
#include <unistd.h>


uint64_t encrypt(uint64_t m, uint64_t e, uint64_t n){
	
	uint64_t accul = 1;
	
	for (int i = 0; i < e; i++){
		accul *= m;
	}
	return accul %= n;
}

#define BASE_CHAR 'A'
#define BUFFER_LEN 500000000
uint64_t msg_encrypted[BUFFER_LEN];
unsigned char msg_decrypted[BUFFER_LEN];
int main(){
	
	int p = 2;
	int q = 7;
	int n = p * q;
	
	int e = 5;
	int d = 11;
	
	//char* msg = "Hello World";
	//int msg_len = strlen(msg);
	//printf("Message: %s\n", msg );


	//printf("In Character: %c\n", msg[0]);
	
/*	for (int i = 0; i < msg_len; i++){
		msg_encrypted[i] = encrypt(msg[i],e,n);
	}*/
	int msg_len = 0;
	char ch;
	while( read(STDIN_FILENO, &ch, 1) != 0  ){
		msg_encrypted[msg_len++] = encrypt( (ch-BASE_CHAR), e, n);
	}
	//msg_len--;
	
	/*for (int i = 0; i < msg_len; i++){
		printf("%c", msg_encrypted[i] + BASE_CHAR);
	}
	printf("\n");*/
	
	/*
	for (int i = 0; i < msg_len; i++){
		msg_decrypted[i] = (char) decrypt(msg_encrypted[i],d,n);
	}*/
	for (int i = 0; i < msg_len; i++){
		msg_decrypted[i] = (unsigned char) encrypt(msg_encrypted[i],d,n) + BASE_CHAR;
	}
	//msg_decrypted[msg_len++] = '\n';
	
	write(STDOUT_FILENO, msg_decrypted, msg_len);
	
	//printf("Decrypted Msg: %s\n", msg_decrypted );
	
	
	return 0;
	
	
}

