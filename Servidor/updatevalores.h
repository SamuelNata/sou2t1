#ifndef UPDATEVALORES_H
#define UPDATEVALORES_H

#include <QObject>
#include<stdint.h>

#include <cstdio>		//printf()
#include <cstdlib>		//exit()
#include <cstring>		//memset - seta todos os valores de sin_zero para 0.
#include <netinet/in.h>	//htons - converte o numero da porta para o formato certo (big-endian ou little-endian)
#include <arpa/inet.h>	//inet_addr - converte a string com o ip no numero (para ipv6 não funciona, deve usar inet_pton: "presentation to network"; inet_ntop faz o inverso)
#include <sys/socket.h>	//soket
#include <unistd.h>		//close()
#include<stdint.h>


#define PORTNUM 4325

typedef struct{
 int32_t time;
 char led_color;
 int32_t pot_adc;
 float humidity;
 float temperature;
} Things_t;

class UpdateValores : public QObject
{
     Q_OBJECT

public:
    UpdateValores();
    void saveDados();
    float getHumidity();
    float getTemperature();
    int32_t getPotenciometro();
    char getColor();
    int32_t getTime();
public slots:
    void doWork();

signals:
    void updateCount(char,float,float,int);
    void finished();
private:
    bool m_running;
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

#endif // UPDATEVALORES_H
