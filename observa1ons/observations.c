#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct{
    uint8_t number;
    char symbol;
} hex_t;

hex_t hectic[] = {{0,'0'} , {1,'1'} , {2,'2'} , {3,'3'} , {4,'4'},
                    {5,'5'} , {6,'6'} , {7,'7'} , {8,'8'} , {9,'9'},
                    {10,'a'} , {11,'b'} , {12,'c'} , {13,'d'} , {14,'e'},
                    {15,'f'}
};

int numbar(char c){
    for(int i = 0; i < 16; i++){
        if(hectic[i].symbol == c){
            return hectic[i].number;
        }
    }
    return 0;
}

int pack__hex(char* str, size_t len){
    int k = 0, r = 0;
    for(int i = (len - 1); i >= 0; i--){
        r += numbar(str[i]) * pow(16, k);
        k++;
    }
    return r;
}

int main(){
    int x = 0xffe;
    char* f = "0xffe";
    printf("%s\n", f);
    for(int i = 0; i < strlen(f); i++){
        printf("%d\n", f[i]);
    }
    int k = strlen(f);
    int d = pack__hex(f, k);
    printf("\n%d", d);

    return 0;
}