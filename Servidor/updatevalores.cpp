#include "updatevalores.h"
#include "portablesleep.h"

#include <QApplication>
#include <iostream>
#include <thread>

UpdateValores::UpdateValores()
{

}


/**
 * @brief   UpdateCpu::doWork
 *          Metodo responsavel por pegar os dados e chamar o metodo
 *          para modificar os valroes da interface
 * @param   void
 * @return  void
 */
void UpdateValores::doWork()
{
    //SocketServidor a;
    //std::thread threadA(&SocketServidor::saveDados, this);
    //threadA.join();


    this->d.time = 0;
    this->configEndereco();
    this->newSocket();

    while(true)
    {
        //a.saveDados();
        //std::cout << "DEBUGEEEE" << a.getColor() << std::endl;










        printf("SERVER: servidor esperando conexao:\n");

        //servidor fica bloqueado até esrecer uma conexao
        conexaoClienteId = accept(socketId, (struct sockaddr*) &cliente, &tamEnderecoClient );

        printf("SERVER: Servidor recebeu conexao de %s\n", inet_ntoa(cliente.sin_addr));

        //verificando erro
        if( conexaoClienteId==-1 ){
            printf("erro ao executar accept()\n");
            exit(EXIT_FAILURE);
        }

        //receber dados
        printf("SERVER: esperando dados.\n");
        while( 1 ){
            byteslidos = recv(conexaoClienteId, &d, sizeof(Things_t), 0);

            if( byteslidos==-1 ){
                printf("falha no recv().\n");
                exit(EXIT_FAILURE);
            }
            else if( byteslidos==0 ){
                printf("SERVER: o cliente fechou a conexao.\n");
                exit(EXIT_SUCCESS);
            }

            printf("Recebido: led(%c), pot(%i), humi(%f\%), temp(%fC°).\n", d.led_color, d.pot_adc, d.humidity, d.temperature);

            emit updateCount(d.led_color, d.temperature, d.humidity, d.pot_adc);
            qApp->processEvents();
            PortableSleep::msleep(d.time);


        }

        close(conexaoClienteId);










    }
     emit finished();
}


float UpdateValores::getHumidity()
{
    return d.humidity;
}

float UpdateValores::getTemperature()
{
    return d.temperature;
}

int32_t UpdateValores::getPotenciometro()
{
    return d.pot_adc;
}

char UpdateValores::getColor()
{
    return d.led_color;
}

int32_t UpdateValores::getTime()
{
    return d.time;
}

void UpdateValores::configEndereco()
{
    //configurando endereço
    memset(&endereco, 0, sizeof(endereco));
    endereco.sin_family = AF_INET;
    endereco.sin_port = htons(PORTNUM);
    endereco.sin_addr.s_addr = inet_addr("192.168.7.1");
}

void UpdateValores::newSocket()
{
    //criando socket
    socketId = socket(AF_INET, SOCK_STREAM, 0);

    //Verificando se deu erro
    if( socketId==-1 ){
        printf("falha ao executar socket()\n");
        exit(EXIT_FAILURE);
    }

    //Conectando socket a uma porta (only server)
    if( bind(socketId, (struct sockaddr *) &endereco, sizeof(struct sockaddr))==-1 ){
        printf("Falha ao executar binD()\n");
        exit(EXIT_FAILURE);
    }

    //Fazendo o servidor receber conexões
    if( listen(socketId, 10)==-1 ){
        printf("Erro ao executar listen()\n");
        exit(EXIT_FAILURE);
    }

}
