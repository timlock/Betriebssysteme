#include <iostream>
#include <dirent.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include <grp.h>
#include <pwd.h>
#include <sstream>
#include <unistd.h>
#include <fcntl.h>


using namespace std;


int main(int argc, char *argv[]) {
//
//    bool a = true;
//    bool l = true;
//    bool g = false;
//    bool o = false;

    unsigned char magic[6] = {'u', 's', 't', 'a', 'r', '\0'};
    unsigned char header[512];

    int fd = open(argv[1], O_RDONLY);
    if (read(fd, (void *) header, 512) == -1) {
        cerr << "Lesefehler" << endl;
        exit(1);
    }
    int j = 0;

    for (int i = 257; i <
                      263; i++) { // magic number liegt zwischen 257 und 263 http://www.gnu.org/software/tar/manual/html_node/Standard.html

        if (magic[j] != header[i]) {
            cerr << "Keine tar datei" << endl;
            exit(1);
        }
        j++;
    }
    unsigned char *filename = nullptr;
    unsigned char *filemode = nullptr;
    unsigned char *owner = nullptr;
    unsigned char *group = nullptr;
    unsigned char *size = nullptr;
    unsigned char *lastmodification = nullptr;
    unsigned char *linkedFileType = nullptr;
    unsigned char *linkedFile = nullptr;
    int bytesread = 0;
    do {
        bytesread = read(fd, (void *) header, 512);
        if (bytesread == -1) {
            cerr << "Lesefehler" << endl;
            exit(1);
        } else if (bytesread > 0) {
            filename = header;
            filemode = &header[100];
            owner = &header[108];
            group = &header[116];
            size = &header[124];
            lastmodification = &header[136];
            linkedFileType = &header[156];
            linkedFile = &header[157];

#define REGTYPE  '0'            /* regular file */
#define AREGTYPE '\0'           /* regular file */
#define LNKTYPE  '1'            /* link */
#define SYMTYPE  '2'            /* reserved */
#define CHRTYPE  '3'            /* character special */
#define BLKTYPE  '4'            /* block special */
#define DIRTYPE  '5'            /* directory */
#define FIFOTYPE '6'            /* FIFO special */
#define CONTTYPE '7'            /* reserved */

           // http://www.gnu.org/software/tar/manual/html_node/Standard.html
           // ALle Infos findest du unter dem Link
            cout << filemode << " " << owner << " " << group << " " << size << " " << lastmodification << " "
                 << filename << endl;
        }

    } while (bytesread != 0);
    close(fd);
/*
    char *directory = argv[argc -
                           1]; // Zielpfad getopt() tauscht die Reihenfolge von argv, Zielpfad liegt jetzt an letzter Stelle
    DIR *dir = opendir(directory);
    dirent *file = nullptr;
    while ((file = readdir(dir)) != nullptr) {
        char *path = file->d_name;
        stringstream ss;
        ss << directory << "/" << path;
        if (*path != '.' || a) {
            struct stat buf;
            if (lstat((char *) ss.str().c_str(), &buf) != 0) // lsat schreibt Datei Informationen in das struct buf
                cout << "Errno: " << path << " " << strerror(errno) << endl;
            mode_t m = buf.st_mode;
            char fileType = '-';
            if (l || o || g) {
                if (S_ISDIR(m) != 0) fileType = 'd';
                if (S_ISCHR(m) != 0) fileType = 'c';
                if (S_ISBLK(m) != 0) fileType = 'b';
                if (S_ISFIFO(m) != 0) fileType = 'p';
                if (S_ISSOCK(m) != 0) fileType = 's';
                cout << fileType;
                if (S_IRUSR & m) cout << "r"; // Binärer Und Vergleich
                else cout << "-";
                if (S_IWUSR & m) cout << "w";
                else cout << "-";
                if (S_IXUSR & m) cout << "x";
                else cout << "-";
                if (S_IRGRP & m) cout << "r";
                else cout << "-";
                if (S_IWGRP & m) cout << "w";
                else cout << "-";
                if (S_IXGRP & m) cout << "x";
                else cout << "-";
                if (S_IROTH & m) cout << "r";
                else cout << "-";
                if (S_IWOTH & m) cout << "w";
                else cout << "-";
                if (S_IXOTH & m) cout << "x";
                else cout << "-";

                cout << " " << buf.st_nlink << " ";
                if ((o && !g) || !o && !g) {
                    struct passwd *p = nullptr;
                    p = getpwuid(buf.st_uid);
                    cout << p->pw_name << " ";
                }
                if ((g && !o) || !o && !g) {
                    struct group *grp = nullptr;
                    grp = getgrgid(buf.st_gid);
                    cout << grp->gr_name << " ";
                }
                cout << buf.st_size << " ";
                struct tm *tmptr = nullptr;
                tmptr = localtime(&buf.st_mtim.tv_sec);
                char ptr[80];
                strftime(ptr, 80, "%b  %d  %X", tmptr);
                cout << ptr << " ";
            }
            cout << path;
            if (l || g || o) cout << endl;
            else cout << " ";
        }

    }


    int closeDir = closedir(dir);
    */
}
