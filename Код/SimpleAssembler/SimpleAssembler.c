#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <libgen.h>
#include <limits.h>

#define MEM_SIZE 128
#define MAX_PATH 256
#define MIN_VALUE -16383
#define MAX_VALUE 16383

typedef struct
{
    char *name;
    int code;
} Command;

Command commands[] = {
    {"NOP", 0x00}, {"CPUINFO", 0x01}, {"READ", 0x0A}, {"WRITE", 0x0B}, {"LOAD", 0x14}, {"STORE", 0x15}, {"ADD", 0x1E}, {"SUB", 0x1F}, {"DIVIDE", 0x20}, {"MUL", 0x21}, {"JUMP", 0x28}, {"JNEG", 0x29}, {"JZ", 0x2A}, {"HALT", 0x2B}, {"NOT", 0x33}, {"AND", 0x34}, {"OR", 0x35}, {"XOR", 0x36}, {"JNS", 0x37}, {"JC", 0x38}, {"JNC", 0x39}, {"JP", 0x3A}, {"JNP", 0x3B}, {"CHL", 0x3C}, {"SHR", 0x3D}, {"RCL", 0x3E}, {"RCR", 0x3F}, {"NEG", 0x40}, {"ADDC", 0x41}, {"SUBC", 0x42}, {"LOGLC", 0x43}, {"LOGRC", 0x44}, {"RCCL", 0x45}, {"RCCR", 0x46}, {"MOVA", 0x47}, {"MOVR", 0x48}, {"MOVCA", 0x49}, {"MOVCR", 0x4A}, {"ADDC", 0x4B}, {"SUBC", 0x4C}};

int memory[MEM_SIZE] = {0};

int find_command_code(const char *name)
{
    for (size_t i = 0; i < sizeof(commands) / sizeof(commands[0]); i++)
    {
        if (strcmp(commands[i].name, name) == 0)
        {
            return commands[i].code;
        }
    }
    return -1;
}

int parse_number(const char *str, int *success)
{
    int sign = 1;
    int value = 0;
    *success = 1;

    if (*str == '+')
    {
        str++;
    }
    else if (*str == '-')
    {
        sign = -1;
        str++;
    }

    if (!isdigit(*str))
    {
        *success = 0;
        return 0;
    }

    while (*str && isdigit(*str))
    {
        value = value * 10 + (*str - '0');
        str++;
    }

    if (*str != '\0' && !isspace(*str) && *str != ';')
    {
        *success = 0;
    }

    int result = sign * value;

    if (result < MIN_VALUE || result > MAX_VALUE)
    {
        *success = 0;
    }

    return result;
}

void process_line(char *line, int line_num)
{
    char *comment = strchr(line, ';');
    if (comment)
        *comment = '\0';

    char *end = line + strlen(line) - 1;
    while (end >= line && isspace(*end))
        end--;
    *(end + 1) = '\0';

    if (strlen(line) == 0)
        return;

    char *token = strtok(line, " \t");
    if (!token)
        return;

    int address;
    if (sscanf(token, "%d", &address) != 1)
    {
        fprintf(stderr, "Line %d: Invalid address format\n", line_num);
        return;
    }

    if (address < 0 || address >= MEM_SIZE)
    {
        fprintf(stderr, "Line %d: Address %d out of range (0-%d)\n", line_num, address, MEM_SIZE - 1);
        return;
    }

    token = strtok(NULL, " \t");
    if (!token)
    {
        fprintf(stderr, "Line %d: Missing command or '='\n", line_num);
        memory[address] = 0;
        return;
    }

    if (strcmp(token, "=") == 0)
    {
        token = strtok(NULL, " \t");
        if (!token)
        {
            fprintf(stderr, "Line %d: Missing value after '='\n", line_num);
            memory[address] = 0;
            return;
        }

        int success;
        int value = parse_number(token, &success);
        if (!success)
        {
            fprintf(stderr, "Line %d: Invalid number format or out of range '%s' (must be between %d and %d)\n", line_num, token, MIN_VALUE, MAX_VALUE);
            memory[address] = 0;
            return;
        }

        memory[address] = value;
    }
    else
    {
        int command_code = find_command_code(token);
        if (command_code == -1)
        {
            fprintf(stderr, "Line %d: Unknown command '%s'\n", line_num, token);
            memory[address] = 0;
            return;
        }

        token = strtok(NULL, " \t");
        if (!token)
        {
            fprintf(stderr, "Line %d: Missing operand for command\n", line_num);
            memory[address] = 0;
            return;
        }

        int success;
        int operand = parse_number(token, &success);
        if (!success)
        {
            fprintf(stderr, "Line %d: Invalid operand format or out of range '%s' (must be between %d and %d)\n", line_num, token, MIN_VALUE, MAX_VALUE);
            memory[address] = 0;
            return;
        }

        if (operand < 0 || operand >= MEM_SIZE)
        {
            fprintf(stderr, "Line %d: Operand %d out of range (0-%d)\n", line_num, operand, MEM_SIZE - 1);
            memory[address] = 0;
            return;
        }

        memory[address] = (command_code << 7) | (operand & 0x7F);
    }
}

void write_binary(const char *filename)
{
    FILE *f = fopen(filename, "wb");
    if (!f)
    {
        perror("Error opening output file");
        exit(1);
    }

    for (int i = 0; i < MEM_SIZE; i++)
    {
        int word = memory[i];
        fwrite(&word, sizeof(int), 1, f);
    }

    fclose(f);
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Usage: %s input.sa output.bin\n", argv[0]);
        return 1;
    }

    memset(memory, 0, sizeof(memory));

    FILE *f = fopen(argv[1], "r");
    if (!f)
    {
        perror("Error opening input file");
        return 1;
    }

    char line[256];
    int line_num = 0;
    while (fgets(line, sizeof(line), f))
    {
        line_num++;
        process_line(line, line_num);
    }

    fclose(f);

    char input_path[MAX_PATH];
    char output_path[MAX_PATH];
    char *base;

    realpath(argv[1], input_path);
    base = basename(input_path);

    char *dot = strrchr(base, '.');
    if (dot && strcmp(dot, ".sa") == 0)
    {
        *dot = '\0';
    }

    snprintf(output_path, MAX_PATH, "../console/saves/%s.bin", base);

    char cmd[MAX_PATH + 10];
    snprintf(cmd, sizeof(cmd), "mkdir -p ../console/saves");
    system(cmd);

    write_binary(argv[2]);
    printf("Successfully compiled to %s\n", argv[2]);

    return 0;
}
