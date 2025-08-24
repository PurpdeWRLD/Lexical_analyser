#include "header.h"

//Function declaration
int is_hex_octal(char *);
int is_delim(char *);
int is_integer(char *);
int is_keyword(char *);
int is_operator(char *);
int is_real_number(char *);
int is_pre_processor(char *);
int is_identifier(char *);

int is_integer(char *str)
{
    int i = 0;

    if (str == NULL || str[0] == '\0')
        return 0; // empty string is not a valid integer

    if (str[i] == '+' || str[i] == '-')
        i++;

    if (str[i] == '\0')
        return 0; // string was only '+' or '-'

    while (str[i] != '\0')
    {
        if (!isdigit(str[i]))
            return 0;
        i++;
    }

    return 1;
}

int is_delim(char *str)
{
    const char *delim[] = {",", ";", "(", ")", "[", "]", "{", "}", ":"};
    int i;
    for (i = 0; i < 9; i++)
    {
        if (strcmp(delim[i], str) == 0)
        {
            return 1;
        }
    }
    return 0;
}

int is_hex_octal(char *str)
{
    int i;

    if (str == NULL || str[0] == '\0')
        return 0;

    // Check for hex: starts with 0x or 0X, and must have at least one hex digit after that
    if (str[0] == '0' && (str[1] == 'x' || str[1] == 'X')) 
    {
    
        for (i = 2; str[i] != '\0'; i++) 
        {
            if (!isxdigit(str[i])) 
                return 0;
        }
        return 1;
    }
    // Check for octal: starts with 0 and contains only digits 0-7
    else if (str[0] == '0') 
    {
        for (i = 1; str[i] != '\0'; i++) 
        {
            if (str[i] < '0' || str[i] > '7') 
                return 0;
        }
        return 1;
    }

    return 0;
}


int is_keyword(char *str)
{
    int i;
    char keyword[32][10] = {"char", "int", "float", "double", "signed", "unsigned", "short", "long", "const", "volatile", "for", "while", "do", "goto", "break", "continue", "if", "else", "switch", "case", "default", "auto", "register", "static", "extern", "struct", "unions", "enums", "typedefs", "void", "return", "sizeof"};
    for(i = 0; i < 32; i++)
    {
        if(strcmp(str, keyword[i]) == 0)
        {
            return 1;
        }
    }
    return 0;
}

int is_operator(char *str)
{
    const char *operators[] = {"+", "-", "*", "/", "=", "==", "!=", "<", ">", "<=", ">=", "++", "--", "&&", "||", "&", "|", "^", "~", "!", "%", "<<", ">>", "+=", "-=", ".", "->"};
    int i;
    int len = sizeof(operators)/ sizeof(operators[0]);
    for(i = 0; i < len; i++)
    {
        if(strcmp(str, operators[i]) == 0)
        {
            return 1;
        }
    }
    return 0;
}

int is_real_number(char *str)
{
    int i = 0, flag = 0, digit_count = 0;
    if(str[0] == '-' || str[0] == '+')
    {
            i++;
    }
    while(str[i] != '\0')
    {
        if(str[i] == '.')
        {
            flag++;
            if(flag > 1)
            {
                return 0;
            }
        }
        else if (isdigit(str[i]))
        {
            digit_count++;
        }
        else
        {
            return 0;
        }
        i++;
    }
    return (flag == 1 && digit_count > 0);
}


int is_pre_processor(char *str)
{
    const char *preprocessors[] = {"#include", "#define", "#undef", "#ifdef", "#ifndef","#endif", "#if", "#else", "#elif", "#error", "#pragma", "#line" };

    int i;
    int len = sizeof(preprocessors) / sizeof(preprocessors[0]);
    for (i = 0; i < len; i++) {
        if (strstr(str, preprocessors[i]) != NULL)
        {
            return 1;
        }
    }
    return 0;
}


int is_identifier(char *str)
{
    int i;
    if(!isalpha(str[0]) && str[0] != '_')
    {
        return 0;
    }
    for(i = 1; str[i] != '\0'; i++)
    {
        if(!isalnum(str[i]) && str[i] != '_')
        {
            return 0;
        }
    }
    return 1;
}





void tokenize_line(char *line, FILE *fc, Tokens *statistics) 
{
    int i = 0;
    char token[1000];
    static char str[200];
    static char str1[200];
    static int index = 0;
    static int index1 = 0;
    static int m_flag = 0;
    static int s_flag = 0; // flag for multi-line comment or string

    while (line[i] != '\0') 
    {
        // Skip whitespace
        if (isspace(line[i])) 
        {
            i++;
            continue;
        }

        // Delimiters
        char delim_buf[2] = {line[i], '\0'};
        if (is_delim(delim_buf))
        {
            fprintf(fc, "%c is a delimiter\n", line[i]);
            statistics->delimiters++;
            i++;
            continue;
        }

        // Preprocessor directives
        else if (line[i] == '#') 
        {
            int j = 0;
            token[j++] = line[i++];
            while (line[i] != '\0' && j < sizeof(token) - 1)
                token[j++] = line[i++];
            token[j] = '\0';

            if (is_pre_processor(token))
            {
                fprintf(fc, "%s is a preprocessor directive\n", token);
                statistics->preprocessor++;
            }
            continue;
        }

        else if (line[i] == '"' || s_flag == 1)
        {
            str[index++] = line[i++];
            while (line[i] != '"' && line[i] != '\0')
            {
                str[index++] = line[i++];
            }
            if (line[i] == '\0') 
            {
                index--;
                //str[index++] = ' ';
                s_flag = 1;
                return ;
            }
            else if(line[i] == '"')
            {
                str[index++] = line[i++];
                str[index] = '\0';
                fprintf(fc, "%s is a string literal\n", str);
                index = 0;
                //str[0] = '\0';
                s_flag = 0;
                statistics->string_literals++;
                continue;
            } 
        }
        // Multi-line comments
        else if ((line[i] == '/' && line[i + 1] == '*') || m_flag == 1) 
        {

            while (line[i] != '\0' && !(line[i] == '*' && line[i + 1] == '/'))
            {
                str[index++] = line[i++];
            }

            if (line[i] == '\0')
            {
                //str[index++] = ' ';
                index--;
                m_flag = 1;
                return ;
            }
            else
            {
                str[index++] = line[i++];
                str[index++] = line[i++];
                str[index] = '\0';
                m_flag = 0;
                fprintf(fc, "%s is a multi-line comment\n", str);
                statistics->multi_line++;
                index = 0;
            }
            continue;
        }

        // Single-line comments
        else if (line[i] == '/' && line[i + 1] == '/') 
        {
            int j = 0;
            while (line[i] != '\0')
                token[j++] = line[i++];
            token[j] = '\0';
            fprintf(fc, "%s is a single line comment\n", token);
            statistics->single_line++;
            return ;
        }

        // Identifiers / Keywords
        else if (isalpha(line[i]) || line[i] == '_') 
        {
            int j = 0;
            while ((isalnum(line[i]) || line[i] == '_') && j < sizeof(token) - 1)
                token[j++] = line[i++];
            token[j] = '\0';

            if (is_keyword(token))
            {
                fprintf(fc, "%s is a keyword\n", token);
                statistics->keyword++;
            }
            else if (is_identifier(token))
            {
                fprintf(fc, "%s is an identifier\n", token);
                statistics->identifiers++;
            }
            continue;
        }

        // Numbers
        else if((isdigit(line[i]) || (line[i] == '+' || line[i] == '-')))
        {
            int j = 0;
            token[j++] = line[i++];
            while (line[i] != '\0' && j < sizeof(token) - 1 && (isalnum(line[i]) || line[i] == '.' || line[i] == 'x' || line[i] == 'X'))
            {
                token[j++] = line[i++];
            }
            token[j] = '\0';

            if (is_real_number(token)) 
            {
                fprintf(fc, "%s is a real number\n", token);
                statistics->real_numbers++;
            }
            else if (is_integer(token))
            {
                fprintf(fc, "%s is an integer\n", token);
                statistics->integers++;
            }
            else if (is_hex_octal(token))
            {
                fprintf(fc, "%s is a hex or octal number\n", token);
                statistics->hex_octal++;
            }
            continue;
        }
        
        // Character Literals
        else if (line[i] == '\'') 
        {
            int j = 0;
            token[j++] = line[i++];

            if (line[i] == '\\') 
            {
                token[j++] = line[i++]; // backslash
                if (line[i] != '\0')
                    token[j++] = line[i++]; // escaped char
            }
            else if (line[i] != '\0')
            {
                token[j++] = line[i++]; // normal char
            }

            if (line[i] == '\'') 
            {
                token[j++] = line[i++];
            }
            token[j] = '\0';
            fprintf(fc, "%s is a character literal\n", token);
            statistics->character_literals++;
            continue;
        }

        // Operators
        else if (strchr("=<>!&|+-*/%.~->^", line[i])) 
        {
            char op[3] = { line[i], line[i + 1], '\0' };

            if (is_operator(op)) 
            {
                fprintf(fc, "%s is an operator\n", op);
                statistics->operators++;
                i += 2;
                continue;
            }
            else 
            {
                op[1] = '\0';
                if (is_operator(op)) 
                {
                    fprintf(fc, "%s is an operator\n", op);
                    statistics->operators++;
                    i++;
                    continue;
                }
            }
        }
        else
        {
            // Unrecognized characters
            fprintf(fc, "%c is unknown\n", line[i]);
            i++;
        }
    }
}
