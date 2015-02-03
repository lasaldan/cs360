#include "messageFiler.h"
#include <iostream>
#include <string>
#include <map>
using namespace std;

MessageFiler::MessageFiler() {
	
}

MessageFiler::~MessageFiler() {
	//nothing to delete
}

void
MessageFiler::storeMessage( string recipient, Message msg ) {
	messages.insert(pair<string,Message>(recipient, msg));
}

string
MessageFiler::listMsgs( string recipient ) {
    string out, buf;
    out += "list ";
    buf = "";
    
    int numMessages = 0;
    
	for ( it=messages.begin(); it != messages.end(); it++ )
    	if ( (*it).first == recipient ) {
    		numMessages ++;
    		buf += numMessages;
    		buf += " ";
    		buf += (*it).second.getSubject();
    		out += "\n";
    	}
    	
    out += numMessages;
    out += "\n";
    out += buf;
    
    cout << out << endl;
    return out;
}

string
MessageFiler::getMsg( string recipient, int index ) {

}

bool 
MessageFiler::test( std::ostream & os ) {
	string recipient1("Daniel");
	string recipient2("Dan");
	
	string sub1("grade");
	string sub2("school");
	string sub3("car");
	
	string note1("Your grade is pretty Good");
	string note2("You are doing well in school");
	string note3("You should wash your car");
	
	Message msg1(sub1, note1);
	Message msg2(sub2, note2);
	Message msg3(sub3, note3);
	
	MessageFiler filer;
	filer.storeMessage(recipient1, msg1);
	filer.storeMessage(recipient1, msg2);
	filer.storeMessage(recipient2, msg3);
	
	cout << filer.listMsgs(recipient1) << endl;
	filer.listMsgs(recipient2);
	
	return true;
}
