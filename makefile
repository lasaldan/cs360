# Makefile for CS 360 example code

CXX=	g++ -g
DNS_OBJS=	dns.o
MSG_OBJS=	msg.o
MSGD_OBJS=	msgd.o messageFiler.o message.o
OBJS =	$(DNS_OBJS) $(MSG_OBJS) $(MSGD_OBJS)

LIBS=

CCFLAGS= -g

all:	dns msg msgd

msg:$(MSG_OBJS)
	$(CXX) -o msg $(MSG_OBJS) $(LIBS)

msgd:$(MSGD_OBJS)
	$(CXX) -o msgd $(MSGD_OBJS) $(LIBS)

dns:	$(DNS_OBJS)
	$(CXX) -o dns $(DNS_OBJS) $(LIBS)

clean:
	rm -f $(OBJS) $(OBJS:.o=.d)
	rm -f msg
	rm -f msgd
	rm -f dns

# These lines ensure that dependencies are handled automatically.
%.d:	%.cc
	$(SHELL) -ec '$(CC) -M $(CPPFLAGS) $< \
		| sed '\''s/\($*\)\.o[ :]*/\1.o $@ : /g'\'' > $@; \
		[ -s $@ ] || rm -f $@'

include	$(OBJS:.o=.d)
