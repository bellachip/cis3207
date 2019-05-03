#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <time.h>

#define METASIZE 64
#define BLOCK_SIZE 512
#define FILE_SIZE 2101248
#define ENTRY 8
#define NUM_BLOCKS 4096

typedef char block[BLOCK_SIZE];

typedef struct
{
    char filename[36];
    char extension[3];
    long fileID;
    int dirorfile;
    time_t time;
} metadata;

typedef struct
{
    long file_allocation_table[NUM_BLOCKS];
} FAT;
//directory 
struct directory
{
    metadata entry[BLOCK_SIZE / METASIZE];
};
//global pointers
FAT *fatpointer;
block *blocks;

//test_util.c
struct directory *initialize_directory(void);
long is_fat_empty(void);
int is_entry_empty(struct directory *);
int is_entry_free_a(long dir_loc);
int file_location_entry(char *filename, char *extension, struct directory *dirp);
int file_location_entry_a(char *filename, char *extension, long dir_loc);
char **get_dir(char *path_t);
long get_directory_location(char *path);
void print_file_meta(metadata *filepointer);
void print_directory(char *path);
void print_directory_a(long dir_loc);

//test_func.c
metadata *f_open(char *path, char *filename, char *extension);
void f_create(char *path, char *filename, char *extension, int dirorfile);
void f_delete(char *path, char *filename, char *extension);
void f_write(metadata *filepointer, char *buf);
void f_writing(long fat, char *buf);
void f_read(metadata *filepointer);

//test_test.c
void f_test(void);
