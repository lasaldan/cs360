#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <sys/socket.h>

#include <iostream>
#include <string>

using namespace std;

int
main(int argc, char** argv)
{
    string host;

      // get URLs passed on the command line
    if (argc < 2) {
        cout << "dns <host>" << endl;
        exit(-1);
    }
    host = string(argv[1]);
    
      // use DNS to get host name
    struct hostent *hostEntry;
    hostEntry = gethostbyname(host.c_str());
    if (!hostEntry) {
        cout << "No such host name: " << host << endl;
        exit(-1);
    }

    cout << hostEntry->h_name << endl;
    int i = 0;
    while (hostEntry->h_aliases[i] != NULL) {
        cout << "\t" << hostEntry->h_aliases[i] << endl;
        i++;
    }
    if (hostEntry->h_addrtype == AF_INET)
        cout << "AF_INET" << endl;
    else if (hostEntry->h_addrtype == AF_INET6)
        cout << "AF_INET6" << endl;
    else
        cout << "unknown address type" << endl;
    
    i = 0;
    while (hostEntry->h_addr_list[i] != NULL) {
        cout << "\t" << inet_ntoa(*(struct in_addr *)hostEntry->h_addr_list[i]) << endl;
        i++;
    }

}

