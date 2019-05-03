#include "test_main.h"

struct directory *initialize_directory(void)
{
    struct directory *directorybufferfer = malloc(sizeof(*directorybufferfer));
    for (int i = 0; i < 8; i++)
    {
        strcpy(directorybufferfer->entry[i].filename, "");
        strcpy(directorybufferfer->entry[i].extension, "");
        directorybufferfer->entry[i].fileID = -1;
        directorybufferfer->entry[i].dirorfile = -1;
        directorybufferfer->entry[i].time = (time_t)NULL;
    }
    return directorybufferfer;
}
//checks is the fat is empty
long is_fat_empty(void)
{
    long i = 1;
    while (i < NUM_BLOCKS)
    {
        if (fatpointer->file_allocation_table[i] == -1)
            return i;
        i++;
    }
    return 0;
}

//check if the block entry is empty
int is_entry_empty(struct directory *dirpointer)
{
    int i;
    for (i = 0; i < ENTRY; i++)
    {
        if (dirpointer->entry[i].fileID == -1)
            return i;
    }
    return -1;
}

int is_entry_free_a(long directory_index)
{
    struct directory *dirpointer = malloc(sizeof(*dirpointer));
    memcpy(dirpointer, &blocks[directory_index], sizeof(*dirpointer));
    int i;
    for (i = 0; i < ENTRY; i++)
    {
        if (dirpointer->entry[i].fileID == -1)
            return i;
    }
    return -1;
}

int file_location_entry(char *filename, char *extension, struct directory *dirpointer)
{
    int i;
    for (i = 0; i < ENTRY; i++)
    {
        if (!strcmp(dirpointer->entry[i].filename, filename) && !strcmp(dirpointer->entry[i].extension, extension))
        {
            if (!dirpointer->entry[i].dirorfile)
            {
                printf("%s.%s is a file\n", filename, extension);
                return i;
            }
            else
            {
                printf("%s.%s is a directory\n", filename, extension);
                return i;
            }
        }
    }
    return -1;
}

int file_location_entry_a(char *filename, char *extension, long directory_index)
{
    struct directory *dirpointer = malloc(sizeof(*dirpointer));
    memcpy(dirpointer, &blocks[directory_index], sizeof(*dirpointer));

    int i;
    for (i = 0; i < ENTRY; i++)
    {
        if (!strcmp(dirpointer->entry[i].filename, filename) && !strcmp(dirpointer->entry[i].extension, extension))
        {
            if (!dirpointer->entry[i].dirorfile)
            {
                printf("%s.%s is a file\n", filename, extension);
                return i;
            }
            else
            {
                printf("%s.%s is a directory\n", filename, extension);
                return i;
            }
        }
    }
    return -1;
}

char **get_dir(char *path_t)
{
    char *filepath = malloc(sizeof(filepath));
    strcpy(filepath, path_t);
    int i = 0;
    char **buffer = malloc(64 * sizeof(char *));
    if ((buffer[0] = strtok(filepath, "/")) == NULL)
    {
        printf("in get_dir, srtok() failed\n");
        return NULL;
    }
    while ((buffer[++i] = strtok(NULL, "/")) != NULL)
        ;
    return buffer;
}

long get_directory_location(char *filepath)
{
  
    printf("get_directory_location(%s)\n", filepath);
    char **folders = malloc(64 * sizeof(char *));

    struct directory *dirpointer = malloc(sizeof(*dirpointer));

    long index_indisk = 0;
    int location_in_directory;
    char *folder = malloc(32);
    int n;
    folders = get_dir(filepath);
    int i = 0;
    while ((folder = folders[i]) != NULL)
    {

        memcpy(dirpointer, &blocks[index_indisk], sizeof(*dirpointer));

        if ((location_in_directory = file_location_entry(folder, " ", dirpointer)) == -1)
        {
            return -1;
        }

        print_file_meta(&dirpointer->entry[location_in_directory]);

        n = dirpointer->entry[location_in_directory].dirorfile;

        if (!n || n == -1)
        {
            printf("filepath: %s notfound; %s is not a file or directory\n", filepath, folder);
            return -1;
        }

        index_indisk = dirpointer->entry[location_in_directory].fileID;

        i++;
    }

    return index_indisk;
}

void print_file_meta(metadata *filepointer)
{

    printf("file %s.%s\t", filepointer->filename, filepointer->extension);
    printf("file location: %ld\t dirorfile: %d\t", filepointer->fileID, filepointer->dirorfile);
    printf("time modified: %ld\n", filepointer->time);
}

void print_directory(char *filepath)
{
    long directory_index = get_directory_location(filepath);

    print_directory_a(directory_index);
}

void print_directory_a(long directory_index)
{

    struct directory *dirpointer = malloc(sizeof(*dirpointer));
    memcpy(dirpointer, &blocks[directory_index], sizeof(*dirpointer));

    for (int i = 0; i < ENTRY; i++)
    {
        printf("file %d: filename: %s.%s\t", i, dirpointer->entry[i].filename, dirpointer->entry[i].extension);
        printf("fileID: %ld\t", dirpointer->entry[i].fileID);
        printf("dirorfile: %d\n", dirpointer->entry[i].dirorfile);
        printf("time modified: %ld\n", dirpointer->entry[i].time);
    }
    return;
}
