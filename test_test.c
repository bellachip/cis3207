#include "test_main.h"

void f_test(void)
{
    printf("this is the officially TESTING of the file system. The mthodology of testing the filesystem is to call my builtin functions, manually passing in the filepath\n ");

    f_create(NULL, "dir1", "", 1); //ths will be the root directory
    char *filepath = malloc(8);
    strcpy(filepath, "/dir1/");
    f_create(filepath, "file1", "txt", 0);
    f_create(filepath, "dir2", "", 1);
    f_create(filepath, "file2", "", 0);
    char *filepath_01 = malloc(16);
    strcpy(filepath_01, "/dir1/dir2/");
    f_create(filepath_01, "file_in_dir2", "doc", 0);
    f_create(filepath_01, "dir_in_dir2", "", 1);
    printf("filepath:\n");
    print_directory(filepath);
    printf("filepath_01:\n");
    print_directory(filepath_01);
    print_directory_a(1);
    metadata *fptr = f_open(filepath, "file1", "txt");
    printf("fat = %ld, filepath/filename.extension = %s/%s.%s\n", fptr->fileID, filepath, fptr->filename, fptr->extension);

    char *str = malloc(10000);
    int i;
    for (i = 0; i < 300; i = i + 3)
        str[i] = 'r'; //114
    for (i = 1; i < 300; i = i + 3)
        str[i] = 'd'; //100
    for (i = 2; i < 300; i = i + 3)
        str[i] = 't'; //116bytes
    printf("str = %s\n", str);
    f_write(fptr, str);
    printf("write function was called");
    printf("\nf_read fuction1\n");
    f_read(fptr);
    f_write(fptr, str); //writes the string arr
    printf("write function was called");
    printf("\ff_read fuction2\n");
    f_read(fptr);
    f_write(fptr, str);
    printf("write function was called");
    printf("\ff_read fuction3\n");
    f_read(fptr);
    strcpy(filepath_01, "/dir1/dir2/");
    metadata *fptr1 = f_open(filepath_01, "file_in_dir2", "doc");
    char *string1 = malloc(10000);
    strcpy(string1, "testtest");
    f_write(fptr1, string1);
    f_read(fptr1);
    f_write(fptr1, str);
    f_read(fptr1);
    printf("just note to myself: before delete\n");
    print_directory(filepath_01);
    f_delete(filepath_01, "file_in_dir2", "doc");
    printf("just note to myself: after delete\n");
    print_directory(filepath_01);
    free(str);
    free(string1);
    exit(EXIT_SUCCESS);
}
