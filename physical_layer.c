#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include "encDec.h"




int binary_to_int(char binary[]) {
    int number = 0;
    for (int i = 0; i < 8; i++) {
        if (binary[i] == '1') {
            number += (1 << (7 - i));
        }
    }
    return number;
}

char* intToBinary(int n) {
    char* binary = (char*)malloc(8);
    for (int i = 0; i < 8; i++) {
        
        binary[7 - i] = (n & (1 << i)) ? '1' : '0';
    }
    return binary;
}

char binary_to_char_with_parity(char* binary) {
    if (!binary || strlen(binary) != 8) {
        printf("Error: Invalid binary input.\n");
        return '\0';
    }
    
    int count = 0;
    char character = 0;
    
    
    for (int i = 0; i < 7; i++) {
        if (binary[i] == '1') {
            character |= (1 << (6 - i));
            count++;
        }
    }

    
    
    char expected_parity = (count % 2 == 0) ? '1' : '0';  // Validate parity bit
    if (binary[7] != expected_parity) {
        printf("Warning: Parity bit mismatch!\n");
    }
    
    return character;
    
}

char* char_to_binary(char c)
{
    char* binary = (char*) malloc(8);  // 8 bits + null terminator
    int count = 0;
    for (int i = 6; i >=0; i--) {
        binary[6-i] = (c & (1 << i)) ? '1' : '0';
        if(binary[6-i]=='1') count++;
    }
    binary[7] = (count%2==0)?'1':'0';
    
    return binary;
}