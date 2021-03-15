#include <stdint.h>
#include <math.h>
#include <string.h>
#include <unistd.h>

// s specifies the per-round shift amounts
const uint32_t s[64] = {
    7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,
    5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20,
    4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,
    6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21 };

// binary integer part of the sines of integers (Radians) as constants
// equivalent to for i = 0 : 63 -> k[i] = floor(2^32 * abs((int)sin(i + 1)))
const uint32_t K[64] = {
    0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee, 
    0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
    0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
    0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
    0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
    0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
    0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
    0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
    0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
    0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
    0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
    0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
    0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
    0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
    0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
    0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391 };


#define A0 0x67452301   // A
#define B0 0xefcdab89   // B
#define C0 0x98badcfe   // C
#define D0 0x10325476   // D
#define MSG_SIZE 4 * 262144
#define LEFTROTATE(x, c) (((x) << (c)) | ((x) >> (32 - (c))))    // left rotate

uint32_t variables[] = {A0, B0, C0, D0};


uint32_t string_to_int32(char *msg) {
    return (uint32_t)msg[0] 
        | ((uint32_t)msg[1] << 8) 
        | ((uint32_t)msg[2] << 16) 
        | ((uint32_t)msg[3] << 24);
}


char msg[MSG_SIZE];
void md5(size_t msg_size, char *result) {
    // ------------------preprocessing------------------------------
    size_t size;
    for (size = msg_size; size % (512/8) != 448/8; size++){}
    
    //memcpy(msg, message, msg_size);
    
    // append "1" bit to message 
    // where the first bit is the most significant bit of the byte
    // note: all values are in little-endian, so most significant bit is on the largest address
    msg[msg_size] = 0x80;   // equivalent to 0b10000000
     
    for (int i = msg_size + 1; i < size; i++) {
        msg[i] = 0;         // append "0" bit until message length in bits ≡ 448 (mod 512) 
    }

    // append original length in bits mod 2^64 to message
    uint32_t value = (msg_size * 8) % (uint64_t)pow(2, 64);
    msg[size] = (char)value;
    msg[size + 1] = (char)(value >> 8);
    msg[size + 2] = (char)(value >> 16);
    msg[size + 3] = (char)(value >> 24);

    // ------------------processing--------------------------------------
    // process the message in successive 512-bit chunks:
    uint32_t words[16];


    for (int i = 0; i < size; i += MSG_SIZE/512) {
        // break chunk into sixteen 32-bit words M[j], 0 ≤ j ≤ 15
        for (int j = 0; j < 16; j++) {
            words[j] = string_to_int32(msg + i + j * 4);
        }

        uint32_t A = variables[0];
        uint32_t B = variables[1];
        uint32_t C = variables[2];
        uint32_t D = variables[3];

        for (int k = 0; k < 64; k++) {
            uint32_t F, g;

            if (k < 16) {
                F = (B & C) | ((~B) & D);
                g = k;
            }
            else if (k < 32) {
                F = (D & B) | ((~D) & C);
                g = (5 * k + 1) % 16;
            }
            else if (k < 48) {
                F = B ^ C ^ D;
                g = (3 * k) % 16;
            }
            else {
                F = C ^ (B | (~D));
                g = (7 * k) % 16;
            }

            F = F + A + K[k] + words[g];
            A = D;
            D = C;
            C = B;
            B = B + LEFTROTATE(F, s[i]);
        }

        variables[0] += A;
        variables[1] += B;
        variables[2] += C;
        variables[3] += D;

    }

    // a0 append b0 append c0 append c0 for digest
    for (int i = 0; i < 16; i += 4) {
        uint32_t value = variables[i/4];

        result[i] = (char)value;
        result[i + 1] = (char)(value >> 8);
        result[i + 2] = (char)(value >> 16);
        result[i + 3] = (char)(value >> 24);
    }

}


int main() {
    //char message[] = "The quick brown fox jumps over the lazy dog";
    char result[16];

    //int msg_size = strlen(message);

    int msg_size = 0;
    
    int num_bytes;

    while ( (num_bytes = read(STDIN_FILENO, msg, 512)) != 0 ){
        msg_size += num_bytes;
    };

    md5(msg_size, result);
    
    write(STDOUT_FILENO, result, 16);

    return 0;
}
