#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <conio.h>
#include <dirent.h>

void cat(char *input_name)
{
    FILE *input_pt;
    input_pt = fopen(input_name, "r");
    char c;
    while (!feof(input_pt))
    {
        c = fgetc(input_pt);
        printf("%c", c);
    }
    fclose(input_pt);
    printf("\nDone!");
}
// ok
void create_file(char *input_name, int m) // m : 1 = using to creat | 0 = using for undo
{
    int i, count, j, k;
    char save[100];
    char address[1000];
    memset(address, 0, 1000);
    count = 0;
    for (i = 0; i < strlen(input_name); i++)
    {
        if (input_name[i] == '/')
            count++;
    }
    j = 0;
    k = 0;
    while (j < count)
    {
        memset(save, 0, 100);
        for (i = 0; i < 100; i++)
        {
            if (input_name[i + k] != '/')
            {
                save[i] = input_name[i + k];
            }
            else
            {
                j++;
                k += i + 1;
                strcat(address, save);
                mkdir(address);
                strcat(address, "/");
                break;
            }
        }
    }
    memset(save, 0, 100);
    j = 0;
    for (i = strlen(input_name) - 1; input_name[i] != '/'; i--)
    {
        save[j] = input_name[i];
        j++;
    }
    char temp[strlen(save)];
    for (j = 0; j < strlen(save); j++)
    {
        temp[j] = save[strlen(save) - 1 - j];
    }
    strcat(address, temp);
    FILE *input_pt;
    if (input_pt = fopen(address, "r"))
    {
        fclose(input_pt);
        if (m == 1)
            printf("The file already exists");
    }
    else
    {
        input_pt = fopen(address, "w");
        fclose(input_pt);
    }
}
// ok
void create_backup(char *input_name)
{
    char backup_address[strlen(input_name)];
    strcpy(backup_address, input_name);
    backup_address[0] = 'u';
    backup_address[1] = 'n';
    backup_address[2] = 'd';
    backup_address[3] = 'o';
    create_file(backup_address, 0);
    FILE *input_pt;
    FILE *backupfile_pt;
    input_pt = fopen(input_name, "r");
    backupfile_pt = fopen(backup_address, "w");
    char c;
    c = fgetc(input_pt);
    while (c != EOF)
    {
        fputc(c, backupfile_pt);
        c = fgetc(input_pt);
    }
    fclose(input_pt);
    fclose(backupfile_pt);
}
// ok
void prev_text(int line_num, int char_num, char *input_name, char prev[10000])
{
    FILE *input_pt;
    input_pt = fopen(input_name, "r");
    char save[100];
    int i;

    // save prev text
    memset(prev, 0, 10000);
    for (i = 1; i < line_num; i++)
    {
        fgets(save, 100, input_pt);
        strcat(prev, save);
    }
    memset(save, 0, 100);
    fgets(save, 100, input_pt);
    int prev_len = strlen(prev);
    for (i = 0; i < char_num; i++)
    {
        if ((save[i] != '\n') && (save[i] != '\0'))
        {
            prev[prev_len + i] = save[i];
        }
        else
        {
            prev[prev_len + i] = ' ';
        }
    }
    fclose(input_pt);
    // printf("%s", prev);
}
// ok
void next_text(int line_num, int char_num, char *input_name, char next[10000])
{
    FILE *input_pt;
    input_pt = fopen(input_name, "r");
    char save[100];
    char c;
    int i, j;

    for (i = 1; i < line_num; i++)
    {
        fgets(save, 100, input_pt);
    }
    memset(save, 0, 100);
    fgets(save, 100, input_pt);
    j = 0;
    memset(next, 0, 10000);
    int save_len = strlen(save);
    if (save[save_len - 1] == '\n')
    {
        if (save_len <= char_num)
        {
            next[j] = '\n';
            j++;
            while (!feof(input_pt))
            {
                next[j] = fgetc(input_pt);
                j++;
            }
        }
        else
        {
            for (i = char_num; i < save_len; i++)
            {
                next[i - char_num] = save[i];
            }
            j = save_len - char_num;
            c = fgetc(input_pt);
            while (!feof(input_pt))
            {
                next[j] = c;
                j++;
                c = fgetc(input_pt);
            }
        }
    }
    fclose(input_pt);
}
// ok
void insertstr(int line_num, int char_num, char *input_name, char *text)
{
    create_backup(input_name); // for undo
    FILE *input_pt;
    char prev[10000];
    char next[10000];
    prev_text(line_num, char_num, input_name, prev);
    next_text(line_num, char_num, input_name, next);
    input_pt = fopen(input_name, "w");
    fprintf(input_pt, "%s%s%s", prev, text, next);
    fclose(input_pt);
}
// ok
void removestr(char *input_name, int size, int line_num, int char_num, int mode) // mode: 1 = forward | 0 = backward
{
    create_backup(input_name); // for undo
    FILE *input_pt;
    char prev[10000];
    char next[10000];
    char new_prev[10000];
    char new_next[10000];
    char c;
    int i;
    prev_text(line_num, char_num, input_name, prev);
    next_text(line_num, char_num, input_name, next);
    if (mode == 1)
    {
        int next_len = strlen(next);
        for (i = 0; i <= next_len; i++)
        {
            new_next[i] = next[size + i];
        }
        input_pt = fopen(input_name, "w");
        fprintf(input_pt, "%s%s", prev, new_next);
        fclose(input_pt);
    }
    if (mode == 0)
    {
        int prev_len = strlen(prev);
        for (i = 0; i < prev_len - size; i++)
        {
            new_prev[i] = prev[i];
        }
        input_pt = fopen(input_name, "w");
        fprintf(input_pt, "%s%s", new_prev, next);
        fclose(input_pt);
    }
}
// ok
void copystr(char *input_name, int size, int line_num, int char_num, int mode) // mode: 1 = forward | 0 = backward
{
    FILE *input_pt;
    char prev[10000];
    char next[10000];
    char text[size];
    char c;
    int i;
    prev_text(line_num, char_num, input_name, prev);
    next_text(line_num, char_num, input_name, next);
    if (mode == 1)
    {
        for (i = 0; i < size; i++)
        {
            text[i] = next[i];
        }
    }
    if (mode == 0)
    {
        int prev_len = strlen(prev);
        for (i = 0; i < size; i++)
        {
            text[size - 1 - i] = prev[prev_len - i - 1];
        }
    }
    OpenClipboard(0);
    EmptyClipboard();
    const size_t text_len = strlen(text) + 1;
    HGLOBAL h = GlobalAlloc(GMEM_MOVEABLE, text_len);
    memcpy(GlobalLock(h), text, text_len);
    GlobalUnlock(h);
    SetClipboardData(CF_TEXT, h);
    CloseClipboard();
}
// ok
void cutstr(char *input_name, int size, int line_num, int char_num, int mode) // mode: 1 = forward | 0 = backward
{
    create_backup(input_name); // for undo
    copystr(input_name, size, line_num, char_num, mode);
    removestr(input_name, size, line_num, char_num, mode);
}
// ok
void pastestr(char *input_name, int line_num, int char_num)
{
    OpenClipboard(0);
    HANDLE text = GetClipboardData(CF_TEXT);
    CloseClipboard();
    insertstr(line_num, char_num, input_name, text);
}
// ok
int number_of_lines(char *input_name)
{
    int n = 0;
    char c;
    FILE *input_pt;
    input_pt = fopen(input_name, "r");
    c = fgetc(input_pt);
    while (c != EOF)
    {
        if (c == '\n')
            n++;
        c = fgetc(input_pt);
    }
    fclose(input_pt);
    n++;
    return n;
}
// ok
void compare(char *input_name1, char *input_name2)
{
    int line_num1 = number_of_lines(input_name1);
    int line_num2 = number_of_lines(input_name2);
    int i, j, k, min;
    char save1[1000];
    char save2[1000];
    FILE *input_pt1;
    input_pt1 = fopen(input_name1, "r");
    FILE *input_pt2;
    input_pt2 = fopen(input_name2, "r");
    min = line_num1;
    if (min > line_num2)
        min = line_num2;
    for (i = 1; i <= min; i++)
    {
        fgets(save1, 1000, input_pt1);
        fgets(save2, 1000, input_pt2);
        if (i == min)
        {
            if (min == line_num1)
                strcat(save1, "\n");
            if (min == line_num2)
                strcat(save2, "\n");
        }
        if (strlen(save1) != strlen(save2))
        {
            printf("==== %d ====\n%s%s", i, save1, save2);
        }
        else
        {
            for (j = 0; j < strlen(save1); j++)
            {
                if (save1[j] != save2[j])
                {
                    printf("==== %d ====\n%s%s", i, save1, save2);
                    break;
                }
            }
        }
    }

    if ((min == line_num1) && (line_num1 != line_num2))
    {
        printf(">>>> #%d - #%d >>>>\n", (line_num1 + 1), line_num2);
        k = line_num2 - line_num1;
        memset(save2, 1000, 0);
        for (i = 1; i <= k; i++)
        {
            fgets(save2, 1000, input_pt2);
            printf("%s", save2);
        }
    }
    if ((min == line_num2) && (line_num1 != line_num2))
    {
        printf("\n>>>> #%d - #%d >>>>\n", (line_num2 + 1), line_num1);
        k = line_num1 - line_num2;
        memset(save1, 1000, 0);
        for (i = 1; i <= k; i++)
        {
            fgets(save1, 1000, input_pt1);
            printf("%s", save1);
        }
    }
}
// ok
void tree(char *input_name, int depth)
{
    int i;
    char address[100];
    DIR *d;
    struct dirent *dir;
    d = opendir(input_name);
    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
            if (!strcmp(dir->d_name, ".") || !strcmp(dir->d_name, ".."))
                continue;
            if (depth == 2 && dir->d_type == DT_DIR)
            {
                printf("%s\n", dir->d_name);
                memset(address, 0, 100);
                strcat(address, "root/");
                strcat(address, dir->d_name);
                tree(address, 1);
            }
            else
            {
                if (2 - depth)
                    printf("|___%s\n", dir->d_name);
                else
                    printf("%s\n", dir->d_name);
            }
        }
        closedir(d);
    }
}
// bug
void undo(char *input_name)
{
    // make temp file
    char temp_address[strlen(input_name)];
    strcpy(temp_address, input_name);
    temp_address[0] = 't';
    temp_address[1] = 'e';
    temp_address[2] = 'm';
    temp_address[3] = 'p';
    create_file(temp_address, 0);
    FILE *input_pt;
    FILE *tempfile_pt;
    input_pt = fopen(input_name, "r");
    tempfile_pt = fopen(temp_address, "w");
    char c;
    c = fgetc(input_pt);
    while (c != EOF)
    {
        fputc(c, tempfile_pt);
        c = fgetc(input_pt);
    }
    fclose(input_pt);
    fclose(tempfile_pt);
    // open backup
    char backup_address[strlen(input_name)];
    strcpy(backup_address, input_name);
    backup_address[0] = 'u';
    backup_address[1] = 'n';
    backup_address[2] = 'd';
    backup_address[3] = 'o';
    FILE *backupfile_pt;
    backupfile_pt = fopen(backup_address, "r");
    input_pt = fopen(input_name, "w");
    // write backup in main file
    c = fgetc(backupfile_pt);
    while (c != EOF)
    {
        fputc(c, input_pt);
        c = fgetc(backupfile_pt);
    }
    fclose(input_pt);
    fclose(backupfile_pt);
    // write temp in backup file
    backupfile_pt = fopen(backup_address, "w");
    tempfile_pt = fopen(temp_address, "r");
    c = fgetc(tempfile_pt);
    while (c != EOF)
    {
        fputc(c, backupfile_pt);
        c = fgetc(tempfile_pt);
    }
    fclose(tempfile_pt);
    fclose(backupfile_pt);
}
// ok