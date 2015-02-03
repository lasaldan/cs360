#include "messageFiler.h"
#include <iostream>
#include <string>
#include <map>
using namespace std;


Message::Message() {
	sub = "";
	msg = "";
}

Message::~Message() {
	//nothing to delete
}

Message::Message(string _sub, string _msg) {
	sub = _sub;
	msg = _msg;
}

string
Message::getSubject() {
	return sub;
}
string
Message::getMessage() {
	return msg;
}

string
Message::socketOutput() {
	string out;
	out = "Subject: ";
	out += sub;
	out += " Message: ";
	out += msg;
	return out;
}
