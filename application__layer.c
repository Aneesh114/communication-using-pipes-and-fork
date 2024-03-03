void write_to_output_file(char filename*,char* data) {
    FILE *file_pointer = fopen(filename, "w");
    if (!file_pointer) return;

    fprintf(file_pointer, "%s", data);
    fclose(file_pointer);
}

int main(int argc,char* argv[]){
    char* data = argv[2];
    char* filename = argv[1];
    write_to_output_file(filename,data);
    return 0;
}