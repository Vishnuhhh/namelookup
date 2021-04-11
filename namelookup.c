#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <error.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <string.h>
#include <arpa/inet.h>

#define HOST_NAME_MAX 256

void usage(void) {
    printf("usage\t: namelookup [options] argument\n");
    printf("-i \t: prints IP address from given name\n");
    printf("-n \t: prints name from given IP address\n");
    return; 
}

void getAddrInfo(char *name) {
    printf("In function getAddrInfo called with name [%s]\n", name);
    char address[INET6_ADDRSTRLEN];
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    struct addrinfo *res;
    struct addrinfo *rp;
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags |= AI_CANONNAME;

    int ret = getaddrinfo(name, NULL, &hints, &res);
    if(ret != 0) 
    {
        fprintf(stderr, "getaddrinfo failed: [%s]\n", gai_strerror(ret));
        exit(EXIT_FAILURE);
    }
    rp = res;
    printf("Domain Name:\t[%s]\n", rp->ai_canonname);
    for(; rp != NULL; rp = rp->ai_next) {
        //printf("rp->ai_family %d\n", rp->ai_family);
        memset(address, 0, INET6_ADDRSTRLEN);
        if(rp->ai_family == AF_INET)
        {
            if(!inet_ntop(rp->ai_family, &((struct sockaddr_in *)rp->ai_addr)->sin_addr, address, INET_ADDRSTRLEN)) {
                fprintf(stderr, "inet_ntop failed\n");
            }
            else 
                printf("Address:\t[%s]\n", address);
        }
        else if(rp->ai_family == AF_INET6)
        {
            if(!inet_ntop(rp->ai_family, &((struct sockaddr_in6 *)rp->ai_addr)->sin6_addr, address, INET6_ADDRSTRLEN)) {
                fprintf(stderr, "inet_ntop failed\n");
            }
            else
                printf("Address:\t[%s]\n", address);
        }
    }
    freeaddrinfo(res);
    return;
}

void getNameInfo(char *address) {
    printf("In function getNameInfo called with name [%s]\n", address);

    return;
}

int main(int argc, char **argv) {
    int c;
    int index;
    char host[HOST_NAME_MAX];
    char address[INET6_ADDRSTRLEN];
    while((c = getopt(argc, argv, ":i:n:")) != -1) {
        switch(c)
        {
            case 'i' : 
                //strncpy(host, optarg, HOST_NAME_MAX);
                memcpy(host, optarg, HOST_NAME_MAX);
                getAddrInfo(optarg);
                break;
            case 'n' : 
                //strncpy(address, optarg, INET6_ADDRSTRLEN);
                memcpy(address, optarg, INET6_ADDRSTRLEN);
                getNameInfo(address);
                break;
            case ':' :
                fprintf(stderr, "Option -%c requires an arument\n", optopt);
                usage();
                return -1;

            case '?' :
                if(isprint(optopt))
                    fprintf(stderr, "Unkown option '%c'. \n", optopt);
                else
                    fprintf(stderr, "Unkown option character '\\x%x'.\n", optopt);
                usage();
                return -1;
            default :
                usage();
                return -1;
        }
    }
    for(index = optind; index < argc; index++) {
        printf("Non-option argument\n");
    }
    return 0;
}
