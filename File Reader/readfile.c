#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <time.h>
#include <stdlib.h>

struct stat stats;

char res[4194304];

char buf[2];

char filename[256];

char empty[8];

void printFileProperties(struct stat stats) {
    struct tm dt;

    printf("File size: %ld\n", stats.st_size);
    dt = *(gmtime(&stats.st_ctime));
    printf("Created on: %d-%d-%d %d:%d:%d\n", dt.tm_mday, dt.tm_mon, dt.tm_year + 1900, dt.tm_hour, dt.tm_min, dt.tm_sec);

    dt = *(gmtime(&stats.st_mtime));
    printf("Modified on: %d-%d-%d %d:%d:%d\n", dt.tm_mday, dt.tm_mon, dt.tm_year + 1900, dt.tm_hour, dt.tm_min, dt.tm_sec);
}

int main() {
    int i;
    printf("File name: ");
    fgets(filename, 256, stdin);
    for (i = 0; i < strlen(filename); i += 1) {
        if (filename[i] == '\n') {
            filename[i] = '\0';
            break;
        }
    }
    if (access(filename, F_OK)) {
        printf("File does not exist.\n");
        fgets(filename, 256, stdin);
        return 0;
    }
    if (access(filename, R_OK)) {
        printf("File is unreadable.\n");
        fgets(filename, 256, stdin);
        return 0;
    }
    FILE *fstr = fopen(filename, "r");
    unsigned int c;
    c = fgetc(fstr);
    i = 0;
    if (stat(filename, &stats) == 0) {
      printf("File metadata: \n");
      printFileProperties(stats);
    }
    printf("Enter to continue reading.");
    fgets(empty, 8, stdin);
    printf("Raw stream:\n");
    while(c != -1) {
        res[i] = c;
        printf("%d ", c);
        c = fgetc(fstr);
        if (i >= 4194304) {
          printf("\nFile too big! Reading from stream terminated. Press Enter to continue.\n");
          fgets(empty, 8, stdin);
          break;
        }
        i += 1;
    }
    printf("\n");
    if (res[0] == 127 && res[1] == 69 && res[2] == 76 && res[3] == 70) {
      printf("This file looks like an ELF shared library file.\n");
    }
    if (res[0] == 137 && res[1] == 80 && res[2] == 78 && res[3] == 71) {
      printf("This file looks like a PNG image file.\n");
    }
    if (res[0] == 37 && res[1] == 80 && res[2] == 68 && res[3] == 70) {
      printf("This file looks like an Adobe PDF file.\n");
    }
    if (res[0] == -1 && res[1] == -40 && res[2] == -1) {
      printf("This file looks like a JPEG image file.\n");
    }
    if (res[0] == 80 && res[1] == 75 && (res[2] == 3 || res[2] == 5 || res[2] == 7)) {
      printf("This file looks like a ZIP file.\n");
    }
    if (res[0] == 47 && res[1] == 49 && res[2] == 46 && res[3] == 38 && (res[4] == 39 || res[4] == 37) && res[5] == 61) {
      printf("This file looks like a GIF image file.\n");
    }
    if (res[0] == 77 && res[1] == 90 && res[2] == -112 && res[3] == 0 && res[4] == 3 && res[5] == 0 && res[6] == 0 && res[7] == 0 && res[8] == 4 && res[9] == 0) {
      printf("This file looks like a Windows Executable file.\n");
    }
    printf("Enter to continue.");
    fgets(empty, 8, stdin);
    printf("\nFile stream length: %d\n", i);
    printf("Reading file as ASCII: \n");
    int b = i;
    for (i = 0; i < b; i += 1) {
        buf[0] = res[i];
        if (buf[0] == '\0') {
            printf("␀");
        }
        buf[1] = '\0';
        printf("%s", buf);
    }
    printf("\n");
    printf("Reading of file \"%s\" ended. Press Enter to continue.\n", filename);
    fgets(empty, 8, stdin);
    fclose(fstr);
    return 0;
}
