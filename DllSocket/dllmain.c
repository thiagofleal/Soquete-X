#include <string.h>
#include <Tonight\tonight.h>
#include <Tonight\socket.h>
#include "Socket\socket.h"
#include "header.h"
#include "dll.h"

const static Result voidRet = {
	.type = type_void,
	.value.rt_pointer = NULL
};

const static Result boolRet = {
	.type = type_bool,
	.value.rt_bool = true
};

DLLIMPORT Result IniciarSoquete(Result *_arg, int numArg)
{
	try
	{
		startSocket();
		return boolRet;
	}
	catch(SocketException)
	{
		Result ret = boolRet;
		ret.value.rt_bool = false;
		return ret;
	}
}

DLLIMPORT Result EncerrarSoquete(Result *_arg, int numArg)
{
	endSocket();
	return boolRet;
}

DLLIMPORT Result NovoSoquete(Result *_arg, int numArg)
{
	Result ret;
	
	ret.type = type_object;
	
	if(numArg < 3)
	{
		ret.value.rt_pointer = new Object(Socket, new_void_sock());
	}
	else
	{
		int type = (int)_arg[0].value.rt_double;
		string ip = _arg[1].value.rt_String;
		unsigned long port = (unsigned long)_arg[2].value.rt_double;
		
		ret.value.rt_pointer = new Object(Socket, new_sock(type, ip, SocketResources.inet, port));
	}
	
	return ret;
}

DLLIMPORT Result DestruirSoquete(Result *_arg, int numArg)
{
	object obj = _arg[0].value.rt_pointer;
	
	delete(obj);
	return voidRet;
}

DLLIMPORT Result Aceitar(Result *_arg, int numArg)
{
	object sock = _arg[0].value.rt_pointer;
	object other = _arg[1].value.rt_pointer;
	
	try
	{
		iSocket.accept(sock, other);
		return boolRet;
	}
	catch(SocketException)
	{
		Result ret = boolRet;
		ret.value.rt_bool = false;
		return ret;
	}
}

DLLIMPORT Result Iniciar(Result *_arg, int numArg)
{
	object sock = _arg[0].value.rt_pointer;
	
	try
	{
		iSocket.start(sock);
		return boolRet;
	}
	catch(SocketException)
	{
		Result ret = boolRet;
		ret.value.rt_bool = false;
		return ret;
	}
}

DLLIMPORT Result Conectar(Result *_arg, int numArg)
{
	object sock = _arg[0].value.rt_pointer;
	
	try
	{
		iSocket.connect(sock);
		return boolRet;
	}
	catch(SocketException)
	{
		Result ret = boolRet;
		ret.value.rt_bool = false;
		return ret;
	}
}

DLLIMPORT Result Vincular(Result *_arg, int numArg)
{
	object sock = _arg[0].value.rt_pointer;
	
	try
	{
		iSocket.bind(sock);
		return boolRet;
	}
	catch(SocketException)
	{
		Result ret = boolRet;
		ret.value.rt_bool = false;
		return ret;
	}
}

DLLIMPORT Result Ouvir(Result *_arg, int numArg)
{
	object sock = _arg[0].value.rt_pointer;
	int backlog = (int)_arg[1].value.rt_double;
	
	try
	{
		iSocket.listen(sock, backlog);
		return boolRet;
	}
	catch(SocketException)
	{
		Result ret = boolRet;
		ret.value.rt_bool = false;
		return ret;
	}
}

DLLIMPORT Result Enviar(Result *_arg, int numArg)
{
	object sock = _arg[0].value.rt_pointer;
	string message = _arg[1].value.rt_String;
	Package pack;
	
	pack.length = strlen(message) + 1;
	pack.value = message;
	
	try
	{
		iSocket.send(sock, pack);
		return boolRet;
	}
	catch(SocketException)
	{
		Result ret = boolRet;
		ret.value.rt_bool = false;
		return ret;
	}
}

DLLIMPORT Result Receber(Result *_arg, int numArg)
{
	Result ret;
	Package pack;
	object sock = _arg[0].value.rt_pointer;
	
	ret.type = type_string;
	
	try
	{
		pack = iSocket.receive(sock);
		ret.value.rt_String = pack.value;
	}
	catch(SocketException)
	{
		ret.value.rt_String = toString("");
	}
	
	return ret;
}

DLLIMPORT Result Fechar(Result *_arg, int numArg)
{
	object sock = _arg[0].value.rt_pointer;
	
	try
	{
		iSocket.close(sock);
		return boolRet;
	}
	catch(SocketException)
	{
		Result ret = boolRet;
		ret.value.rt_bool = false;
		return ret;
	}
}
