#ifndef MESSAGEFILER_H
#define MESSAGEFILER_H

#include <iostream>
#include <string>
#include <map>

class Message {
	public:
		Message();
		~Message();
		Message(std::string, std::string);
		std::string getMessage();
		std::string getSubject();
		std::string socketOutput();
	private:
		std::string sub;
		std::string msg;
};

class MessageFiler {
	public:
		MessageFiler();
		~MessageFiler();
		void storeMessage( std::string, Message );
		std::string listMsgs( std::string );
		std::string getMsg( std::string, int );
		static bool test( std::ostream & );
	private:
		std::map<std::string,Message> messages;
		std::map<std::string,Message>::iterator it;
};
#endif
