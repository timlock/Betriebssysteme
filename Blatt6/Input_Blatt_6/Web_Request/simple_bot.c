/**************************************************************
* 
* Einfacher Bot zum Lesen verschiedener URLs, die in einer Datei (wird
* als Parameter uebergeben) angegeben sind. Verwendet die
* Datei web_request.c.
*
* Oliver Kaestner, Heinz-Josef Eikerling, HS OS
* 
* 10.11.2019: Neue Version verwendet libcurl statt Socket-Implementierung.
* 14.12.2020: Cast des Ergebnisses von malloc(). (HJE)
* 
* To Do:
* - ...
**************************************************************/

#include <malloc.h>
#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <sys/time.h>

#include "web_request.h"

#define USE_PROXY 0

typedef struct URLList {
    char* url;
    struct URLList* next;
} URLList;

/* 
 * Main
 */
int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr,
                "Usage: %s <inputfile> [options]\n\n"
                "Options:\n"
                "\t--webreq-delay <uint>\n"
                "\t--webreq-delay-seed <int>\n"
                "\t--webreq-path <path>\n",
                argv[0]);
        return EXIT_FAILURE;
    }

    // open file containing the pages to GET
    FILE* fd_in;
    if (!(fd_in = fopen(argv[1], "r"))) {
        perror("failed to open input file");
        exit(EXIT_FAILURE);
    }

    // generate time stamp
    struct timeval  tv;

    gettimeofday(&tv, NULL);
    double start = (tv.tv_sec) * 1000 + (tv.tv_usec) / 1000;

    //-------------------------------------------------------------

    // create list of URLs to query
    URLList* urllist = NULL;
    URLList* curr = NULL;

    char url[256];

    while (fgets(url, sizeof (url), fd_in)) {
        // trim newline chars and skip empty lines
        // (strtok: no token found -> NULL -> empty string)
        if (strtok(url, "\r\n") == NULL)
            continue;

        // append the url to the list
        if (urllist == NULL) {
            // start of the list
            urllist = (URLList*) malloc(sizeof (URLList));
            curr = urllist;
        } else {
            // new node in list
            curr->next = (URLList*) malloc(sizeof (URLList));
            curr = curr->next;
        }

        curr->url = strdup(url);
        curr->next = NULL;
    }

    // close file
    fclose(fd_in);

    // init lib
    webreq_init(argc, argv);

    // download all
    curr = urllist;
    int i = 0;
    while (curr != NULL) {
        // get domain for filename
        char* url = strdup(curr->url);
        strtok(url, "/");
        char* domain = strtok(NULL, "/");

        if (domain == NULL) {
            fprintf(stderr, "[ERROR] domain could not be extracted from URL [%s], skipping\n", curr->url);
        } else {
            char filename[64];
            snprintf(filename, sizeof (filename), "%i_%s.html", i++, domain);

            printf("[START] Downloading URL: %s ->> File: %s\n", curr->url, filename);

            int res;
#if USE_PROXY
            res = webreq_download_via_proxy(curr->url, filename);
#else
            res = webreq_download(curr->url, filename);
#endif
            if (res < 0)
                fprintf(stderr, "[ERROR] URL: %s, Message: %s\n", curr->url, webreq_error(res));
            else if (res != WEBREQ_HTTP_OK)
                fprintf(stderr, "[ERROR] HTTP Status %d returned for URL: %s\n", res, curr->url);
            else
                printf("[DONE ] URL: %s ->> File: %s\n", curr->url, filename);
        }

        free(url);
        curr = curr->next;
    }

    // free mem
    webreq_cleanup();

    curr = urllist;
    URLList* next;

    // free list from first to last element
    while (curr != NULL) {
        // save ptr to next node, so that this node can be freed
        next = curr->next;
        free(curr->url);
        free(curr);
        curr = next;
    }

    //-------------------------------------------------------------
    // measure runtime
    gettimeofday(&tv, NULL);
    double end = (tv.tv_sec) * 1000 + (tv.tv_usec) / 1000;
    double diff = (end - start) / 1000;

    printf("Duration: %5.2f s\n", diff);

    return EXIT_SUCCESS;
}
