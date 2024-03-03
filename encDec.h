#ifndef ENCDEC_H
#define ENCDEC_H

char* read_next_frame_from_input_file(FILE *file);
char* read_from_input_file(char filename[]);
void write_to_output_file(char filename[],char* data);


int binary_to_int(char binary[]); // Function prototype
char* intToBinary(int n);
char binary_to_char_with_parity(char* binary);
char* char_to_binary(char c);

char* add_length_and_syn(char arr_frame[],int length);
char* frame_characters(char* frame, char* ptr);

#endif // ENCDEC_H
