#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

enum type {
    unsigned_int,
    signed_int,
    string,
};

void print_help() {
    printf(
        "binw 0.0.1\n"
        "Write binary data to a file\n\n"
        "usage:\n"
        "    binw [-h|--help] <file> (<u8|i8|u16|i16|u32|i32|u64|i64|str> <value>)+\n\n"
        "example:\n"
        "    binw data.bin u8 123 str 'ding dong' i16 32145\n\n"
        "    xxd -b data.bin\n"
        "    01111011 01100100 01101001 01101110 01100111 00100000  {ding \n"
        "    01100100 01101111 01101110 01100111 10010001 01111101  dong.}\n"
    );
}

int_fast8_t append(FILE *file, char **arg) {
    for (; *arg; ++arg) {
        enum type type;
        uint_fast8_t int_width = 0;
        if (strcmp("u8", *arg) == 0) {
            type = unsigned_int;
            int_width = 1;
        } else if (strcmp("u16", *arg) == 0) {
            type = unsigned_int;
            int_width = 2;
        } else if (strcmp("u32", *arg) == 0) {
            type = unsigned_int;
            int_width = 4;
        } else if (strcmp("u64", *arg) == 0) {
            type = unsigned_int;
            int_width = 8;
        } else if (strcmp("i8", *arg) == 0) {
            type = signed_int;
            int_width = 1;
        } else if (strcmp("i16", *arg) == 0) {
            type = signed_int;
            int_width = 2;
        } else if (strcmp("i32", *arg) == 0) {
            type = signed_int;
            int_width = 4;
        } else if (strcmp("i64", *arg) == 0) {
            type = signed_int;
            int_width = 8;
        } else if (strcmp("str", *arg) == 0) {
            type = string;
        } else {
            return -1;
        }

        if (!*(++arg)) {
            return -2;
        }

        switch (type) {
        case unsigned_int: {
            uint_fast64_t value = strtoull(*arg, NULL, 10);
            if ((value == 0 && strcmp("0", *arg) != 0) || value > pow(2, int_width * 8) - 1) {
                return -3;
            }
            fwrite(&value, int_width, 1, file);
            break;
        }
        case signed_int: {
            int_fast64_t value = strtoll(*arg, NULL, 10);
            if (
                (value == 0 && strcmp("0", *arg) != 0)
                || value > pow(2, (int_width * 8) - 1) - 1
                || value < -(pow(2, (int_width * 8) - 1) - 1)
            ) {
                return -3;
            }
            fwrite(&value, int_width, 1, file);
            break;
        }
        case string:
            fwrite(*arg, sizeof(**arg), strlen(*arg), file);
            break;
        }
    }
    return 0;
}

int main(int argc, char *argv[]) {
    for (char **arg = argv; *arg; ++arg) {
        if (strcmp("--help", *arg) == 0 || strcmp("-h", *arg) == 0) {
            print_help();
        }
    }

    if (argc == 1) {
        print_help();
        return 1;
    } else if (argc < 3) {
        printf("error: missing arguments\n");
        return 1;
    }

    FILE *file = fopen(argv[1], "ab");
    int_fast8_t result = append(file,  argv + 2);
    fclose(file);

    if (result == -1) {
        printf("error: invalid type\n");
    } else if (result == -2) {
        printf("error: missing argument\n");
    } else if (result == -3) {
        printf("error: invalid value\n");
    }
    
    if (result != 0) {
        return 1;
    }

    return 0;
}
