#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <locale.h>
#include <stdint.h>
#include <inttypes.h>
#include <string.h>
#define bitrate 64
#define limit 18446744073709551615ULL
#define max_d_len 32

//Core idea - to store number as a stuct with key values - number itself ()
typedef struct {
    uint64_t digits64[max_d_len];   //Memory to store all the values. Mem = 256 bytes
    uint8_t d_length;               //Quantity of digits in such a number (system 2**64). Mem = 1 byte (1 to 32 digits) - how much is actually used
    uint16_t b_length;              //Bit length of such a number. Mem = 2 bytes (1 to 2048 bits)
    bool sign;                      //0 is for positive numbers, 1 is for negative numbers
}int2048_t;

/*98.8% of total memory used is a number itself, we use additional few percents to store key characteristics of number which we can use with ease*/
/*Numbers will be passed to func as strings (array of char`s) - then it must be converted to int2048_t via procedure
Init function here will be important (or even necessary) - it will actually take the string, calculate additional values (d_len / b_len) and create a number
with fixed length*/
/*For each actual digit whe can add numbers from string WHILE they don`t reach limit, after we move to the next digit and look what we have there
So the paradigm is obvious - first of all, we need to init a big number int2048_t*/

//init with zero overall value (sign is + for zero for obvious reasons)
void init_null(int2048_t* longint){
    for(int i = 0; i < max_d_len; i++){
        longint->digits64[i] = 0;
    }
    longint->b_length = 1;
    longint->d_length = 1;
    longint->sign = 0;
}

//Initialize and pack int2048_t. Needs a string with long number, an int2048_t variable (pointer) and number system as integer
//Number system here have an integer limitation, obviously so - it`s an input, so we basically use binary, decimal or hex here in most cases
//After packing, we`ll have a number with it`s key values as 1 struct variable, which can be used for other functions
void pack_int2048(const unsigned char* large_int, int2048_t* longint, int base){
    init_null(longint);
    if(strlen(large_int) == 1){
        longint->digits64[0] = large_int[0];
        longint->d_length = 1;
        longint->b_length = (large_int[0]>>1 == 0) ? 1 :
                            (large_int[0]>>2 == 0) ? 2 :
                            (large_int[0]>>3 == 0) ? 3 : 4;
            return;
    }
    int k = 0, q = 0;
    int current = 0;
    for(int i = strlen(large_int); i >= 0; i--){
        current += large_int[i] * pow(10, k);
        k++;
    }
}

int main(){
    printf("Maximum value of one digit is %llu\n", limit);
    int2048_t* large_num = malloc(sizeof(int2048_t));
    init_null(large_num);

    for(int i = 0; i < max_d_len; i++){
        printf("{%llu}\n", large_num->digits64[i]);
    }
    printf("Digits - %d, bits - %d, sign %c", large_num->d_length, large_num->b_length, (large_num->sign == 1) ? '-' : '+');

    return 0;
}