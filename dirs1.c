#include <sys/types.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/stat.h>



// readdir opendir closedir stat
void dfs(int deep, char* filename, char* curDir);

int main()
{
    char path[1000];
    char filename[1000] = "dirs1.c";
    int deep = 5;
    realpath("./", path);
//   ile ((dirStruct = readdir(dir)) != NULL)
        //printf("%s\n", dirStruct->d_name);
//    printf("Enter name of searching file: ");
//    scanf("%s", filename);

//    printf("Enter searching deep: ");
//    scanf("%d", &deep);
    dfs(deep, filename, path);

    /*
    DIR* dir = opendir(".");
    struct dirent* dirStruct;
    struct stat mRestrict;
    while ((dirStruct = readdir(dir)) != NULL)
    {
        char fullpath[300] = "./";
        strcat(fullpath, dirStruct->d_name);
        if ((stat(fullpath, &mRestrict) == -1))
        {
                printf("Something is going wrong\n");
                continue;
        }

        char str[300];
        switch(dirStruct->d_type)
        {
            case DT_BLK: strcpy(str, "block device.");
        break;
            case DT_CHR: strcpy(str, "character device");
        break;
            case DT_DIR: strcpy(str, "directory");
        break;
            case DT_LNK: strcpy(str, "symbolic link");
        break;
            case DT_REG: strcpy(str, "regular file");
        break;
            case DT_UNKNOWN: strcpy(str, "unknown file type");
        break;
        }
           printf("name: %s "
                  "size: %lu bytes, "
                  "inode number: %lu, "
                  "length of record: %hu, "
                  "type: %s \n\n\n",
                  dirStruct->d_name, mRestrict.st_size, dirStruct->d_ino, dirStruct->d_reclen, str);


    }
*/



}
void dfs(int deep, char* filename, char* curDir)
{
    if (deep == 0)
        return;
    DIR* dir = opendir(curDir);
    struct dirent* dirStruct;
    while ((dirStruct = readdir(dir)) != NULL)
    {
        if (dirStruct->d_type == DT_DIR && strcmp(dirStruct->d_name, ".") && strcmp(dirStruct->d_name, ".."))
        {
            char newDirpath[400] = "";
            strcpy(newDirpath, curDir);
            strcat(newDirpath, "/");
            strcat(newDirpath, dirStruct->d_name);
            dfs(deep - 1, filename, newDirpath);
        } else if (dirStruct->d_type == DT_REG && !strcmp(dirStruct->d_name, filename))
        {
            char newDirpath[400];
            strcpy(newDirpath, curDir);
            strcat(newDirpath, "/");
            strcat(newDirpath, dirStruct->d_name);
            printf("My congratulations! %s \n", newDirpath);
        }
    }
    return;
}
