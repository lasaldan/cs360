#include <arpa/inet.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#include <fstream>
#include <iostream>
#include <sstream>

#define debug(msg) if(debug) cout << msg << endl;
using namespace std;


string sendAndReceive( int s, string command );


int
main(int argc, char **argv)
{
    struct sockaddr_in server;
    const char *ipaddress;
    int option, s, port;
    bool debug;
    string line, msg;
    char *buf = (char *) calloc(1024,sizeof(char));

    // setup default arguments
    port = 3000;
    ipaddress = "127.0.0.1";
    debug = false;

    // process command line options using getopt()
    // see "man 3 getopt"
    while ((option = getopt(argc,argv,"s:p:d")) != -1) {
        switch (option) {
            case 'p':
                port = atoi(optarg);
                break;
            case 's':
                ipaddress = optarg;
                break;
            case 'd':
            	debug = true;
            	break;
            default:
                cout << "client [-s IP address] [-p port] [-d debug]" << endl;
                exit(EXIT_FAILURE);
        }
    }
    
    debug("Debug Mode Active");
    debug("Destination: " << ipaddress << ":" << port);

      // setup socket address structure
    memset(&server,0,sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    if (!inet_aton(ipaddress,&server.sin_addr)) {
        printf("inet_addr() conversion error\n");
        exit(-1);
    }

      // create socket
    s = socket(PF_INET,SOCK_STREAM,0);
    if (s < 0) {
        perror("socket");
        exit(-1);
    }

      // connect to server
    if (connect(s,(const struct sockaddr *)&server,sizeof(server)) < 0) {
        perror("connect");
        exit(-1);
    }

	string command;
	
	command = "put daniel grade 25\nYour Grade is pretty good";
    send(s, command.c_str(), command.length(), 0);
    
    command = "put daniel great 25\nYour good is pretty grade";
    send(s, command.c_str(), command.length(), 0);
    
    command = "list daniel\n";
    send(s, command.c_str(), command.length(), 0);
    
    command = "read daniel 1\n";
    send(s, command.c_str(), command.length(), 0);
	
	return 1;
/**/
	cout << "% ";
      // read a line from standard input
    while (getline(cin,line)) {
    
    	char * input = new char[line.length()+1];
  		char * token;
  		
		strcpy(input,line.c_str());
 		token = strtok (input," ");
 		
 		// check if the user wants to send a message
 		if(strcmp(token,"send") == 0 ) { 			
 			char * name;
 			char * subject;
 			
 			// make sure we have a name and subject
 			name = strtok(NULL," ");
 			if(name == NULL)
 				name = (char *)"NoName";
 				
 			subject = strtok(NULL," ");
 			if(subject == NULL)
 				subject = (char *)"NoSubject";
 			
 			// It appears the users wants to send a messge, let's get ready.
 			string message = "";
 			cout << "- Type your message. End with a blank line -" << endl;
 			
 			// get message from cin
 			getline(cin,msg);
 			message = msg;
 			// and allow user to add many lines
 			while(msg != "") {
 				getline(cin,msg);
 				if(msg != "")
 					message += "\n" + msg;
 			}
 			
 			// format command for sending to server
 			stringstream c;
			c << "put " << name << " " << subject << " " << message.length() << "\n" << message;
			string command = c.str();
			
 			//debug("Sending message to server");
		    string response = "";
		    
		    response = sendAndReceive(s, command);
		   	cout << response << endl;
		    
		    memset(buf,0,1024);
		    recv(s,buf,1024,0);
		    
		    // print the response
		    cout << buf << endl;
 		}
 		if(strcmp(token,"read") == 0 ) {
 			debug("Recieved Read Command");
 		}
 		if(strcmp(token,"list") == 0 ) {
 			debug("Recieved List Command");
 		}
 		if(strcmp(token,"quit") == 0 ) {
 			debug("Recieved Quit Command");
 			return 0;
 		}
 		
		// Get next token
		//token = strtok (NULL, " ");

        // read the response
        //memset(buf,0,1024);
        //recv(s,buf,1024,0);
        
        // print the response
        //cout << buf << endl;
        cout << "% ";
    }

      // Close socket
    close(s);
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
