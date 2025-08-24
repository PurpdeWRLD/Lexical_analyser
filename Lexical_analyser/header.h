#ifndef HEADER_H
#define HEADER_H
#include <stdio.h>
#include <string.h>
#include <ctype.h>

//Structure to count each keyword , identifier, .... count
typedef struct
{
    int keyword, identifiers, operators, integers, real_numbers, string_literals, character_literals, delimiters, preprocessor, hex_octal, single_line, multi_line;
}Tokens;


void tokenize_line(char *line, FILE *fc, Tokens *statistics);
#endif