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

#define ASCII_TO_NUMBER(num) ((num)-48) //Converts an ascii digit to the corresponding number

static int decodeTarOctal(char *data, size_t size = 12) {
    unsigned char *currentPtr = (unsigned char *) data + size;
    int sum = 0;
    int currentMultiplier = 1;

    //Skip everything after the last NUL/space character
    //In some TAR archives the size field has non-trailing NULs/spaces, so this is neccessary
    unsigned char *checkPtr = currentPtr; //This is used to check where the last NUL/space char is
    for (; checkPtr >= (unsigned char *) data; checkPtr--) {
        if ((*checkPtr) == 0 || (*checkPtr) == ' ') {
            currentPtr = checkPtr - 1;
        }
    }

    for (; currentPtr >= (unsigned char *) data; currentPtr--) {
        sum += ASCII_TO_NUMBER(*currentPtr) * currentMultiplier;
        currentMultiplier *= 8;
    }
    return sum;
}

int main(int argc, char *argv[]) {
    unsigned char magic[6] = {'u', 's', 't', 'a', 'r'};
    unsigned char header[512];

    int fd = open(argv[1], O_RDONLY);

    int bytesread = read(fd, (void *) header, 512);

    if (bytesread == -1) {
        cerr << "Lesefehler" << endl;
        exit(1);
    }
    int j = 0;

    for (int i = 257; i <
                      262; i++) { // magic number liegt zwischen 257 und 263 http://www.gnu.org/software/tar/manual/html_node/Standard.html

        if (magic[j] != header[i]) {
            cerr << "Keine tar datei" << endl;
            exit(1);
        }
        j++;
    }
    unsigned char *filename = nullptr;
    unsigned char *typeflag = nullptr;
    unsigned char *filemode = nullptr;
    unsigned char *owner = nullptr;
    unsigned char *group = nullptr;
    unsigned char *size = nullptr;
    unsigned char *lastmodification = nullptr;

    // http://www.gnu.org/software/tar/manual/html_node/Standard.html
    /* Values used in typeflag field.  */
#define REGTYPE  '0'            /* regular file */
#define AREGTYPE '\0'           /* regular file */
#define LNKTYPE  '1'            /* link */
#define SYMTYPE  '2'            /* reserved */
#define CHRTYPE  '3'            /* character special */
#define BLKTYPE  '4'            /* block special */
#define DIRTYPE  '5'            /* directory */
#define FIFOTYPE '6'            /* FIFO special */
#define CONTTYPE '7'            /* reserved */

#define XHDTYPE  'x'            /* Extended header referring to the
                                   next file in the archive */
#define XGLTYPE  'g'            /* Global extended header */

/* Bits used in the mode field, values in octal.  */
#define TSUID    04000          /* set UID on execution */
#define TSGID    02000          /* set GID on execution */
#define TSVTX    01000          /* reserved */
    /* file permissions */
#define TUREAD   00400          /* read by owner */
#define TUWRITE  00200          /* write by owner */
#define TUEXEC   00100          /* execute/search by owner */
#define TGREAD   00040          /* read by group */
#define TGWRITE  00020          /* write by group */
#define TGEXEC   00010          /* execute/search by group */
#define TOREAD   00004          /* read by other */
#define TOWRITE  00002          /* write by other */
#define TOEXEC   00001          /* execute/search by other */

    do {
        if (bytesread == -1) {
            cerr << "Lesefehler" << endl;
            exit(1);
        } else if (bytesread > 0) {
            filename = header;
            filemode = &header[100];
            typeflag = &header[156];
            owner = &header[265];
            group = &header[297];
            size = &header[124];
            lastmodification = &header[136];

            int m = decodeTarOctal(reinterpret_cast<char *>(filemode));

            char fileType = '-';
            if (DIRTYPE == *typeflag) fileType = 'd';
            if (CHRTYPE == *typeflag) fileType = 'c';
            if (BLKTYPE == *typeflag) fileType = 'b';
            if (FIFOTYPE == *typeflag) fileType = 'p';
            if (LNKTYPE == *typeflag) fileType = 's';
            cout << fileType;
            if (TUREAD & m) cout << "r"; // Binärer Und Vergleich
            else cout << "-";
            if (TUWRITE & m) cout << "w";
            else cout << "-";
            if (TUEXEC & m) cout << "x";
            else cout << "-";
            if (TGREAD & m) cout << "r";
            else cout << "-";
            if (TGWRITE & m) cout << "w";
            else cout << "-";
            if (TGEXEC & m) cout << "x";
            else cout << "-";
            if (TOREAD & m) cout << "r";
            else cout << "-";
            if (TOWRITE & m) cout << "w";
            else cout << "-";
            if (TOEXEC & m) cout << "x";
            else cout << "-";

            int lm = decodeTarOctal(reinterpret_cast<char *>(lastmodification));
            int year = 1970 + lm/(60 * 60 * 24 * 365); // Richtig
            int month = lm/(60 * 60 * 24 * 12);
            int day = (lm/(60 * 60 * 24)) % 24;
            int hour = (lm/(60 * 60)) % 60;
            int minute = (lm/(60)) % 60; // Richtig

            stringstream time;

            time << year << "-" << month << "-" << day << " " << hour << ":" << minute;

            int s = decodeTarOctal(reinterpret_cast<char *>(size));

            // http://www.gnu.org/software/tar/manual/html_node/Standard.html
            // ALle Infos findest du unter dem Link
            cout << " " << owner << "/" << group << " " << s << " " << time.str() << " "
                 << filename << endl;
        }
        bytesread = read(fd, (void *) header, 512);
    } while (filename[0] != '\0');
    close(fd);
}
