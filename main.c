#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include "encDec.h"

char* uppercase_transform(char c[]) //Function to transform an array of letters to uppercase.
{
    for (int i = 0; c[i]; i++) {
        if (c[i] >= 'a' && c[i] <= 'z') {
            c[i] -= 'a' - 'A';
        }
    }
    return c;
}



int main()
{
    
    int pipe_fd1[2], pipe_fd2[2];
    pid_t pid;
    char* decoded_output = NULL;  // Final output (uppercase decoded by produced) is stored here
    char* syn_frame_output = NULL;  // Encoded version of input data sent by producer is here
    char* uppercase_accumulator = NULL;  // Uppercase data transformed by consumer is here
    char* full_frame_accumulator = NULL; // Encoded version of uppercase data is here




    // Create pipes
    if (pipe(pipe_fd1) == -1 || pipe(pipe_fd2) == -1) {
        perror("Failed to create pipes");
        return 1;
    }
    // Create fork
    pid = fork();
    if (pid == -1) {
        perror("Fork failed");
        return 1;
    }

    if(pid>0) //producer
    {

        close(pipe_fd1[0]); //read of pipe 1 closed
        close(pipe_fd2[1]); // write of pipe 2 closed
        FILE *file = fopen("filename.inpf","r");
        if(!file){
            perror("Failed to open input file");
            exit(1);
        }
        
        while(!feof(file)){ // loop to take input data frame by frame each of length 64 characters
            char* ptr;
            ptr = read_next_frame_from_input_file(file);
            if(strlen(ptr)==0){
                free(ptr);
                break;
            }
             int required_size = 8 * strlen(ptr);
             char arr_frame[required_size];
             
            
            char* frame = (char*)malloc(required_size); 
           
            frame = frame_characters(frame,ptr); // binary characters frame
            //printf("%s",frame);
            strcpy(arr_frame,frame);     
            char* syn_frame = (char*)malloc(required_size+24);
            syn_frame = add_length_and_syn(arr_frame,strlen(ptr)); //encoded version of input data
            write(pipe_fd1[1], syn_frame, strlen(syn_frame)); // writing encoded input data to pipe 1
            if (syn_frame_output == NULL) {
            syn_frame_output = strdup(syn_frame);
            } else 
            {
                char* temp = malloc(strlen(syn_frame_output) + strlen(syn_frame) + 1);
                strcpy(temp, syn_frame_output);
                strcat(temp, syn_frame);
                free(syn_frame_output);
                syn_frame_output = temp;
            }   
            char* buffer_prod = (char*)malloc(strlen(syn_frame));
            read(pipe_fd2[0], buffer_prod,strlen(syn_frame)); // reading encoded output data from pipe 2
            char decoded_str[strlen(ptr)+1];
            int decode_idx = 0;
            for (int i = 24; i < strlen(syn_frame); i += 8) {
                char temp[9];
                strncpy(temp, &buffer_prod[i], 8);
                temp[8] = '\0';
                char decoded_char = binary_to_char_with_parity(temp);
                decoded_str[decode_idx++] = decoded_char;
                } 
            decoded_str[strlen(ptr)] = '\0';
            //printf("%s",decoded_str);
           if (decoded_output == NULL) {
                decoded_output = strdup(decoded_str);
            } 
            else 
            {
                char* temp = malloc(strlen(decoded_output) + strlen(decoded_str) + 1);
                strcpy(temp, decoded_output);
                strcat(temp, decoded_str);
                free(decoded_output);
                decoded_output = temp;
            }   
        }
        write_to_output_file("filename.done", decoded_output); // writing final output to a file
        free(decoded_output);
        fclose(file); 
        execl("./application__layer","application__layer","filename.binf",syn_frame_output,NULL);
        free(syn_frame_output);
           
    }
    else //consumer
    {
        close(pipe_fd1[1]); // closing write of pipe 1
        close(pipe_fd2[0]); // closing read of pipe 2
        while(1){
            char* buffer_cons = (char*)malloc(536);
            read(pipe_fd1[0],buffer_cons,24); // reading syn and length data from pipe 1
            char frame_length[8];
            strncpy(frame_length, buffer_cons+16, 8);
            int num_chars = binary_to_int(frame_length);
            int num_bits = num_chars * 8;

            
            if(num_chars==0) break;
            read(pipe_fd1[0], buffer_cons + 24, num_bits); // reading characters in binary form from pipe 1
            char decoded_str[num_bits+1]; 
            int decode_idx = 0;


            for (int i = 24; i < 24+num_bits; i += 8) {
                char temp[9];
                strncpy(temp, &buffer_cons[i], 8);
                temp[8] = '\0';
                char decoded_char = binary_to_char_with_parity(temp); // converting binary with parity to character
                decoded_str[decode_idx++] = decoded_char;
            }       
            decoded_str[decode_idx] = '\0'; 
            char* upper_str = uppercase_transform(decoded_str); // transforming to uppercase
            if (uppercase_accumulator == NULL) {
                uppercase_accumulator = strdup(upper_str);
            } 
            else 
            {
                char* temp = malloc(strlen(uppercase_accumulator) + strlen(upper_str) + 1);
                strcpy(temp, uppercase_accumulator);
                strcat(temp, upper_str);
                free(uppercase_accumulator);
                uppercase_accumulator = temp;
            }
            
            char* uppercase_msg = (char*)malloc(512);
            int j=0;
            for (int i = 0; i < strlen(upper_str); i++) {
                char* encoded_upper = char_to_binary(upper_str[i]); // encoding uppercase
                for (int k = 0; k < 8; k++) {
                    uppercase_msg[j++] = encoded_upper[k];
                }
                free(encoded_upper);
            }       
 
            char* full_frame = add_length_and_syn(uppercase_msg, strlen(upper_str));
            
            //printf("Encoded uppercase: %s\n",full_frame);
            if (full_frame_accumulator == NULL) {
                full_frame_accumulator = strdup(full_frame);
                } 
            else
                {
                    char* temp = malloc(strlen(full_frame_accumulator) + strlen(full_frame) + 1);
                    strcpy(temp, full_frame_accumulator);
                    strcat(temp, full_frame);
                    free(full_frame_accumulator);
                    full_frame_accumulator = temp;
                }
            write(pipe_fd2[1], full_frame, strlen(full_frame)); // writing uppercase encoded data to pipe 2
        
        }
    }
    write_to_output_file("filename.outf", uppercase_accumulator); // writing uppercase data transformed by consumer to a file
    free(uppercase_accumulator);
    //printf("about to print accumulator");
    execl("./application__layer","application__layer","filename.chck",full_frame_accumulator,NULL); // writing encoded version of uppercase data to a file
    write_to_output_file("filename.chck", full_frame_accumulator); 
    free(full_frame_accumulator);

    
    return 0;

}