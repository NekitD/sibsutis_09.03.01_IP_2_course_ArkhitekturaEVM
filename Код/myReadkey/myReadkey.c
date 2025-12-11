#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <string.h>
#include <ctype.h>
#include <poll.h>
#include "myReadkey.h"
#include "mySimpleComputer.h"
#include "Enum.h"
#include "MyTerm.h"
#include "CU.h"
#include <signal.h>
#include <stdbool.h>

static struct termios original_termios;

int rk_readkey(enum keys *address)
{
    if (rk_mytermregime(1, 0, 1, 0, 0) != 0)
        return -1;
    tcflush(STDIN_FILENO, TCIFLUSH);

    char buf[8] = {0};
    int bytes_read = read(STDIN_FILENO, buf, sizeof(buf));

    if (bytes_read <= 0)
    {
        return -1;
    }

    if (bytes_read == 1)
    {
        switch (buf[0])
        {
        case 'l':
            *address = l;
            break;
        case 's':
            *address = s;
            break;
        case 'i':
            *address = i;
            break;
        case 'r':
            *address = r;
            break;
        case 't':
            *address = t;
            break;
        case '\x1B':
            *address = ESC;
            break;
        case '\n':
        case '\r':
            *address = ENTER;
            break;
        default:
            return -1;
        }
    }
    else if (bytes_read >= 3)
    {

        if (strncmp(buf, "\x1B[15~", 5) == 0)
        {
            *address = F5;
        }
        else if (strncmp(buf, "\x1B[17~", 5) == 0)
        {
            *address = F6;
        }
        else if (strncmp(buf, "\x1B[A", 3) == 0)
        {
            *address = UP;
        }
        else if (strncmp(buf, "\x1B[B", 3) == 0)
        {
            *address = DOWN;
        }
        else if (strncmp(buf, "\x1B[C", 3) == 0)
        {
            *address = RIGHT;
        }
        else if (strncmp(buf, "\x1B[D", 3) == 0)
        {
            *address = LEFT;
        }
        else
        {
            return -1;
        }
    }
    else
    {
        return -1;
    }
    return 0;
}

int rk_mytermsave(void)
{
    char filename[256] = {0};
    char full_path[512] = {0};
    struct termios old_term, new_term;

    if (tcgetattr(STDIN_FILENO, &original_termios) != 0)
    {
        return -1;
    }

    old_term = original_termios;
    new_term = old_term;
    new_term.c_lflag |= (ICANON | ECHO);
    if (tcsetattr(STDIN_FILENO, TCSANOW, &new_term) != 0)
    {
        return -1;
    }

    mt_gotoXY(30, 0);
    mt_delline();
    printf("Введите имя файла для сохранения: ");
    fflush(stdout);

    if (fgets(filename, sizeof(filename), stdin) == NULL)
    {
        tcsetattr(STDIN_FILENO, TCSANOW, &old_term);
        rk_mytermregime(1, 0, 1, 0, 1);
        return -1;
    }
    alarm(5);
    pause();
    IRC(SIGALRM);
    filename[strcspn(filename, "\n")] = '\0';
    snprintf(full_path, sizeof(full_path), "saves/%s.bin", filename);

    FILE *file = fopen(full_path, "wb");
    if (file == NULL)
    {
        mt_gotoXY(30, 0);
        mt_delline();
        printf("Ошибка открытия файла для записи!");
        fflush(stdout);
        tcsetattr(STDIN_FILENO, TCSANOW, &old_term);
        rk_mytermregime(1, 0, 1, 0, 1);
        return -1;
    }

    fwrite(MEMORY, sizeof(int), MEMORY_SIZE, file);
    fclose(file);
    tcsetattr(STDIN_FILENO, TCSANOW, &old_term);
    mt_gotoXY(30, 0);
    mt_delline();
    printf("Сохранено в файл: %s", filename);
    fflush(stdout);
    rk_mytermregime(1, 0, 1, 0, 1);
    return 0;
}

int rk_mytermrestore(void)
{
    char filename[256] = {0};
    char full_path[512] = {0};
    struct termios old_term, new_term;

    if (tcgetattr(STDIN_FILENO, &old_term) != 0)
    {
        return -1;
    }
    new_term = old_term;
    new_term.c_lflag |= (ICANON | ECHO);

    if (tcsetattr(STDIN_FILENO, TCSANOW, &new_term) != 0)
    {
        return -1;
    }

    mt_gotoXY(30, 0);
    mt_delline();
    printf("Введите имя файла для загрузки: ");
    fflush(stdout);

    if (fgets(filename, sizeof(filename), stdin) == NULL)
    {
        tcsetattr(STDIN_FILENO, TCSANOW, &old_term);
        return -1;
    }
    alarm(5);
    pause();
    filename[strcspn(filename, "\n")] = '\0';
    snprintf(full_path, sizeof(full_path), "saves/%s.bin", filename);

    FILE *file = fopen(full_path, "rb");
    if (file == NULL)
    {
        mt_gotoXY(30, 0);
        mt_delline();
        printf("Файл %s не найден!", filename);
        fflush(stdout);
        tcsetattr(STDIN_FILENO, TCSANOW, &old_term);
        return -1;
    }

    if (fread(MEMORY, sizeof(int), MEMORY_SIZE, file) != MEMORY_SIZE)
    {
        mt_gotoXY(30, 0);
        mt_delline();
        printf("Ошибка чтения данных!");
        fclose(file);
        tcsetattr(STDIN_FILENO, TCSANOW, &old_term);
        return -1;
    }
    fclose(file);

    if (tcsetattr(STDIN_FILENO, TCSANOW, &original_termios) != 0)
    {
        return -1;
    }
    mt_gotoXY(30, 0);
    mt_delline();
    printf("Загружено из файла: %s", filename);
    fflush(stdout);
    rk_mytermregime(1, 0, 1, 0, 1);
    return 0;
}

int rk_mytermregime(int regime, int vtime, int vmin, int echo, int sigint)
{
    struct termios new_termios;

    if (tcgetattr(STDIN_FILENO, &new_termios) != 0)
    {
        return -1;
    }
    if (regime)
    {
        new_termios.c_lflag &= ~(ICANON | ECHO | (sigint ? 0 : ISIG));
        new_termios.c_cc[VTIME] = vtime;
        new_termios.c_cc[VMIN] = vmin;
    }
    else
    {
        new_termios.c_lflag |= (ICANON | (echo ? ECHO : 0) | (sigint ? ISIG : 0));
    }
    return tcsetattr(STDIN_FILENO, TCSANOW, &new_termios);
}

int rk_readvalue(int *value, int timeout)
{
    char input[6] = {0};
    int pos = 0;
    struct pollfd fds = {STDIN_FILENO, POLLIN, 0};

    if (rk_mytermregime(1, 0, 1, 1, 0) != 0)
    {
        return -1;
    }

    while (1)
    {
        int poll_result = poll(&fds, 1, timeout * 1000);
        if (poll_result <= 0)
            break;

        char c;
        if (read(STDIN_FILENO, &c, 1) != 1)
            break;

        c = toupper(c);

        if (c == '\n' || c == '\r')
        {
            if (pos > 0)
            {
                input[pos] = '\0';
                int is_negative = (input[0] == '-');
                char *endptr;
                unsigned long hex_value = strtoul(is_negative ? input + 1 : input, &endptr, 16);

                if (*endptr != '\0' || hex_value > 0x7FFF)
                    break;

                unsigned int command = (hex_value >> 8) & 0x7F;
                unsigned int operand = hex_value & 0x7F;
                unsigned int full_value = (is_negative << 14) | (command << 7) | operand;

                if (is_negative)
                {
                    full_value = (full_value - 1) & 0x7FFF;
                    full_value = ~full_value & 0x7FFF;
                    *value = -(full_value);
                }
                else
                {
                    *value = full_value;
                }
                return 0;
            }
            break;
        }
        else if (c == 127 || c == 8)
        {
            if (pos > 0)
            {
                pos--;
                printf("\b \b");
                fflush(stdout);
            }
        }
        else if (pos < 5)
        {
            if ((pos == 0 && (c == '-' || c == '+')) || (pos > 0 && ((c >= '0' && c <= '9') || (c >= 'A' && c <= 'F'))))
            {
                input[pos++] = c;
                printf("%c", c);
                fflush(stdout);
            }
        }
    }

    tcflush(STDIN_FILENO, TCIFLUSH);
    rk_mytermregime(1, 0, 1, 0, 0);
    return -1;
}
