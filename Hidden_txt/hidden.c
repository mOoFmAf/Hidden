// made by moof
// last updated: 11/21/2025

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#define uint unsigned int

#define MAX 1024

typedef struct {
    char *fileString;
    size_t size;
} txt_t;

void print_buff(txt_t *file) {
    for (size_t i = 0; i <= file->size; i++) {
        printf("%u, ", (unsigned short int)file->fileString[i]);
    }
    printf("\n");
}


char hidechar(char c, int r) {

    for (int i = 0; i < r; i++) {
        if (c == 127) {
            c = 1;
        } else {
            c++;
        }
    }
    return c;
}

char unhidechar(char c, int r) {

    for (int i = 0; i < r; i++) {
        if (c == 1) {
            c = 127;
        } else {
            c--;
        }
    }
    return c;
}

void printToFile(FILE *fptr, txt_t *file) {

    size_t i = 0;
    
    while (file->fileString[i] != 0) {
        fputc(file->fileString[i], fptr);
        i++;
    }
}

size_t getFileSize(FILE *fptr) {

    char c = fgetc(fptr);
    size_t i = 0;
    
    while (c != EOF) {
        c = fgetc(fptr);
        i++;
    }
    fseek(fptr, 0, SEEK_SET);
    return i;
}

uint strToInt(const char *string) {

    uint intval = 0;

    for (size_t i = 0; string[i] != '\0'; i++) {
        if (isdigit(string[i])) {
            intval = intval * 10 + (string[i] - 48);
        }
    }
    return intval;
}


void hideFile(txt_t *file, const char *newfilename, uint code) {

    srand(code);

    FILE *fptr = fopen(newfilename, "w");
    int r;
    
    for (size_t i = 0; i < file->size; i++) {
		r = rand() % MAX;
        file->fileString[i] = hidechar(file->fileString[i], r);
    }
    printToFile(fptr, file);
    fclose(fptr);
}

void unhideFile(txt_t *file, const char *newfilename, uint code) {

    srand(code);

    FILE *fptr = fopen(newfilename, "w");
    int r;

    for (size_t i = 0; i < file->size; i++) {
        r = rand() % MAX;
        file->fileString[i] = unhidechar(file->fileString[i], r);
    }

    printToFile(fptr, file);
    fclose(fptr);
    
}

void unhideFileAndPrint(txt_t *file, uint code) {

    srand(code);

    int r;

    for (size_t i = 0; i < file->size; i++) {
        r = rand() % MAX;
        file->fileString[i] = unhidechar(file->fileString[i], r);
    }
    printf("FILE:\n");
    printToFile(stdout, file);
    
}

txt_t readFile(const char *filename) {

    int j = 0;
    FILE *fptr = fopen(filename, "r");
    char c;
    txt_t file = {0};
    file.fileString = malloc(sizeof(char));
    file.size = getFileSize(fptr);

    if (fptr == NULL) {
        fprintf(stderr, "ERROR: No such file found\n");
        exit(-1);
    }
    c = fgetc(fptr);
    while (c != EOF) {
        file.fileString = realloc(file.fileString, sizeof(char) * (j + 2));
        if (file.fileString == NULL) {
            fprintf(stderr, "ERROR: realloc fail in readFile\n");
            exit(-1);
        }
        file.fileString[j] = c;
        file.fileString[j + 1] = '\0';
        j++;
        c = fgetc(fptr);
    }
    fclose(fptr);
    return file;
}

void help() {
    
    printf("HELP:\n");
    printf("do as so: hidden -arg filename code newfilename\n");
    printf("-h: to hide a file\n");
    printf("-uh: to unhide the file\n");
    printf("-uhp: unhides and prints file to terminal\n");
    printf("-uhp USAGE: hidden -uhp filename code\n");
    printf("The code is a number that is the password to the hidden file\n");
}

int main(int argc, char *argv[]) {
    
    txt_t targetFile = {0};
    
	if (argc == 1) {
		help();
		return 0;
	}

    if ((strcmp(argv[1], "-h") == 0)) {
        if (argc != 5) {
            fprintf(stderr, "ERROR: Too many or too few args\n");
            help();
            return 0;
        }
        targetFile = readFile(argv[2]);
        hideFile(&targetFile, argv[4], strToInt(argv[3]));
    } else if (strcmp(argv[1], "-uh") == 0) {
        if (argc != 5) {
            fprintf(stderr, "ERROR: Too many or too few args\n");
            help();
            return 0;
        }
        targetFile = readFile(argv[2]);
        unhideFile(&targetFile, argv[4], strToInt(argv[3]));
    } else if (strcmp(argv[1], "-uhp") == 0) {
        if (argc == 4) {
            targetFile = readFile(argv[2]);
            unhideFileAndPrint(&targetFile, strToInt(argv[3]));
        } else {
            fprintf(stderr, "ERROR: Too many or too few args\n");
            help();
            return 0;
        }
       
    } else if (strcmp(argv[1], "-help") == 0) {
        help();
    } else {
        printf("unknown arg or args\n");
        help();
    }
    free(targetFile.fileString);
    return 0;
}
