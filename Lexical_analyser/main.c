#include "header.h"
int main(int argc, char **argv, char **envp)
{
    FILE *fp, *fc;
    char str[1000];
    Tokens statistics= {0}; //Initialise every keyword, identifiers, operators ..... count to zero!!
    fp = fopen(argv[1], "r");
    fc = fopen("output.txt", "w");
    if(fp == NULL || fc == NULL)
    {
        perror("Error: ");
        return 0;
    }
   // printf("hi\n");
    while(fgets(str, sizeof(str), fp))
    {
        str[strcspn(str, "\n")] = '\0';
        tokenize_line(str, fc, &statistics);
        // if (feof(fp))
        // {
        //     break;
        // }

    }

    //Summary
    
    fprintf(fc, "\nSummary:\n");
    fprintf(fc, "Keywords: %d\n", statistics.keyword);
    fprintf(fc, "Identifiers: %d\n", statistics.identifiers);
    fprintf(fc, "Operators: %d\n", statistics.operators);
    fprintf(fc, "Integers: %d\n", statistics.integers);
    fprintf(fc, "Real Numbers: %d\n", statistics.real_numbers);
    fprintf(fc, "String Literals: %d\n", statistics.string_literals);
    fprintf(fc, "Delimiters: %d\n", statistics.delimiters);
    fprintf(fc, "Preprocessor Directives: %d\n", statistics.preprocessor);
    fprintf(fc, "Hex/Octal Numbers: %d\n", statistics.hex_octal);
    fprintf(fc, "Character Literals: %d\n", statistics.character_literals);
    fprintf(fc, "Single line comments: %d\n", statistics.single_line);
    fprintf(fc, "Multi line comments: %d\n", statistics.multi_line);
    fclose(fp);
    fclose(fc);
    return 0;
}
