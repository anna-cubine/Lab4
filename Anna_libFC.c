#include "Anna_libFC.h"
//Global variables
FileEntry file_table[MAX_FILES];
int file_count = 0;

//Create a new file
int fileCreate(const char *filename){
    //Check if file already exists
    for (int i = 0; i< file_count; i++){
        if(strcmp(file_table[i].filename,filename) == 0){
            printf("Error: File '%s' already exists.\n", filename);
            return -1;
        }
    }

    //Create the file
    FILE *file = fopen(filename, "w");
    if (!file){
        printf("Error: undable to create file '%s'.\n", filename);
        return -1;
    }
    fclose(file);

    //Add to file table
    strcpy(file_table[file_count].filename, filename);
    file_table[file_count].size = 0;
    file_table[file_count].is_open = 0; //File is closed
    file_count++;

    printf("File '%s' created successfully.\n",filename);
    return 0;
}

int fileOpen(const char *filename){
    for (int i = 0; i < file_count; i++){
        if (strcmp(file_table[i].filename, filename) == 0){
            if (file_table[i].is_open){
                printf("Error: file '%s' is already open.\n",filename);
                return -1;
            }
            file_table[i].is_open = 1; //Mark file as open
            printf("File '%s' opened successfully.\n",filename);
            return i; //Returning file index
        }
    }

    printf("Error: file '%s' not found.\n", filename);
    return -1;
}

int fileWrite(int file_index, const char *data){
    if (!file_table[file_index].is_open){
        printf("Error: file '%s' is not open.\n",file_table[file_index].filename);
        return -1;
    }
    
    int data_size = strlen(data);

    //Write data to the file
    FILE *file = fopen(file_table[file_index].filename, "w");
    if (!file){
        printf("Error: Unable to open file '%s' for writing.\n", file_table[file_index].filename);
        return -1;
    }
    fwrite(data,1,data_size,file);
    fclose(file);

    file_table[file_index].size = data_size;
    printf("Data written to file '%s' successfullyi.\n",file_table[file_index].filename);
    return 0;
}

int fileRead(int file_index, char *buffer, int buffer_size){
    //Checking for valid file index
    if (file_index < 0 || file_index >= file_count){
        printf("Error : Invalid file index or file not open");
        return -1;
    }

    //Openning file in read
    FILE *file = fopen(file_table[file_index].filename, "r");
    if(!file){
        printf("Error: Unable to open file '%s' for writing.\n", file_table[file_index].filename);
        return -1;
    }

    size_t bytesRead = fread(buffer,1,buffer_size-1,file);
    buffer[bytesRead] = '\0'; //Null terminating buffer
    fclose(file);

    printf("File content:\n%s\n",buffer);
    return 0;
}

int fileClose(int file_index){
    if (file_index < 0 || file_index >= file_count || !file_table[file_index].is_open){
        printf("Error: Invalid file index or file is already closed.\n");
        return -1;
    }

    file_table[file_index].is_open = 0;
    printf("File '%s' closed successfully.\n",file_table[file_index].filename);
    return 0;
}

int fileDelete(const char *filename){
    for (int i=0; i < file_count; i++){
        if (strcmp(file_table[i].filename,filename) == 0){
            if (file_table[i].is_open){
                printf("Error: file '%s' is open.\n",filename);
                return -1;
            }

            if(remove(filename) == 0){
                printf("File '%s' deleted successfully.\n",filename);
                //Removing from the table
                for (int j = i; j < file_count-1; j++){
                    file_table[j] = file_table[j + 1];
                }
                file_count--;
                return 0;
            } else {
                printf("Error: Unable to delete file '%s'.\n",filename);
                return -1;
            }
        }
    }
}