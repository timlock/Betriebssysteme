#include <iostream>
#include <dirent.h>
#include <sys/stat.h>
#include <vector>

using namespace std;


int main(int argc, char *argv[]) {
    char *directory = argv[1];
    char *params = nullptr;
    int paramCount = argc - 2;
    if (argc < 2) params = argv[2];
    DIR *dir = opendir(directory);
    dirent *file = nullptr;
    do {
        file = readdir(dir);
        char *path = nullptr;
        if(file) {
            path = file->d_name;

            struct stat buf;
            if (lstat(path, &buf) != 0) {
                cout << "lstat Fehler\n";
                exit(1);
            }
            mode_t m = buf.st_mode;
            char* fileType = "-";
            if(S_ISDIR(m) != 0) fileType ="d";
            if(S_ISCHR(m) != 0) fileType ="c";
            if(S_ISBLK(m) != 0) fileType ="b";
            if(S_ISFIFO(m) != 0) fileType ="p";
            if(S_ISSOCK(m) != 0) fileType ="s";
            cout << fileType;
            if(S_IRUSR & m) cout << "r";
            else cout << "-";
            if(S_IWUSR & m) cout << "w";
            else cout << "-";
            
            cout << buf.st_nlink << " " << buf.st_uid << " " << buf.st_gid << " " << buf.st_size << " " << buf.st_atime << " " << path << endl;

        }
    } while (file);



    int closeDir = closedir(dir);
}