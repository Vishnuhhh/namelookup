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
    printf("In function getNameInfo called\n");
    char host[NI_MAXHOST];
    char serv[NI_MAXSERV];
    char ip_addr[INET6_ADDRSTRLEN];

#if 0
    struct sockaddr_in addr;
    memset(&addr, \0, sizeof(addr));
#endif

    struct addrinfo *host_addr;
    socklen_t host_addr_len;
    struct addrinfo hints; 
    memset(&hints, 0x00, sizeof(hints)); //Updating other fields in hints to 0
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags |= AI_NUMERICHOST | AI_CANONNAME;
    
    int err_code;
    err_code = getaddrinfo(address, NULL, &hints, &host_addr);
    if(err_code != 0)
    {
        fprintf(stderr, "getaddrinfo failed: [%s]\n", gai_strerror(err_code));
        exit(EXIT_FAILURE);
    }

#if 0
    if((err_code = inet_pton(AF_INET, address, &addr.sin_addr)) != 1)
    {
        fprintf(stderr, "inet_pton failed: %s\n", strerror(err_code));
        exit(EXIT_FAILURE);
    }
    if(inet_ntop(AF_INET, &addr.sin_addr, ip_addr, INET6_ADDRSTRLEN) == NULL)
    {
        perror("inet_ntop failed:");
        exit(EXIT_FAILURE);
    }
    fprintf(stdout, "Input address: [%s]\n", ip_addr);
    addr.sin_family = AF_INET;
#endif

    (host_addr->ai_family == AF_INET6)? host_addr_len = sizeof(struct sockaddr_in6) \
                           : (host_addr->ai_family == AF_INET)? host_addr_len = sizeof(struct sockaddr_in): 0;
    if(host_addr->ai_family == AF_INET6) 
    {
        if(inet_ntop(host_addr->ai_family, &(((struct sockaddr_in6 *)host_addr->ai_addr)->sin6_addr), ip_addr, INET6_ADDRSTRLEN) == NULL)
        {
            perror("inet_ntop failed:");
            exit(EXIT_FAILURE);
        }
        printf("Got address [%s]\n", ip_addr);
    }
    else if(host_addr->ai_family == AF_INET) 
    {
        if(inet_ntop(host_addr->ai_family, &(((struct sockaddr_in *)host_addr->ai_addr)->sin_addr), ip_addr, INET_ADDRSTRLEN) == NULL)
        {
            perror("inet_ntop failed:");
            exit(EXIT_FAILURE);
        }
        printf("Got address [%s]\n", ip_addr);
    }
    int ret = getnameinfo((struct sockaddr *)host_addr->ai_addr, host_addr_len, host, NI_MAXHOST, NULL, 0, NI_NAMEREQD);
    if(ret != 0)
    {
        fprintf(stderr, "getnameinfo failed: [%s]\n", gai_strerror(ret));
        exit(EXIT_FAILURE);
    }

    printf("Host: [%s]\n", host);

    return;
}


int main(int argc, char **argv) {
    int c;
    int index;
    char host[NI_MAXHOST];
    char address[INET6_ADDRSTRLEN];
    while((c = getopt(argc, argv, ":i:n:")) != -1) {
        switch(c)
        {
            case 'i' : 
                //strncpy(host, optarg, NI_MAXHOST);
                memcpy(host, optarg, NI_MAXHOST);
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
