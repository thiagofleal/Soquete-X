#ifndef _DLL_H_
#define _DLL_H_

#if BUILDING_DLL
#define DLLIMPORT __declspec(dllexport)
#else
#define DLLIMPORT __declspec(dllimport)
#endif

DLLIMPORT Result IniciarSoquete(Result*, int);
DLLIMPORT Result EncerrarSoquete(Result*, int);
DLLIMPORT Result NovoSoquete(Result*, int);
DLLIMPORT Result DestruirSoquete(Result*, int);
DLLIMPORT Result Aceitar(Result*, int);
DLLIMPORT Result Iniciar(Result*, int);
DLLIMPORT Result Conectar(Result*, int);
DLLIMPORT Result Vincular(Result*, int);
DLLIMPORT Result Ouvir(Result*, int);
DLLIMPORT Result Enviar(Result*, int);
DLLIMPORT Result Receber(Result*, int);
DLLIMPORT Result Fechar(Result*, int);

#endif
