#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include "encDec.h"


char* add_length_and_syn(char arr_frame[],int length){
    char* g = (char*) malloc(length*8+24+1);  
    char k[16] = "0001011000010110";
    char* p = (char*)malloc(8);
    p = intToBinary(length);
   
    for(int i = 0;i<16;i++){
        g[i] = k[i];
    }
    for(int i =0;i<8;i++){
        g[i+16] = p[i];
    }
    for(int i = 0;i<length*8;i++){
        g[i+24] = arr_frame[i];
    }
    g[length*8+24] = '\0';
   
    return g;

}

char* frame_characters(char* frame, char* ptr){
    int j = 0; 
    for (int i = 0; i < strlen(ptr); i++) {
        char* l = char_to_binary(ptr[i]);
       
        for (int k = 0; k < 8; k++) {
            frame[j++] = l[k];
        }
    }
    return frame;
}