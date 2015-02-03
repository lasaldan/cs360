#include <arpa/inet.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include "messageFiler.h"
using namespace std;

int manageClient (int c);
enum { PUT, LIST, GET, AWAITING_MESSAGE, AWAITING_COMMAND };
string sendAndReceive( int s, string command );

int
main(int argc, char **argv)
{
    struct sockaddr_in server,client;
    socklen_t clientlen = sizeof(client);
    int option, port, s, c;
    char *buf = (char *) calloc(1024,sizeof(char));
    char *ptr;
    int opt;
    int nread,nwritten,nleft;
    MessageFiler filer;
    //filer.test(cout);

    // setup default arguments
    port = 3000;

    // process command line options using getopt()
    // see "man 3 getopt"
    while ((option = getopt(argc,argv,"p:")) != -1) {
        switch (option) {
            case 'p':
                port = atoi(optarg);
                break;
            default:
                cout << "server [-p port]" << endl;
                exit(EXIT_FAILURE);
        }
    }

      // setup socket address structure
    memset(&server,0,sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = INADDR_ANY;

      // create socket
    s = socket(PF_INET,SOCK_STREAM,0);
    if (!s) {
        perror("socket");
        exit(-1);
    }

      // set socket to immediately reuse port when the application closes
    opt = 1;
    if (setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("setsockopt");
        exit(-1);
    }

      // call bind to associate the socket with our local address and
      // port
    if (bind(s,(const struct sockaddr *)&server,sizeof(server)) < 0) {
        perror("bind");
        exit(-1);
    }

      // convert the socket to listen for incoming connections
    if (listen(s,SOMAXCONN) < 0) {
        perror("listen");
        exit(-1);
    }

      // accept clients
    while ((c = accept(s,(struct sockaddr *)&client,&clientlen)) > 0) {

 		manageClient(c);
        close(c);
        
    }
    
    close(s);
}

int readBuffer( int c, string & received ) {

    int nread;
    char *buf = (char *) calloc(1024,sizeof(char));
    memset(buf,0,1024);
    nread = recv(c,buf,1024,0);
    received += buf;
    
    if (nread < 0) {
        if (errno == EINTR)
            return nread;
        else
            return 0;
    } else if (nread == 0) {
        return 0;
    }
    
    return nread;
}

int manageClient(int c) {

	string received;
	received = "";
	size_t newline;
	int state, read;
	char *token, *name, *index, *length, *subject, *received_c;
	
	state = AWAITING_COMMAND;

	while (1) {

		// read a request
		if( ! (read = readBuffer(c,received)) ) break;
		
		while (read > 0) {
			cout << "Read: [" << received << "]" << endl;
			
			newline = received.find("\n");
	 		received_c = new char [received.size()+1];
			strcpy (received_c, received.c_str());
			token = strtok (received_c," \n");
		
			//If we're awaiting a command and we have a new line in buffer:
			if ( state == AWAITING_COMMAND && newline != string::npos ) {
	  			
	  			if(strcmp(token,"list") == 0 )
	  				state = LIST;
	  			else if(strcmp(token,"get") == 0 )
	  				state = GET;
	  			else if(strcmp(token,"put") == 0 )
	  				state = PUT;
		 	}
		 	
		 	
		 	if ( state == LIST ) {
		 		
				name = strtok(NULL," \n");
				name = strtok(NULL," \n");
		 		cout << "Listing messages for " << token << endl;
		 		while(1);
		 		
		 	}
		 	
		 	if ( state == GET ) {
		 		
				name = strtok(NULL," \n");
				index = strtok(NULL," \n");
		 		cout << "Getting message " << index << " for " << name << endl;
		 		while(1);
		 		
		 	}
		 	
		 	if ( state == PUT ) {
		 		
		 		int linebr;
				name = strtok(NULL," \n");
				subject = strtok(NULL," \n");
				length = strtok(NULL," \n");
				linebr = received.find("\n");
		 		received.erase (0,linebr+1);
		 		read -= linebr+1;
				state = AWAITING_MESSAGE;
				
			}
				
			if( state == AWAITING_MESSAGE && received.length() > atoi(length) ) {
		 		cout << "Putting message '" << subject << "' for " << name << " len(" << length << ")" << endl;
		 		cout << "Message: '" << received.substr(0,atoi(length)) << "'" << endl;
		 		received.erase (0,atoi(length));
		 		read -= atoi(length);
		 		state = AWAITING_COMMAND;
		 	}
		 }
    }
}

string sendAndReceive( int s, string command ) {
	
    char *buf = (char *) calloc(1024,sizeof(char));
    char * ptr;
    int nleft, nread, nwritten;
    nread = command.size();
	ptr = buf;
    nleft = nread;
    while (nleft) {
        if ((nwritten = send(s, ptr, nleft, 0)) < 0) {
            if (errno == EINTR) {
          // the socket call was interrupted -- try again
	continue;
            } else {
          // an error occurred, so break out
                perror("write");
                break;
            }
        } else if (nwritten == 0) {
              // the socket is closed
            break;
        }
        nleft -= nwritten;
        ptr += nwritten;
    }
    
    string response = "";
	while (response.find("\n") == string::npos) {
	    memset(buf,0,1024);
	    int nread = recv(s,buf,1024,0);
            if (nread < 0) {
                if (errno == EINTR)
 		      // the socket call was interrupted -- try again
                    continue;
                else
 		      // an error occurred, so break out
                    break;
            } else if (nread == 0) {
                  // the socket is closed
                break;
            }

	    response += buf;
	}
	
	return response;
}
