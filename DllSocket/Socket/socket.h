struct Socket{
	extend(ASocket);
};

class(Socket);

void startSocket(void);
void endSocket(void);
SocketPrivate new_sock(int, string, short, unsigned long);
SocketPrivate new_void_sock(void);
