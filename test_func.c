#include "test_main.h"

void f_create(char *filepath, char *filename, char *extension, int dirorfile)
{
    long dir_index; //parent index to new file 

    if (filepath == NULL)
    {
        dir_index = 0;
    }
    else
    {
        dir_index = get_directory_location(filepath);
    }

    struct directory *dirpointer = malloc(sizeof(*dirpointer));
    memcpy(dirpointer, &blocks[dir_index], sizeof(*dirpointer)); //cpy the dir at direcotyr location in to the pointer 

    int index_indir; //index in directory 
    if ((index_indir = is_entry_empty(dirpointer)) == -1)
    {
        printf("the directory was full\n");
        return;
    }

    long index; //index of new file 
    if (!(index = is_fat_empty()))
    {
        printf("the disk is full!\ndelete a file or directory to continue\n");
        return;
    }
    //init the dir entry with metadata
    strcpy(dirpointer->entry[index_indir].filename, filename);
    strcpy(dirpointer->entry[index_indir].extension, extension);
    dirpointer->entry[index_indir].fileID = index;
    dirpointer->entry[index_indir].dirorfile = dirorfile;
    dirpointer->entry[index_indir].time = time(0);

    memcpy(&blocks[dir_index], dirpointer, sizeof(*dirpointer)); //cpy the pointer into the dir location

    free(dirpointer);
    fatpointer->file_allocation_table[index] = 0; //set fat table for the current file to 0 meaning end of file 
    if (dirorfile == 1)
    {

        struct directory *create_directory = initialize_directory();
        memcpy(&blocks[index], create_directory, sizeof(*create_directory));
        free(create_directory);
    }
}
/* the open funcition opens the file looks for filepath, fiilename, and extension
*/
metadata *f_open(char *filepath, char *filename, char *extension)
{

    printf("f_open(%s, %s, %s)\n", filepath, filename, extension);
    long index_indisk;    //block[] index
    if (filepath == NULL) //if the file is in root set the location to 0th index in the drive
                          //else get the init the dir path
    {
        index_indisk = 0;
    }
    else
    {

        index_indisk = get_directory_location(filepath);
    }

    //allocate
    struct directory *dirpointer = malloc(sizeof(*dirpointer));
    //open dir
    memcpy(dirpointer, &blocks[index_indisk], sizeof(*dirpointer));
    //find index of name.extension in the directory
    int index_indir = file_location_entry(filename, extension, dirpointer);
    //if -1 its empty
    if (index_indir == -1)
    {
        return NULL;
    }

    printf("opening file:\n");
    print_file_meta(&dirpointer->entry[index_indir]);

    return &dirpointer->entry[index_indir]; //returning poitner to the entry
}

/*
*/
void f_delete(char *filepath, char *filename, char *extension)
{
    //find the file that is given at the end of the file path
    long index_indisk;
    if (filepath == NULL)
    {
        index_indisk = 0;
    }
    else
        index_indisk = get_directory_location(filepath);
    //allocate
    struct directory *dirpointer = malloc(sizeof(*dirpointer));

    memcpy(dirpointer, &blocks[index_indisk], sizeof(*dirpointer));

    int index_indir; //entry index
    //find entry that matches name.extension
    index_indir = file_location_entry(filename, extension, dirpointer);
    if (index_indir == -1)
    {
        printf("could NOT find %s.%s in %s\n", filename, extension, filepath);
        return;
    }



    long fat = dirpointer->entry[index_indir].fileID;
    if (!(dirpointer->entry[index_indir].dirorfile))
    {
        do
        {

            fatpointer->file_allocation_table[fat] = -1;

        } while ((fat = fatpointer->file_allocation_table[fat]) && fat != -1);
    }

    else

        fatpointer->file_allocation_table[fat] = -1;

    strcpy(dirpointer->entry[index_indir].filename, "");
    strcpy(dirpointer->entry[index_indir].extension, "");
    dirpointer->entry[index_indir].fileID = -1;
    dirpointer->entry[index_indir].dirorfile = -1;
    dirpointer->entry[index_indir].time = (time_t)NULL;

    memcpy(&blocks[index_indisk], dirpointer, sizeof(*dirpointer));

    printf("%s%s.%s deleted\n", filepath, filename, extension);
}

void f_write(metadata *fp, char *buffer)
{

    int flag = 0;

    long fat = fp->fileID;

    long var;

    if (fatpointer->file_allocation_table[fat] != -1)
    {

        while ((var = fatpointer->file_allocation_table[fat]) != 0)
            fat = var;
    }

    block temporary;

    memcpy(temporary, &blocks[fat], BLOCK_SIZE);

    int i = 0;
    while (temporary[i] != '\0')
        i++;

    int j = 0;

    for (; i < BLOCK_SIZE; i++)
    {
        if (buffer[j] != '\0')
        {

            temporary[i] = buffer[j];

            j++;
        }
        else
        {
            printf("flag = 1\n");
            flag = 1;
            break;
        }
    }

    memcpy(&blocks[fat], temporary, BLOCK_SIZE);

    if (!flag)
    {

        long next = is_fat_empty();

        printf("calling f_writing(%ld, %s)\n", next, buffer + j);

        fatpointer->file_allocation_table[fat] = next;
        fatpointer->file_allocation_table[next] = 0;

        f_writing(next, buffer + j);
    }
}

void f_writing(long fat, char *buffer)
{
    int flag = 0;
    long next;

    block temporary;
    memcpy(temporary, &blocks[fat], BLOCK_SIZE);

    for (int i = 0; i < BLOCK_SIZE; i++)
    {
        if (buffer[i] != '\0')
        {
            temporary[i] = buffer[i];
        }
        else
        {

            printf("flag = 1\n");
            flag = 1;
            break;
        }
    }

    memcpy(&blocks[fat], temporary, BLOCK_SIZE);

    if (!flag)
    {
        next = is_fat_empty();
        fatpointer->file_allocation_table[fat] = next;
        fatpointer->file_allocation_table[next] = 0;
        return f_writing(next, buffer + BLOCK_SIZE);
    }
}

void f_read(metadata *fp)
{
    block temporary;
    char *buffer = malloc(8);
    long fat = fp->fileID;

    do
    {
        memcpy(temporary, &blocks[fat], BLOCK_SIZE);
        strcat(buffer, temporary);
    } while ((fat = fatpointer->file_allocation_table[fat]) != 0);
    printf("%s\n", buffer);
}
