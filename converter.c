/*
 C to Python Translator with Array Support
 This program translates a subset of C code to Python code.
 It handles variable declarations, arrays, loops, conditionals, input/output, and expressions.
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

// Global variable for tracking indentation in Python code
int indent_level = 0;
bool in_for_loop = false;          // Track if we're processing a for loop body
bool in_array_declaration = false; // Track array declarations

// Print indentation based on current level
void print_indent()
{
    for (int i = 0; i < indent_level; i++)
    {
        printf("    "); // 4 spaces for each indent level
    }
}

// Trim leading and trailing spaces from a string
void trim(char *str)
{
    // Trim leading spaces
    char *p = str;
    while (*p == ' ' || *p == '\t')
        p++;
    memmove(str, p, strlen(p) + 1);

    // Trim trailing spaces
    p = str + strlen(str) - 1;
    while (p >= str && (*p == ' ' || *p == '\t' || *p == '\n' || *p == '\r'))
        *p-- = '\0';
}

// Extract array name from declaration (e.g., "int arr[10]" -> "arr")
void extract_array_name(char *declaration, char *name)
{
    char *bracket = strchr(declaration, '[');
    if (bracket)
    {
        int len = bracket - declaration;
        strncpy(name, declaration, len);
        name[len] = '\0';
        trim(name);
    }
    else
    {
        strcpy(name, declaration);
    }
}

// Translate array initialization
void translate_array_init(char *type, char *decl)
{
    char name[50], size[50];
    char *bracket = strchr(decl, '[');
    char *equals = strchr(decl, '=');

    if (bracket)
    {
        // Extract array name and size
        sscanf(decl, "%[^[][%[^]]", name, size);
        trim(name);
        trim(size);

        print_indent();
        //  printf("%s = [0] * %s  # Array declaration\n", name, size);

        // Handle initialization if present
        if (equals)
        {
            char init_values[256] = "";
            char *curly = strchr(equals, '{');
            if (curly)
            {
                char *end_curly = strchr(curly, '}');
                if (end_curly)
                {
                    int len = end_curly - curly - 1;
                    strncpy(init_values, curly + 1, len);
                    init_values[len] = '\0';
                    trim(init_values);

                    // Split comma-separated values
                    print_indent();
                    printf("%s = [%s]\n", name, init_values);
                }
            }
        }
    }
}

// Translate array access (e.g., arr[i] = 5;)
void translate_array_access(char *line)
{
    char array_name[50], index[50], value[50];

    // Handle assignment to array element (arr[i] = value;)
    if (sscanf(line, "%[^[][%[^]] = %[^;];", array_name, index, value) == 3)
    {
        trim(array_name);
        trim(index);
        trim(value);
        print_indent();
        printf("%s[%s] = %s\n", array_name, index, value);
        return;
    }

    // Handle reading from array (x = arr[i];)
    if (sscanf(line, "%[^=]= %[^[][%[^]];", value, array_name, index) == 3)
    {
        trim(value);
        trim(array_name);
        trim(index);
        print_indent();
        printf("%s = %s[%s]\n", value, array_name, index);
        return;
    }
}

// Translate a single line of C code to Python
void translate(char *line)
{
    trim(line);

    // Skip the main function signature
    if (strncmp(line, "int main", 8) == 0)
    {
        return;
    }

    // Skip comments
    if (strncmp(line, "//", 2) == 0 || strncmp(line, "/", 2) == 0 || strncmp(line, "/", 2) == 0)
    {
        return;
    }

    // Handle array declarations
    if ((strncmp(line, "int ", 4) == 0 || strncmp(line, "float ", 6) == 0) && strchr(line, '['))
    {
        char *type = strtok(line, " ");
        char *decl = strtok(NULL, ";");
        if (decl)
        {
            translate_array_init(type, decl);
        }
        return;
    }

    // Handle array access operations
    if (strchr(line, '[') && strchr(line, ']'))
    {
        translate_array_access(line);
        return;
    }

    // Handle variable declarations like int a, b = 5;
    if ((strncmp(line, "int ", 4) == 0 || strncmp(line, "float ", 6) == 0) && strchr(line, ';'))
    {
        char *type = strtok(line, " ");
        char *decl = strtok(NULL, ";");
        char *token = strtok(decl, ",");
        while (token)
        {
            trim(token);
            if (strchr(token, '='))
            {
                char var[50], val[50];
                sscanf(token, "%[^=]=%s", var, val);
                trim(var);
                trim(val);
                print_indent();
                printf("%s = %s\n", var, val);
            }
            else
            {
                print_indent();
                printf("%s = 0\n", token);
            }
            token = strtok(NULL, ",");
        }
        return;
    }

    // Convert scanf to Python input()
    if (strstr(line, "scanf"))
    {
        // Handle array element scanf (e.g., scanf("%d", &arr[i]);)
        if (strstr(line, "&") && strstr(line, "["))
        {
            char format[10], array_name[50], index[50];
            if (sscanf(line, "scanf(\"%[^\"]\", &%[^[][%[^]]);", format, array_name, index) == 3)
            {
                print_indent();
                printf("%s[%s] = int(input())\n", array_name, index);
            }
        }
        else
        {
            char *p = strchr(line, '&');
            while (p)
            {
                char var[50];
                sscanf(p + 1, "%[^,)\n]", var);
                trim(var);
                print_indent();
                printf("%s = int(input())\n", var);
                p = strchr(p + 1, '&');
            }
        }
        return;
    }

    // Convert printf to Python print()
    if (strstr(line, "printf"))
    {
        // Handle array element in printf
        if (strstr(line, "[") && strstr(line, "]"))
        {
            char fmt[256] = {0}, array_name[50], index[50];
            if (sscanf(line, "printf(\"%[^\"]\", %[^[][%[^]]);", fmt, array_name, index) == 3)
            {
                for (int i = 0; fmt[i]; i++)
                {
                    if (fmt[i] == '%' && fmt[i + 1] == 'd')
                    {
                        fmt[i++] = '{';
                        fmt[i] = '}';
                    }
                }
                print_indent();
                printf("print(\"%s\".format(%s[%s]))\n", fmt, array_name, index);
                return;
            }
        }

        char fmt[256] = {0}, args[256] = {0};
        if (strstr(line, ",") &&
            sscanf(line, "printf(\"%[^\"]\", %[^)]);", fmt, args) == 2)
        {
            for (int i = 0; fmt[i]; i++)
            {
                if (fmt[i] == '%' && fmt[i + 1] == 'd')
                {
                    fmt[i++] = '{';
                    fmt[i] = '}';
                }
            }
            print_indent();
            printf("print(\"%s\".format(%s))\n", fmt, args);
        }
        else if (sscanf(line, "printf(\"%[^\"]\");", fmt) == 1)
        {
            print_indent();
            printf("print(\"%s\")\n", fmt);
        }
        return;
    }

    // Convert return statements
    if (strstr(line, "return "))
    {
        char value[50];
        sscanf(line, "return %[^;];", value);
        print_indent();
        printf("return %s\n", value);
        return;
    }

    // Convert exit(), break, and continue
    if (strstr(line, "exit("))
    {
        print_indent();
        printf("exit()\n");
        return;
    }
    if (strstr(line, "break;"))
    {
        print_indent();
        printf("break\n");
        return;
    }
    if (strstr(line, "continue;"))
    {
        print_indent();
        printf("continue\n");
        return;
    }

    // Convert for loops
    if (strncmp(line, "for", 3) == 0)
    {
        char *start = strchr(line, '(');
        char *end = strrchr(line, ')');
        if (start && end && end > start)
        {
            char init[100] = "", cond[100] = "", incr[100] = "";
            sscanf(start + 1, "%[^;];%[^;];%[^)]", init, cond, incr);
            trim(init);
            trim(cond);
            trim(incr);

            char var[50] = "";
            int start_val = 0, end_val = 0, step_val = 1;

            // Extract variable and starting value from init
            if (strstr(init, "int"))
                sscanf(init, "int %[^=]=%d", var, &start_val);
            else
                sscanf(init, "%[^=]=%d", var, &start_val);
            trim(var);

            // Extract end value from condition
            if (strstr(cond, "<="))
            {
                sscanf(cond, "%*[^<]<=%d", &end_val);
                end_val += 1;
            }
            else if (strstr(cond, "<"))
            {
                sscanf(cond, "%*[^<]<%d", &end_val);
            }
            else if (strstr(cond, ">="))
            {
                sscanf(cond, "%*[^>]>=%d", &end_val);
                end_val += 1;
                step_val = -1;
            }
            else if (strstr(cond, ">"))
            {
                sscanf(cond, "%*[^>]>%d", &end_val);
                step_val = -1;
            }

            // Extract step from increment
            if (strstr(incr, "++"))
            {
                step_val = 1;
            }
            else if (strstr(incr, "--"))
            {
                step_val = -1;
            }
            else if (strstr(incr, "+="))
            {
                sscanf(incr, "%*[^+=]+= %d", &step_val);
            }
            else if (strstr(incr, "-="))
            {
                sscanf(incr, "%*[^-=]-= %d", &step_val);
                step_val = -step_val;
            }

            // Print Python-style for loop
            print_indent();
            if (step_val == 1)
                printf("for %s in range(%d, %d):\n", var, start_val, end_val);
            else
                printf("for %s in range(%d, %d, %d):\n", var, start_val, end_val, step_val);
            indent_level++;
            in_for_loop = true;
        }
        return;
    }

    // Convert while loops
    if (strncmp(line, "while", 5) == 0)
    {
        char *start = strchr(line, '(');
        char *end = strrchr(line, ')');
        if (start && end && end > start)
        {
            int len = end - start - 1;
            char cond[200];
            strncpy(cond, start + 1, len);
            cond[len] = '\0';
            trim(cond);
            print_indent();
            printf("while %s:\n", cond);
            indent_level++;
        }
        return;
    }

    // Ignore opening braces (unless they're after a control structure)
    if (strchr(line, '{') && !in_for_loop)
    {
        return;
    }

    // Reduce indentation for closing braces
    if (strchr(line, '}'))
    {
        if (in_for_loop)
        {
            in_for_loop = false;
        }
        indent_level--;
        return;
    }

    // Convert ternary operator a = (x > y) ? x : y;
    if (strstr(line, "?"))
    {
        char left[100], cond[100], t[50], f[50];
        if (sscanf(line, "%[^=]= (%[^?]) ? %[^:]: %[^;];", left, cond, t, f) == 4)
        {
            trim(left);
            trim(cond);
            trim(t);
            trim(f);
            print_indent();
            printf("%s = %s if %s else %s\n", left, t, cond, f);
        }
        return;
    }

    // Comment about switch (not supported yet)
    if (strstr(line, "switch"))
    {
        char var[50];
        sscanf(line, "switch (%[^)])", var);
        trim(var);
        print_indent();
        printf("# switch(%s): use match-case or if-elif\n", var);
        return;
    }

    // General assignment like x = 5;
    if (strchr(line, '='))
    {
        char var[50], val[50];
        if (sscanf(line, "%[^=]=%[^;];", var, val) == 2)
        {
            trim(var);
            trim(val);
            print_indent();
            printf("%s = %s\n", var, val);
            return;
        }
    }

    // Handle standalone statements (like i++ or function calls)
    if (strchr(line, ';') && !in_for_loop)
    {
        char stmt[100];
        sscanf(line, "%[^;]", stmt);
        trim(stmt);
        print_indent();
        printf("%s\n", stmt);
    }
    else if (in_for_loop)
    {
        // Handle statements inside for loop body
        char stmt[100];
        sscanf(line, "%[^;]", stmt);
        trim(stmt);
        print_indent();
        printf("%s\n", stmt);
    }
}

// Entry point
int main(int argc, char *argv[])
{
    FILE *file = NULL;

    // If a file is not provided
    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s <input_file.c>\n", argv[0]);
        return 1;
    }

    file = fopen(argv[1], "r");
    if (!file)
    {
        fprintf(stderr, "Error: Cannot open file %s\n", argv[1]);
        return 1;
    }

    char line[1024];
    while (fgets(line, sizeof(line), file))
    {
        // Remove trailing newline
        line[strcspn(line, "\n")] = 0;
        translate(line);
    }

    fclose(file);

    return 0;
}