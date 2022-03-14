#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

uint32_t UTF8_Unicode(FILE *in) {
    uint32_t symbol = 0;
    uint32_t ch[4];
    ch[0] = fgetc(in);
    if ((ch[0] >> 7) == 0) {
        symbol = ch[0] % (1 << 7);
    } else if ((ch[0] >> 5) == 6) {
        ch[1] = fgetc(in);
        symbol = ((ch[0] % (1 << 5)) << 6) + (ch[1] % (1 << 6));
    } else if ((ch[0] >> 4) == 14) {
        ch[1] = fgetc(in);
        ch[2] = fgetc(in);
        symbol = ((ch[0] % (1 << 4)) << 12) + (((ch[1] % (1 << 6))) << 6) + (ch[2] % (1 << 6));
    } else if ((ch[0] >> 3) == 30) {
        ch[1] = fgetc(in);
        ch[2] = fgetc(in);
        ch[3] = fgetc(in);
        symbol = ((ch[0] % (1 << 3)) << 18) + (((ch[1] % (1 << 6))) << 12) + ((ch[2] % (1 << 6)) << 6) +
                 (ch[3] % (1 << 6));
    }
    return symbol;
}

uint32_t UTF16_LE_Unicode(FILE *in) {
    uint32_t symbol = 0, s1 = 0, s2 = 0;
    uint32_t ch[4];
    ch[0] = fgetc(in);
    ch[1] = fgetc(in);
    symbol = (ch[1] << 8) + ch[0];
    if ((symbol < 0xD800) || (symbol > 0xDBFF)) {
        return symbol;
    } else {
        ch[2] = fgetc(in);
        ch[3] = fgetc(in);
        s1 = ((ch[3] << 8) + ch[2]) - 0xD800;
        s2 = ((ch[1] << 8) + ch[0]) - 0xDC00;
        symbol = ((s1 << 10) + s2) + 0x10000;
        return symbol;
    }
}

uint32_t UTF16_BE_Unicode(FILE *in) {
    uint32_t symbol = 0, s1 = 0, s2 = 0;
    uint32_t ch[4];
    ch[0] = fgetc(in);
    ch[1] = fgetc(in);
    symbol = (ch[0] << 8) + ch[1];
    if ((symbol < 0xD800) || (symbol > 0xDBFF)) {
        return symbol;
    } else {
        ch[2] = fgetc(in);
        ch[3] = fgetc(in);
        s1 = ((ch[0] << 8) + ch[1]) - 0xD800;
        s2 = ((ch[2] << 8) + ch[3]) - 0xDC00;
        symbol = ((s1 << 10) + s2) + 0x10000;
        return symbol;
    }
}

uint32_t UTF32_LE_Unicode(FILE *in) {
    uint32_t symbol = 0;
    uint32_t ch[4];
    for (int i = 0; i < 4; i++) {
        ch[i] = fgetc(in);
    }
    symbol = (ch[3] << 24) + (ch[2] << 16) + (ch[1] << 8) + ch[0];
    return symbol;
}

uint32_t UTF32_BE_Unicode(FILE *in) {
    uint32_t symbol = 0;
    uint32_t ch[4];
    for (int i = 0; i < 4; i++) {
        ch[i] = fgetc(in);
    }
    symbol = (ch[0] << 24) + (ch[1] << 16) + (ch[2] << 8) + ch[3];
    return symbol;
}

void Unicode_UTF8(uint32_t symbol, FILE *out) {
    if (symbol < (1 << 7)) {
        fputc(symbol, out);
    } else if ((symbol >= (1 << 7)) && (symbol < (1 << 11))) {
        fputc((symbol >> 6) + 192, out);
        fputc((symbol % (1 << 6) + 128), out);
    } else if ((symbol >= (1 << 11)) && (symbol < (1 << 16))) {
        fputc((symbol >> 12) + 224, out);
        fputc((((symbol >> 6) % (1 << 6)) + 128), out);
        fputc((symbol % (1 << 6) + 128), out);
    } else if ((symbol < (1 << 16)) && (symbol < (1 << 21))) {
        fputc((symbol >> 18) + 240, out);
        fputc((((symbol >> 12) % (1 << 6)) + 128), out);
        fputc((((symbol >> 6) % (1 << 6)) + 128), out);
        fputc(((symbol % (1 << 6)) + 128), out);
    }
}

void Unicode_UTF16_LE(uint32_t symbol, FILE *out) {
    if (symbol < (1 << 16)) {
        fputc(symbol % (1 << 8), out);
        fputc(symbol >> 8, out);
    } else {
        uint32_t ch[4];
        uint32_t s1, s2;
        symbol = symbol - 0x10000;
        s1 = (symbol >> 10) + 0xD800;
        s2 = symbol % (1 << 10) + 0xDC00;
        symbol = (s1 << 16) + s2;
        for (int i = 0; i < 4; i++) {
            ch[i] = symbol % (1 << 8);
            symbol = symbol >> 8;
        }
        fputc(ch[2], out);
        fputc(ch[3], out);
        fputc(ch[0], out);
        fputc(ch[1], out);
    }
}

void Unicode_UTF16_BE(uint32_t symbol, FILE *out) {
    if (symbol < (1 << 16)) {
        fputc(symbol >> 8, out);
        fputc(symbol % (1 << 8), out);
    } else {
        uint32_t ch[4];
        uint32_t s1, s2;
        symbol = symbol - 0x10000;
        s1 = (symbol >> 10) + 0xD800;
        s2 = symbol % (1 << 10) + 0xDC00;
        symbol = (s1 << 16) + s2;
        for (int i = 0; i < 4; i++) {
            ch[i] = symbol % (1 << 8);
            symbol = symbol >> 8;
        }
        for (int i = 0; i < 4; i++) {
            fputc(ch[3 - i], out);
        }
    }
}

void Unicode_UTF32_LE(uint32_t symbol, FILE *out) {
    fputc(symbol % (1 << 8), out);
    symbol >> 8;
    fputc(symbol % (1 << 8), out);
    symbol >> 8;
    fputc(symbol % (1 << 8), out);
    symbol >> 8;
    fputc(symbol % (1 << 8), out);
}

void Unicode_UTF32_BE(uint32_t symbol, FILE *out) {
    fputc(symbol >> 24, out);
    fputc((symbol >> 16) % (1 << 8), out);
    fputc((symbol >> 8) % (1 << 8), out);
    fputc(symbol % (1 << 8), out);
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("%s", "Specify only the file name, the name of the input and output files as arguments, encoding code");
        return 1;
    }

    FILE *in = fopen(argv[1], "r");

    if (in == NULL) {
        printf("%s", "The input file cannot be opened");
        return 1;
    } else {
        FILE *out = fopen(argv[2], "w");

        if (out == NULL) {
            printf("%s", "The output file cannot be opened");
            return 1;
        }

        char s = argv[3][0];
        int code = s - 48;
        if ((code < 0) || (code > 5) || (argv[3][1] != 0)) {
            printf("%s", "Incorrect encoding identifier. The fourth argument of the program must be a number from 0 to 5");
            return 1;
        }

        int ch[4];
        for (int i = 0; i < 4; i++) {
            ch[i] = -1;
        }

        int ind, iter = 0;
        while ((iter != 4) && (feof(in) == 0)) {
            ch[iter] = fgetc(in);
            iter++;
        }

        if ((ch[0] == 239) && (ch[1] == 187) && (ch[2] == 191)) {
            ind = 1;
            fseek(in, 3, SEEK_SET);
        } else if ((ch[0] == 255) && (ch[1] == 254) && (ch[2] == 0) && (ch[3] == 0)) {
            ind = 4;
        } else if ((ch[0] == 0) && (ch[1] == 0) && (ch[2] == 254) && (ch[3] == 255)) {
            ind = 5;
        } else if ((ch[0] == 255) && (ch[1] == 254)) {
            ind = 2;
            fseek(in, 2, SEEK_SET);
        } else if ((ch[0] == 254) && (ch[1] == 255)) {
            ind = 3;
            fseek(in, 2, SEEK_SET);
        } else {
            ind = 0;
            fseek(in, 0, SEEK_SET);
        }

        if (ind == code) {
            int x;
            fseek(in, 0, SEEK_SET);
            while (feof(in) == 0) {
                x = fgetc(in);
                fputc(x, out);
            }
        }

        if ((ind == 0) && (code == 1)) {
            fputc(239, out);
            fputc(187, out);
            fputc(191, out);
            while (feof(in) == 0) {
                Unicode_UTF8(UTF8_Unicode(in), out);
            }
        }

        if ((ind == 0) && (code == 2)) {
            fputc(255, out);
            fputc(254, out);
            while (feof(in) == 0) {
                Unicode_UTF16_LE(UTF8_Unicode(in), out);
            }
        }

        if ((ind == 0) && (code == 3)) {
            fputc(254, out);
            fputc(255, out);
            while (feof(in) == 0) {
                Unicode_UTF16_BE(UTF8_Unicode(in), out);
            }
        }

        if ((ind == 0) && (code == 4)) {
            fputc(255, out);
            fputc(254, out);
            fputc(0, out);
            fputc(0, out);
            while (feof(in) == 0) {
                Unicode_UTF32_LE(UTF8_Unicode(in), out);
            }
        }

        if ((ind == 0) && (code == 5)) {
            fputc(0, out);
            fputc(0, out);
            fputc(254, out);
            fputc(255, out);
            while (feof(in) == 0) {
                Unicode_UTF32_BE(UTF8_Unicode(in), out);
            }
        }

        if ((ind == 1) && (code == 0)) {
            while (feof(in) == 0) {
                Unicode_UTF8(UTF8_Unicode(in), out);
            }
        }

        if ((ind == 1) && (code == 2)) {
            fputc(255, out);
            fputc(254, out);
            while (feof(in) == 0) {
                Unicode_UTF16_LE(UTF8_Unicode(in), out);
            }
        }

        if ((ind == 1) && (code == 3)) {
            fputc(254, out);
            fputc(255, out);
            while (feof(in) == 0) {
                Unicode_UTF16_BE(UTF8_Unicode(in), out);
            }
        }

        if ((ind == 1) && (code == 4)) {
            fputc(255, out);
            fputc(254, out);
            fputc(0, out);
            fputc(0, out);
            while (feof(in) == 0) {
                Unicode_UTF32_LE(UTF8_Unicode(in), out);
            }
        }

        if ((ind == 1) && (code == 5)) {
            fputc(0, out);
            fputc(0, out);
            fputc(254, out);
            fputc(255, out);
            while (feof(in) == 0) {
                Unicode_UTF32_BE(UTF8_Unicode(in), out);
            }
        }

        if ((ind == 2) && (code == 0)) {
            while (feof(in) == 0) {
                Unicode_UTF8(UTF16_LE_Unicode(in), out);
            }
        }

        if ((ind == 2) && (code == 1)) {
            fputc(239, out);
            fputc(187, out);
            fputc(191, out);
            while (feof(in) == 0) {
                Unicode_UTF8(UTF16_LE_Unicode(in), out);
            }
        }

        if ((ind == 2) && (code == 3)) {
            fputc(254, out);
            fputc(255, out);
            while (feof(in) == 0) {
                Unicode_UTF16_BE(UTF16_LE_Unicode(in), out);
            }
        }

        if ((ind == 2) && (code == 4)) {
            fputc(255, out);
            fputc(254, out);
            fputc(0, out);
            fputc(0, out);
            while (feof(in) == 0) {
                Unicode_UTF32_LE(UTF16_LE_Unicode(in), out);
            }
        }

        if ((ind == 2) && (code == 5)) {
            fputc(0, out);
            fputc(0, out);
            fputc(254, out);
            fputc(255, out);
            while (feof(in) == 0) {
                Unicode_UTF32_BE(UTF16_LE_Unicode(in), out);
            }
        }

        if ((ind == 3) && (code == 0)) {
            while (feof(in) == 0) {
                Unicode_UTF8(UTF16_BE_Unicode(in), out);
            }
        }

        if ((ind == 3) && (code == 1)) {
            fputc(239, out);
            fputc(187, out);
            fputc(191, out);
            while (feof(in) == 0) {
                Unicode_UTF8(UTF16_BE_Unicode(in), out);
            }
        }

        if ((ind == 3) && (code == 2)) {
            fputc(255, out);
            fputc(254, out);
            while (feof(in) == 0) {
                Unicode_UTF16_LE(UTF16_BE_Unicode(in), out);
            }
        }

        if ((ind == 3) && (code == 4)) {
            fputc(255, out);
            fputc(254, out);
            fputc(0, out);
            fputc(0, out);
            while (feof(in) == 0) {
                Unicode_UTF32_LE(UTF16_BE_Unicode(in), out);
            }
        }

        if ((ind == 3) && (code == 5)) {
            fputc(0, out);
            fputc(0, out);
            fputc(254, out);
            fputc(255, out);
            while (feof(in) == 0) {
                Unicode_UTF32_BE(UTF16_BE_Unicode(in), out);
            }
        }

        if ((ind == 4) && (code == 0)) {
            while (feof(in) == 0) {
                Unicode_UTF8(UTF32_LE_Unicode(in), out);
            }
        }

        if ((ind == 4) && (code == 1)) {
            fputc(239, out);
            fputc(187, out);
            fputc(191, out);
            while (feof(in) == 0) {
                Unicode_UTF8(UTF32_LE_Unicode(in), out);
            }
        }

        if ((ind == 4) && (code == 2)) {
            fputc(255, out);
            fputc(254, out);
            while (feof(in) == 0) {
                Unicode_UTF16_LE(UTF32_LE_Unicode(in), out);
            }
        }

        if ((ind == 4) && (code == 3)) {
            fputc(254, out);
            fputc(255, out);
            while (feof(in) == 0) {
                Unicode_UTF16_BE(UTF32_LE_Unicode(in), out);
            }
        }

        if ((ind == 4) && (code == 5)) {
            fputc(0, out);
            fputc(0, out);
            fputc(254, out);
            fputc(255, out);
            while (feof(in) == 0) {
                Unicode_UTF32_BE(UTF32_LE_Unicode(in), out);
            }
        }

        if ((ind == 5) && (code == 0)) {
            while (feof(in) == 0) {
                Unicode_UTF8(UTF32_BE_Unicode(in), out);
            }
        }

        if ((ind == 5) && (code == 1)) {
            fputc(239, out);
            fputc(187, out);
            fputc(191, out);
            while (feof(in) == 0) {
                Unicode_UTF8(UTF32_BE_Unicode(in), out);
            }
        }

        if ((ind == 5) && (code == 2)) {
            fputc(255, out);
            fputc(254, out);
            while (feof(in) == 0) {
                Unicode_UTF16_LE(UTF32_BE_Unicode(in), out);
            }
        }

        if ((ind == 5) && (code == 3)) {
            fputc(254, out);
            fputc(255, out);
            while (feof(in) == 0) {
                Unicode_UTF16_BE(UTF32_BE_Unicode(in), out);
            }
        }

        if ((ind == 5) && (code == 4)) {
            fputc(255, out);
            fputc(254, out);
            fputc(0, out);
            fputc(0, out);
            while (feof(in) == 0) {
                Unicode_UTF32_LE(UTF32_BE_Unicode(in), out);
            }
        }

        fclose(in);
        fclose(out);
    }
    return 0;