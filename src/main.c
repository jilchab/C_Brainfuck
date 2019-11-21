#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <curses.h>

#define NUM_CELLS   256

int main(int argc, char **arg)
{
    if (argc < 2) {
        printf("Error missing argument (%s)\n", strerror(EINVAL));
        return -EIO;
    }

    FILE *file = fopen(arg[1], "r");

    if (!file) {
        printf("Error opening file %s (%s)\n", arg[argc - 1], strerror(errno));
        return errno;
    } else {
        printf("Opening file %s\n", arg[argc - 1]);
    }

    fseek(file, 0 , SEEK_END);
    long int file_size = ftell(file);
    fseek(file, 0 , SEEK_SET);

    int cells[NUM_CELLS];
    long int loops[32];
    int loop_id = 0;
    int *pointer = cells;
    int err = 0;

    while (ftell(file) < file_size) {
        if (fgetc(file) == '@') {
            break;
        }
    }
    if (ftell(file) == file_size) {
        err = -1;
    }

    while (ftell(file) < file_size && !err) {
        switch (fgetc(file)) {
        case '+':
            ++(*pointer);
            break;
        case '-':
            --(*pointer);
            break;
        case '>':
            if (pointer < cells + NUM_CELLS - 1) {
                ++pointer;
            }
            break;
        case '<':
            if (pointer > cells) {
                --pointer;
            }
            break;
        case '.':
            printf("%c", *pointer);
            break;
        case ',':
            *pointer = getchar();
            break;
        case '[': {
            long int origin = ftell(file);
            if (*pointer) {
                loops[loop_id] = origin;
                ++loop_id;
            } else {
                char c;
                int level = 1;
                while (ftell(file) < file_size) {
                    c = fgetc(file);
                    if (c == '[') {
                        ++level;
                    } else if (c == ']') {
                        if (--level == 0) {
                            break;
                        }
                    }
                    if (ftell(file) == file_size) {
                        printf("Error missing ']' ('[' at %ld)", origin);
                        err = -1;
                        break;
                    }
                }
            }
            break;
        }
        case ']':
            if (!loop_id) {
                printf("Error missing [");
                break;
            }
            if (*pointer) {
                fseek(file, loops[loop_id - 1], SEEK_SET);
            }
            break;
        case 's':
            printf("\n[%3d ][%3d ][%3d ][%3d ]\n", cells[0], cells[1], cells[2], cells[3]);
            break;
        case '/':
            {
                char c = fgetc(file);
                if (c == '/') {
                    while (ftell(file) < file_size) {
                        if (fgetc(file) == '\n') {
                            break;
                        }
                    }
                } else if (c == '*') {
                    while (ftell(file) < file_size) {
                        if (fgetc(file) == '*') {
                            if (fgetc(file) == '/') {
                                break;
                            }
                        }
                    }
                }
            }
            break;
        case '%':
            err = 1;
            break;
        default:
            break;
        }
    }


    printf("\n");
    fclose(file);

    if (err == 1) {
        err = 0;
    }

    return err;
}

