#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include "encDec.h"



char* read_next_frame_from_input_file(FILE *file_pointer) {
    char* data = (char*) malloc(65); // 64 characters + null terminator
    int bytes_read = fread(data, 1, 64, file_pointer);
    data[bytes_read] = '\0';  // null terminate
    return data;
}

char* read_from_input_file(char filename[]) {
    FILE *file_pointer = fopen(filename, "r");
    //if (!file) return NULL;

    char* data = (char*) malloc(64);  // +1 for the null terminator
    int bytes_read = fread(data, 1,64, file_pointer);
    fclose(file_pointer);
    return data;
}

void write_to_output_file(char filename[],char* data) {
    FILE *file_pointer = fopen(filename, "w");
    if (!file_pointer) return;

    fprintf(file_pointer, "%s", data);
    fclose(file_pointer);
}