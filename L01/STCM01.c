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

//Core idea - to store number as a stuct with key values - number itself, it`s actual number of 64-bit digits
//it`s actual number of bits and it`s sign (if it is important for the operation)
typedef struct {
    uint64_t digits64[max_d_len];   //Memory to store all the values. Mem = 256 bytes
    uint8_t d_length;               //Quantity of digits in such a number (system 2**64). Mem = 1 byte (1 to 32 digits) - how much is actually used
    uint16_t b_length;              //Bit length of such a number. Mem = 2 bytes (1 to 2048 bits)
    bool sign;                      //0 is for positive numbers, 1 is for negative numbers
}int2048_t;

//As we need a function to clear input string, that can be in hex format, we`ll use a table to check if letter sign can or cannot
//be interpreted as hex number - in other cases, we can ignore it so we get clear input data for functions, such as init
typedef struct {
    char letter_sign;
    uint8_t letter_num;
}unhex;

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
    longint->b_length = 0;
    longint->d_length = 0;
    longint->sign = 0;
}

//To print int2048_t. Additional arguments - both bool - adds option to print number of digits and number of bits
void print_int2048(int2048_t* longint, bool d_len, bool b_len){
    printf("\n%c[", longint->sign ? '-' : '+');
    int k = 0;
    if(d_len == 1){
        printf("%llu]",longint->digits64[0]);
    } else {
        for(int i = (longint->d_length - 1); i >= 0; i--){
            printf("%llu", longint->digits64[i]);
            k++;
            (k != longint->d_length) ? printf(",") : printf("]");
            (k % 8 == 0) ? printf("\n") : 0;
        }
    }
    d_len ? printf("\nDigits: %d", longint->d_length) : 0;
    b_len ? printf("\nBits: %d", longint->b_length) : 0;
}

/*A simple functions for reading hex (or <=hex) strings, which clears any other values, except available ones*/

//A simple function to check: can char in input string be a digit of number (in hex or smaller num system)
//If returns -1 then the value can be ignored
int h_check(char digit){
    unhex AV[] = {{'0', 0}, {'1', 1}, {'2', 2}, {'3', 3}, {'4', 4}, 
                {'5', 5}, {'6', 6}, {'7', 7}, {'8', 8}, {'9', 9},
                {'a', 10}, {'b', 11}, {'c', 12}, {'d', 13}, {'e', 14}, {'f', 15},
                {'A', 10}, {'B', 11}, {'C', 12}, {'D', 13}, {'E', 14}, {'F', 15}
    };
    for(int i = 0; i < 22; i++){
        if(digit == AV[i].letter_sign){
            return AV[i].letter_num;
        }
    }
    return -1;
}

//A function to convert string to array of numbers, which can therefore be used for other operations
uint8_t* unstring(char* str, size_t len, int* out_len){
    uint8_t* arr = malloc(len * sizeof(uint8_t));
    int k = 0;
    for(int i = len; i >= 0; i--){
        int buff = h_check(str[i]);
        if(buff >= 0){
            arr[k] = buff;
            k++;
        } else {
            continue;
        }
    }
    arr = realloc(arr, sizeof(int) * k);
    *out_len = k;
    return arr;
}
//Add new number to internal array of int2048_t, change d_length and b_length values
void push_int2048(int2048_t* longint, uint64_t number, bool is_next){
    if(longint->d_length + 1 >= max_d_len){
        printf("Overflow, expected number is bigger than 2048 bit\n");
    }
    if(number == 0){
        longint->d_length++; //addition of zero changes nothing, except the number of ACTIVE values in internal array - we consider this zero as active number
    } else {
        longint->d_length++;
        longint->digits64[longint->d_length - 1] = number;
        if(!is_next) {      //bit length only increased when there will be no next pushed number (for pack function - to calculate it only at once)
            int bl = 0;
            while(number > 0){
                number >>= 1;
                bl++;
            }
            longint->b_length = bl + (bitrate * longint->d_length);
        }
    }
}


//Initialize and pack int2048_t. Needs a string with long number, an int2048_t variable (pointer) and number system as integer
//Number system here have an integer limitation, obviously so - it`s an input, so we basically use binary, decimal or hex here in most cases
//After packing, we`ll have a number with it`s key values as 1 struct variable, which can be used for other functions
void pack_int2048(const char* large_int, int2048_t* longint, int base){
    init_null(longint);
    size_t l1 = strlen(large_int);
    int l2 = 0;
    uint64_t current = 0, next = 0;
    uint8_t* packing = unstring(large_int, l1, &l2);
    for(int i = 0; i < l2; i++){
        //still undone, but getting closer to what i need
    }
}

int main(){
    printf("Maximum value of one digit is %llu\n", limit);
    int2048_t* large_num = malloc(sizeof(int2048_t));
    init_null(large_num);

    print_int2048(large_num, 1, 0);

    large_num->digits64[0] = 16;
    large_num->digits64[1] = 335;
    large_num->d_length++;
    large_num->digits64[2] = 12421873123;
    large_num->d_length++;
    print_int2048(large_num, 0, 0);
    printf("\n");
    char string[] = "abdabcdeX15648515eeefffX01234567";
    int k = 0;
    uint8_t* arr = unstring(string, strlen(string), &k);
    printf("\n");
    for(int i = 0; i < strlen(string); i++){
        printf("%2c", string[i]);
        (i != strlen(string) - 1) ? printf(",") : printf("\n");
    }
    for(int i = 0; i < k; i++){
        printf("%02d", arr[i]);
        (i != k - 1) ? printf(",") : printf("\n");
    }
    return 0;
}