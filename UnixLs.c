#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> //getopt
#include <sys/stat.h>
#include <time.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <fcntl.h>
#include <pwd.h>
#include <stdbool.h>
#include <grp.h>
#include <sys/types.h>
#define MAX_INODE_RECORD 10000

#define MAX_INODE_PER_LINE 6
#define MAX_LIST_DIRENTORY 100
#define MAX_NAMELEN 4096

bool list_flag = false;
bool inode_flag = false;
bool recursive_flag = false;
const char default_dir[] = ".";
const char monthStr[][6] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

void translate_stat(struct stat *ptr);
void list_directory(const char *path, bool recursive);
char *read_link(const char *path);

int main(int argc, char **argv)
{
    //read input

    int i = 1;
    int items = 0;
    const char *dir_list[MAX_LIST_DIRENTORY];
    for (; i < argc; i++)
    {
        if (argv[i][0] != '-')
        {
            dir_list[items++] = argv[i];
        }
    }
    if (items == 0)
    {
        dir_list[items++] = default_dir;
    }
    int opt;
    while ((opt = getopt(argc, argv, "liR")) != -1)
    {
        switch (opt)
        {
        case 'l':
            list_flag = true;
            break;
        case 'i':
            inode_flag = true;
            break;
        case 'R':
            recursive_flag = true;
            break;
        default: /* '?' */
            fprintf(stderr, "Usage: %s [-l -i -R] [directory]\n",
                    argv[0]);
            exit(EXIT_FAILURE);
        }
    }

    //list directory recursively
    for (i = 0; i < items; i++)
    {
        //printf("%s\n", dir_list[i]);
        if (items > 1 && !recursive_flag)
        {
            printf("%s:\n", dir_list[i]);
            ;
        }
        list_directory(dir_list[i], recursive_flag);
        printf("\n");
    }

    return 0;
}

long int inodes_num[MAX_INODE_RECORD];
char *entry_name[MAX_INODE_RECORD];

int print_len(int index)
{
    int column = 0;
    if (inode_flag) /* show inode# */
		column += printf("%7llu ", (long long)inodes_num[index]);
    column += printf("%s ", entry_name[index]);
    return column;
    
}

#define ALIGN_STANDARD //alignment adjustment foe ls -i ls -iR 
void list_directory(const char *path, bool recursive)
{
    DIR *p;
    struct dirent *entry;
    struct stat statbuf;
    int items = 0;
    int fd, i;
    char *child;
    char print_dir[MAX_NAMELEN];
    char *dfs_dirname[1000];
#ifdef ALIGN_STANDARD
    int inodes_cnt = 0;//total_output_len = 0;
#endif
    if ((p = opendir(path)) == NULL)
    {
        printf("open in failure\n");
        return;
    }
    if (recursive)
    {
        printf("%s:\n", path);
    }

#ifdef ALIGN_STANDARD
    int column_width = 0;
    if (!list_flag)//-i 
    {
        while ((entry = readdir(p)) != NULL)
        {
            if (entry->d_name[0] == '.')
                continue;
            if (entry->d_type & DT_DIR)
            {
                if (recursive)
                {
                    child = malloc(sizeof(char) * MAX_NAMELEN);
                    sprintf(child, "%s/%s", path, entry->d_name);
                    dfs_dirname[items++] = child;
                }
            }
            if (inode_flag)
            {
                inodes_num[inodes_cnt] = entry->d_ino;
            }
            int len =  strlen(entry->d_name);
            char *newchar = malloc(len + 1);
            strcpy(newchar, entry->d_name);
            entry_name[inodes_cnt++] = newchar;
            column_width = column_width > len ? column_width : len;
            //total_output_len += strlen(d_name) + 3;
        }
        closedir(p);
        column_width += 2 + ((inode_flag) ? 8 : 0);/* inode# width */
        //printf("%d\n", column_width);
        int nr_cols = (unsigned)100 / column_width;
        int nr_rows = 1;
        int row, nc;
        int next_pos = 0;
        int column = 0;
        if (nr_cols > 1)
        {
            nr_rows = inodes_cnt / nr_cols;
            if (nr_rows * nr_cols < inodes_cnt)
                nr_rows++;
        }
        else
        {
            nr_rows = inodes_cnt;
            nr_cols = 1;
        }

        for (row = 0; row < nr_rows; row++)
        {
            for (nc = 0; nc < nr_cols; nc++)
            {
                i = (nc * nr_rows) + row; 
                if (i < inodes_cnt)
                {
                    if (column > 0)
                    {
                        next_pos -= column;//print spaces
                        printf("%*s", next_pos, "");
                        column += next_pos;
                    }
                    next_pos = column_width * (nc+1); //column + column_width;
                    column += print_len(i);
                    free(entry_name[i]);
                }
            }
            putchar('\n');
            column = 0;
        }

        if (recursive) //continue list...
        {
            putchar('\n');
            for (i = 0; i < items; i++)
            {
                list_directory(dfs_dirname[i], recursive);
                free(dfs_dirname[i]);
            }
        }
        return ;
    }
    
#endif

    int total = 0;
    char *buf = NULL;
    while ((entry = readdir(p)) != NULL)
    {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;
        if (inode_flag)
        {
            //inodes_num =
            printf("%7ld ", entry->d_ino);
        }

        if (entry->d_type & DT_DIR)
        {
            if (recursive)
            {
                child = malloc(sizeof(char) * MAX_NAMELEN);
                sprintf(child, "%s/%s", path, entry->d_name);
                dfs_dirname[items++] = child;
            }
            if (list_flag)
            {
                sprintf(print_dir, "%s/%s", path, entry->d_name);
                fd = open(print_dir, O_RDONLY);
                fstat(fd, &statbuf);
                putchar('d');
                translate_stat(&statbuf);
            }
        }
        else
        {

            if (list_flag)
            {
                sprintf(print_dir, "%s/%s", path, entry->d_name);
                switch (entry->d_type)
                {
                case DT_CHR:
                    putchar('c');
                    break;
                case DT_BLK:
                    putchar('b');
                    break;
                case DT_LNK:
                    putchar('l');
                    break;
                default:
                    putchar('-');
                    break;
                }
                fd = open(print_dir, O_RDONLY);
                fstat(fd, &statbuf);
                translate_stat(&statbuf);
                close(fd);
            }
        }
        ++total;
        if (list_flag)
        {
            if (entry->d_type == DT_LNK)
            {
                printf("%s -> ", entry->d_name);
                buf = read_link(print_dir);

                if (buf != NULL)
                {
                    printf("%s\n", buf);
                    free(buf);
                }
            }

            else
                printf("%s\n", entry->d_name);
        }
        else
        {
            printf("%-15s  ", entry->d_name);
            if (total % MAX_INODE_PER_LINE == 0)
                putchar('\n');
        }
    }
    if (!list_flag)
    {
        if (total % MAX_INODE_PER_LINE)
            putchar('\n');
    }
    closedir(p);

    if (recursive)
    {
        putchar('\n');
        for (i = 0; i < items; i++)
        {
            list_directory(dfs_dirname[i], recursive);
            free(dfs_dirname[i]);
        }
    }
}
char *read_link(const char *path)
{
    char *buf = NULL;
    int bufsize = 0, readsize = 0;

    do
    {
        bufsize += 128;
        buf = realloc(buf, bufsize);
        readsize = readlink(path, buf, bufsize);
        if (readsize == -1)
        {
            free(buf);
            return NULL;
        }
        //printf("\n%d\n", readsize);
    } while (bufsize < readsize + 1);

    buf[readsize] = '\0';

    return buf;
}

bool getAndPrintGroup(gid_t grpNum, char *name)
{
    struct group *grp;

    grp = getgrgid(grpNum);

    if (grp)
    {
        strcpy(name, grp->gr_name);
        return true;
    }
    // else
    // {
    //     printf("No group name for %u found\n", grpNum);
    // }
    return false;
}

bool getAndPrintUserName(uid_t uid, char *name)
{

    struct passwd *pw = NULL;
    pw = getpwuid(uid);

    if (pw)
    {
        strcpy(name, pw->pw_name);
        return true;
        //printf("The user ID %u -> %s\n", uid, pw->pw_name);
    }

    return false;
}

void translate_stat(struct stat *ptr)
{
    int s = (unsigned int)ptr->st_mode, mask = 1 << 9;
    //putchar(mask&s?'d':'-');
    char user_name[128];
    char group_name[128];

    mask >>= 1;
    int k = 0;
    char temp[] = "rwx";
    struct tm *modify;
    while (mask)
    {
        putchar(mask & s ? temp[k] : '-');
        ++k;
        mask >>= 1;
        k = (k == 3) ? 0 : k;
    }
    printf(" %-2ld", ptr->st_nlink);
    /* user info */
    getAndPrintUserName(ptr->st_uid, user_name);
    printf(" %s", user_name);
    /* group info */
    getAndPrintGroup(ptr->st_gid, group_name);
    printf(" %s %-10ld", group_name, ptr->st_size);
    /* modify time info */
    modify = localtime(&ptr->st_mtime);
    //Oct 2 2009 13:32
    printf("%-3s %-2d %-4d %02d:%02d  ", monthStr[modify->tm_mon], modify->tm_mday, 1900 + modify->tm_year, modify->tm_hour, modify->tm_min);
}