#include <string.h>
#include <winsock.h>
#include <Tonight\tonight.h>
#include <Tonight\socket.h>
#include "socket.h"

enum{
	VALUE_LENGTH = 1024
};

struct socket_private{
	struct sockaddr_in addr;
	SOCKET fd;
	int type;
};

static WSADATA *data = NULL;

void startSocket(void)
{
	if(!data)
	{
		data = new Memory(sizeof(WSADATA));
	}
	
	if(WSAStartup(MAKEWORD(2, 2), data))
	{
		throw(StartSocketException, "Error with WSAStartup()");
	}
}

void endSocket(void)
{
	if(data)
	{
		free(data);
		data = NULL;
	}
	
	WSACleanup();	
}

SocketPrivate new_sock(int type, string ip, short family, u_long port)
{
	SocketPrivate sock = new_void_sock();
	struct sockaddr_in addr;
	
	memset(&addr, 0, sizeof addr);
	addr.sin_addr.s_addr = inet_addr(ip);
	addr.sin_family = family;
	addr.sin_port = htons(port);
	sock->type = type;
	sock->addr = addr;
	
	return sock;
}

SocketPrivate new_void_sock(void)
{
	return new Memory(sizeof(struct socket_private));
}

SocketPrivate new_sock_addr(struct sockaddr_in addr, int type)
{
	SocketPrivate sock = new_void_sock();
	
	sock->addr = addr;
	sock->type = type;
	
	return sock;
}

void socket_start(INTERN_METHOD)
{
	CLASS(Socket);
	
	cast_super(ASocket).sock->fd = socket(
		cast_super(ASocket).sock->addr.sin_family,
		cast_super(ASocket).sock->type, 0
	);
	
	if(cast_super(ASocket).sock->fd == INVALID_SOCKET)
	{
		throw(StartSocketException, "Error with socket()");
	}
}

void socket_accept(INTERN_METHOD, object socket)
{
	CLASS(Socket);
	
	int fd_sock, lenght;
	SocketPrivate sock = iSocket.getData(socket);
	
	lenght = sizeof sock->addr;
	fd_sock = accept(cast_super(ASocket).sock->fd, (struct sockaddr*)&sock->addr, &lenght);
	
	if(fd_sock < 0)
	{
		throw(AcceptSocketException, "Error with accept()");
	}
	
	close(cast_super(ASocket).sock->fd);
	cast_super(ASocket).sock->fd = fd_sock;
}

void socket_connect(INTERN_METHOD)
{
	CLASS(Socket);
	
	if(connect(
		cast_super(ASocket).sock->fd,
		(struct sockaddr*)&cast_super(ASocket).sock->addr,
		sizeof cast_super(ASocket).sock->addr
	) < 0)
	{
		throw(ConnectSocketException, "Error with connect()");
	}
}

void socket_bind(INTERN_METHOD)
{
	CLASS(Socket);
	
	if(bind(
		cast_super(ASocket).sock->fd,
		(struct sockaddr*)&cast_super(ASocket).sock->addr,
		sizeof cast_super(ASocket).sock->addr
	) < 0)
	{
		throw(BindSocketException, "Error with bind()");
	}
}

void socket_listen(INTERN_METHOD, int backlog)
{
	CLASS(Socket);
	
	if(listen(
		cast_super(ASocket).sock->fd,
		backlog
	))
	{
		throw(ListenSocketException, "Error with listen()");
	}
}

void socket_send(INTERN_METHOD, Package message)
{
	CLASS(Socket);
	
	if(send(
		cast_super(ASocket).sock->fd,
		message.value,
		message.length, 0
	) < 0)
	{
		throw(SendSocketException, "Error with send()");
	}
}

Package socket_receive(INTERN_METHOD)
{
	CLASS(Socket);
	char value[VALUE_LENGTH];
	Package pack;
	
	if(recv(
		cast_super(ASocket).sock->fd,
		value,
		VALUE_LENGTH, 0
	) < 0)
	{
		throw(ReceiveSocketException, "Error with recv()");
	}
	
	pack.length = strlen(value);
	pack.value = toString(value);
	
	return pack;
}

SocketPrivate socket_getData(INTERN_METHOD)
{
	CLASS(Socket);
	
	return cast_super(ASocket).sock;
}

void socket_close(INTERN_METHOD)
{
	CLASS(Socket);
	
	if(closesocket(cast_super(ASocket).sock->fd) < 0)
	{
		throw(CloseSocketException, "Error with close()");
	}
}

static ISocket Socket_vtble = {
	.start = socket_start,
	.accept = socket_accept,
	.connect = socket_connect,
	.bind = socket_bind,
	.listen = socket_listen,
	.send = socket_send,
	.receive = socket_receive,
	.getData = socket_getData,
	.close = socket_close
};

Constructor(Socket)
{
	CLASS(Socket);
	
	super_init(ASocket);
	super_setInterface(ASocket, ISocket, Socket_vtble);
}

Destructor(Socket)
{
	CLASS(Socket);
	
	if(cast_super(ASocket).sock)
	{
		free(cast_super(ASocket).sock);
	}
	super_delete(ASocket);
}

Define_Class(Socket);
