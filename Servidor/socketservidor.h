#ifndef SOCKETSERVIDOR_H
#define SOCKETSERVIDOR_H


#include <cstdio>		//printf()
#include <cstdlib>		//exit()
#include <cstring>		//memset - seta todos os valores de sin_zero para 0.
#include <netinet/in.h>	//htons - converte o numero da porta para o formato certo (big-endian ou little-endian)
#include <arpa/inet.h>	//inet_addr - converte a string com o ip no numero (para ipv6 não funciona, deve usar inet_pton: "presentation to network"; inet_ntop faz o inverso)
#include <sys/socket.h>	//soket
#include <unistd.h>		//close()
#include<stdint.h>

typedef struct{
 int32_t time;
 char led_color;
 int32_t pot_adc;
 float humidity;
 float temperature;
} Things_t;


#define PORTNUM 4325




class SocketServidor
{
public:
    SocketServidor();
    void saveDados();

private:
    // variaveis do servidor
    struct sockaddr_in endereco;
    int socketId;

    //variaveis sobre a conexao com clientes
    struct sockaddr_in cliente;
    socklen_t tamEnderecoClient = sizeof(struct sockaddr );
    int conexaoClienteId;

    Things_t d;
    int byteslidos;

    void configEndereco();
    void newSocket();
};

#endif // SOCKETSERVIDOR_H
