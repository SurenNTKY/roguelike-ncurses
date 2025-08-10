#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "user.h"

int check_username(const char *username)
{
    FILE *file = fopen("users.txt", "r");
    if (file == NULL)
    {
        return 0;
    }
    char line[USERNAME_MAX_LENGTH + PASSWORD_MIN_LENGTH + EMAIL_MAX_LENGTH + 3];
    while (fgets(line, sizeof(line), file))
    {
        char existing_username[USERNAME_MAX_LENGTH];
        sscanf(line, "%s", existing_username);
        if (strcmp(existing_username, username) == 0)
        {
            fclose(file);
            return 1;
        }
    }
    fclose(file);
    return 0;
}
int check_password_login(const char *username, const char *password)
{
    FILE *file = fopen("users.txt", "r");
    if (file == NULL)
    {
        return 0;
    }
    char line[USERNAME_MAX_LENGTH + PASSWORD_MIN_LENGTH + EMAIL_MAX_LENGTH + 3];
    while (fgets(line, sizeof(line), file))
    {
        char existing_username[USERNAME_MAX_LENGTH];
        char existing_password[PASSWORD_MIN_LENGTH * 2];
        sscanf(line, "%s %s", existing_username, existing_password);
        if (strcmp(existing_username, username) == 0 && strcmp(existing_password, password) == 0)
        {
            fclose(file);
            return 1;
        }
    }
    fclose(file);
    return 0;
}

int check_password(const char *password)
{
    if (strlen(password) < PASSWORD_MIN_LENGTH)
        return 0;

    int has_digit = 0, has_upper = 0, has_lower = 0;
    for (size_t i = 0; i < strlen(password); i++)
    {
        if (isdigit(password[i]))
            has_digit = 1;
        if (isupper(password[i]))
            has_upper = 1;
        if (islower(password[i]))
            has_lower = 1;
    }

    return has_digit && has_upper && has_lower;
}

int check_email(const char *email)
{
    int at_count = 0;
    for (size_t i = 0; i < strlen(email); i++)
    {
        if (email[i] == '@')
            at_count++;
    }
    return at_count == 1 && strchr(email, '.') != NULL;
}

int create_user(const char *username, const char *password, const char *email)
{
    FILE *file = fopen("users.txt", "a");
    if (file == NULL)
    {
        return 0;
    }

    time_t now = time(NULL);
    if (now == (time_t)(-1))
    {
        fclose(file);
        return 0;
    }

    char time_str[20];
    struct tm *tm_info = localtime(&now);
    strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", tm_info);

    fprintf(file, "%s %s %s %s\n", username, password, email, time_str);
    fclose(file);

    return 1;
}

void generate_random_password(char *password)
{
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    int length = 8;
    for (int i = 0; i < length; i++)
    {
        password[i] = charset[rand() % (sizeof(charset) - 1)];
    }
    password[length] = '\0';
}
