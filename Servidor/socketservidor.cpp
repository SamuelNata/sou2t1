#include "socketservidor.h"
#include <thread>

SocketServidor::SocketServidor()
{
    this->d.time = 0;
    //this->configEndereco();
    this->newSocket();
    this->saveDados();
   // std::thread threadA(&SocketServidor::saveDados, this);
    //threadA.join();
}

void SocketServidor::saveDados()
{

        // o servidor deve ficar em loop infinito
        while(1){
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
        }

        close(conexaoClienteId);
    }

}


void SocketServidor::newSocket()
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

