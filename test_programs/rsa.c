#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <stdint.h>
#include <string.h>
#include <gmp.h>
#include <unistd.h>


uint64_t encrypt(uint64_t m, uint64_t e, uint64_t n){
	
	/* 1. Initialize the number */
	mpz_t accul;
	mpz_init(accul);
	mpz_set_ui(accul,m);
	
	mpz_pow_ui(accul,accul,e);
	

	
	mpz_mod_ui(accul, accul, n);
	
	//mpz_out_str(stdout, 10, accul);

	uint64_t result = mpz_get_ui(accul);

	mpz_clear(accul);

	return result;
	
	
}

uint64_t decrypt(uint64_t value, uint64_t d, uint64_t n){
	
	mpz_t accul;
	mpz_init(accul);
	mpz_set_ui(accul,value);
	
	mpz_pow_ui(accul,accul,d);

	
	mpz_mod_ui(accul, accul, n);
	
	//mpz_out_str(stdout, 10, accul);

	uint64_t result = mpz_get_ui(accul);

	mpz_clear(accul);

	return result;
	

}


#define BUFFER_LEN 16777216
uint64_t msg_encrypted[BUFFER_LEN];
unsigned char msg_decrypted[BUFFER_LEN];
int main(){
	
	int p = 61;
	int q = 53;
	int n = p * q;
	
	int e = 17;
	int d = 2753;
	
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
		msg_encrypted[msg_len++] = encrypt(ch,e,n);
	}
	
	
	//printf("Cipher: %d\n", cipher);

	/*
	for (int i = 0; i < msg_len; i++){
		msg_decrypted[i] = (char) decrypt(msg_encrypted[i],d,n);
	}*/
	for (int i = 0; i < msg_len; i++){
		msg_decrypted[i] = (unsigned char) decrypt(msg_encrypted[i],d,n);
	}
	msg_decrypted[msg_len] = '\0';
	
	write(STDOUT_FILENO, msg_decrypted, msg_len);
	
	//printf("Decrypted Msg: %s\n", msg_decrypted );
	
	
	return 0;
	
	
}

