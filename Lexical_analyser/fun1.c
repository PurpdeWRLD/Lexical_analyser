#include "header.h"

int is_integer(char *str)
{
    int i = 0;
    if(str[0] == '+' || str[0] == '-')
    {
        i++;
    }
    while(str[i] != '\0')
    {
        if(!(isdigit(str[i])))
        {
            return 0;
        }
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
    if(str[0] == '0' && str[1] == 'x')
    {
        for(i = 2; str[i] != '\0'; i++)
        {
            if(!isxdigit(str[i]))
            {
                return 0;
            }
        }
        return 1;
    }
    else if(str[0] == '0')
    {
        for(i = 1; str[i] != '\0'; i++)
        {
            if(!isdigit(str[i]))
            {
                return 0;
            }
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
    const char *operators[] = {"+", "-", "*", "/", "=", "==", "!=", "<", ">", "<=", ">=", "++", "--", "&&", "||", "&", "|", "^", "~", "!", "%", "<<", ">>"};
    int i;
    for(i = 0; i < sizeof(operators)/ sizeof(operators[0]); i++)
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
    int i = 0, flag = 0;
    if(str[0] == '-' || str[0] == '+')
    {
            i++;
    }
    while(str[i] != '\0')
    {
        if(str[i] == '.')
        {
            flag = 1;
        }
        else if(!(isdigit(str[i])))
        {
            return 0;
        } 
        i++;
    }
    return (flag == 1);
}

int is_array_decl(char *str)
{
    return (strchr(str, '[') && str[strlen(str) - 1] == ']');
}

int is_pre_processor(char *str)
{
    const char *preprocessors[] = {"#include", "#define", "#undef", "#ifdef", "#ifndef","#endif", "#if", "#else", "#elif", "#error", "#pragma", "#line" };

    int i;
    printf("Hii");
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

int is_string_literal(char *str)
{
    int len = strlen(str);
    if(len < 2)
    {
        return 0;
    }
    if(str[0] == '"' && str[len - 1] == '"')
    {
        return 1;
    }
    return 0;
}

void tokenize_line(char *line, FILE *fc, Tokens *statistics) 
{
    int i = 0;
    char token[100];
    
    if (line[i] == '#') 
    {
            int j = 0;
            //token[j++] = line[i++];
            while (line[i] != '\0')
            {
                token[j++] = line[i++];
            }
            token[j] = '\0';

            if (is_pre_processor(token))
            {
                fprintf(fc, "%s is a preprocessor directive\n", token);
                statistics->preprocessor++;
            } 
            else
            {
                fprintf(fc, "%s is unknown\n", token);
            }
            i = strlen(line);
        }
    while (line[i] != '\0') {
        // Skip whitespace
        if (isspace(line[i])) 
        {
            i++;
            continue;
        }

        

        char delim_buf[2] = {line[i], '\0'};
        if (is_delim(delim_buf))
        {
            fprintf(fc, "%c is a delimiter\n", line[i]);
            statistics->delimiters++;
            i++;
            continue;
        }

        if (isalpha(line[i]) || line[i] == '_') 
        {
            int j = 0;
            while (isalnum(line[i]) || line[i] == '_') 
            {
                token[j++] = line[i++];
            }
            token[j] = '\0';

            if (is_keyword(token))
            {
                fprintf(fc, "%s is a keyword\n", token);
                statistics->keyword++;
            } else if (is_identifier(token))
            {
                fprintf(fc, "%s is an identifier\n", token);
                statistics->identifiers++;
            }
            continue;
        }

        if (isdigit(line[i]) || ((line[i] == '+' || line[i] == '-') && isdigit(line[i+1]))) 
        {
            int j = 0;
            while (isalnum(line[i]) || line[i] == '.' || line[i] == 'x' || line[i] == 'X') 
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
            else
            {
                fprintf(fc, "%s is unknown\n", token);
            }
            continue;
        }

        // if (line[i] == '"' || line[i] == '\'')
        // {
        //     int j = 0;
        //     if(line[i] == '\'')
        //     {
        //         token[j++] = line[i++];
        //         while(line[i] != '\'')
        //         {
        //             token[j++] = line[i++];
        //         }
        //         token[j++] = '\'';
        //         token[j] = '\0';
        //     }
        //     else
        //     {
        //         token[j++] = line[i++];
        //         while(line[i] != '"')
        //         {
        //             token[j++] = line[i++];
        //         }
        //         token[j++] = '"';
        //         token[j] = '\0';
        //     }

        //     if (is_string_literal(token)) 
        //     {
        //         fprintf(fc, "%s is a string literal\n", token);
        //         statistics->string_literals++;
        //     }
        //     else
        //     {
        //         fprintf(fc, "%s is unknown\n", token);
        //     }
        //     continue;
        // }

        if (strchr("=<>!&|+-*/%", line[i])) 
        {
            char op[3] = { line[i], line[i + 1], '\0'};
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



        fprintf(fc, "%c is unknown\n", line[i]);
        i++;
    }
}
