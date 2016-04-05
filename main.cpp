#include <cstdio>		//printf()
#include <cstdlib>		//exit()
#include <cstring>		//memset - seta todos os valores de sin_zero para 0.
#include <netinet/in.h>	//htons - converte o numero da porta para o formato certo (big-endian ou little-endian)
#include <arpa/inet.h>	//inet_addr - converte a string com o ip no numero (para ipv6 não funciona, deve usar inet_pton: "presentation to network"; inet_ntop faz o inverso)
#include <sys/socket.h>	//soket
#include <unistd.h>		//close()
#include "Adafruit_Python_DHT/source/Beaglebone_Black/bbb_dht_read.h"
#include <thread>
#include "BlackLib/v3_0/BlackGPIO/BlackGPIO.h"

#include<iostream>
#include<fstream>
#include<string>
#include<sstream>

#include <QThread>

#define PATH_ADC "/sys/bus/iio/devices/iio:device0/in_voltage"
#define PORTNUM 4325


#include <csignal>

int tipo = DHT22;

//GPIO_P8_11 = GPIO1_13 = base 1 e número 13
int base = 1;
int numero = 13;
sockaddr_in endereco;
int socketId;
int bytesenviados;

using namespace std;

int readAnalog(int number){
   stringstream ss;
   ss << PATH_ADC << number << "_raw";
   fstream fs;
   fs.open(ss.str().c_str(), fstream::in);
   fs >> number;
   fs.close();
   return number;
}

typedef struct{
 int32_t time;
 char led_color;
 int32_t pot_adc;
 float humidity;
 float temperature;
} Things_t;

char setColor(int i, BlackLib::BlackGPIO * ledR, BlackLib::BlackGPIO * ledG, BlackLib::BlackGPIO * ledB ){
    printf("Setando cor do led.\n");
    switch( i ){
        case 1:
            ledR->setValue(BlackLib::high);
            ledG->setValue(BlackLib::low);
            ledB->setValue(BlackLib::low);
            return 'R';
            break;
        case 2:
            ledR->setValue(BlackLib::low);
            ledG->setValue(BlackLib::high);
            ledB->setValue(BlackLib::low);
            return 'G';
            break;
        default:
            ledR->setValue(BlackLib::low);
            ledG->setValue(BlackLib::low);
            ledB->setValue(BlackLib::high);
            return 'B';
            break;
    }
    return '-';
}

void pisca( Things_t & d, BlackLib::BlackGPIO & ledR, BlackLib::BlackGPIO & ledG, BlackLib::BlackGPIO & ledB ){
    while(1){
        switch( d.led_color ){
            case 'R':
                ledR.setValue(BlackLib::low);
                ledG.setValue(BlackLib::low);
                ledB.setValue(BlackLib::low);
                QThread::msleep(20);
                ledR.setValue(BlackLib::high);
                break;
            case 'G':
                ledR.setValue(BlackLib::low);
                ledG.setValue(BlackLib::low);
                ledB.setValue(BlackLib::low);
                QThread::msleep(20);
                ledG.setValue(BlackLib::high);
                break;
            default:
                ledR.setValue(BlackLib::low);
                ledG.setValue(BlackLib::low);
                ledB.setValue(BlackLib::low);
                QThread::msleep(20);
                ledB.setValue(BlackLib::high);
                break;
        }
        sleep(4);
    }
}

void dataUpdate( Things_t & d, BlackLib::BlackGPIO & ledR, BlackLib::BlackGPIO & ledG, BlackLib::BlackGPIO & ledB ){
    while( 1 ){
        d.time = 3;
        d.led_color = setColor((1+rand())%3, &ledR, &ledG, &ledB);
        d.pot_adc = readAnalog(1);
        if (bbb_dht_read(tipo, base, numero, &d.humidity, &d.temperature) != 0)
        {
            printf("Erro na leitura do sensor\n");
        }
        else
        {
            printf("Cliente: enviando dados... ");
            bytesenviados = send(socketId, &d, sizeof(Things_t), 0);
            printf("dados enviados.\n");
            if (bytesenviados == -1)
            {
                printf("Falha ao executar send()");
                exit(EXIT_FAILURE);
            }
        }
        sleep(d.time);
    }
}

void exit(int sig)
{

    BlackLib::BlackGPIO   verde(BlackLib::GPIO_51,BlackLib::output, BlackLib::SecureMode);
    BlackLib::BlackGPIO   amarelo(BlackLib::GPIO_50,BlackLib::output, BlackLib::SecureMode);
    BlackLib::BlackGPIO   vermelho(BlackLib::GPIO_60,BlackLib::output, BlackLib::SecureMode);
    vermelho.setValue(BlackLib::low);
    amarelo.setValue(BlackLib::low);
    verde.setValue(BlackLib::low);

    kill(getpid(), SIGQUIT);
}

int main(int argc, char *argv[])
{
    signal(SIGINT, exit );

    //system("sudo sh -c \"echo 'BB-ADC' > /sys/devices/platform/bone_capemgr/slots\"");

    Things_t d;

    memset( &endereco, 0, sizeof(endereco) );
    endereco.sin_family = AF_INET;
    endereco.sin_port = htons(PORTNUM);
    endereco.sin_addr.s_addr = inet_addr("192.168.7.1");

    socketId = socket(AF_INET, SOCK_STREAM, 0);

    if (socketId == -1)
    {
        printf("Falha ao executar socket()\n");
        exit(EXIT_FAILURE);
    }

    if( connect( socketId, (struct sockaddr *)&endereco, sizeof(struct sockaddr))==-1 ){
        printf("Falha ao executar connect()\n");
        exit(EXIT_FAILURE);
    }

    printf ("Cliente: conectado ao servidor\n");
    srand(time(NULL));

    BlackLib::BlackGPIO   ledR(BlackLib::GPIO_60,BlackLib::output, BlackLib::SecureMode);
    BlackLib::BlackGPIO   ledG(BlackLib::GPIO_50,BlackLib::output, BlackLib::SecureMode);
    BlackLib::BlackGPIO   ledB(BlackLib::GPIO_51,BlackLib::output, BlackLib::SecureMode);

    thread piscar(pisca, std::ref(d), std::ref(ledR), std::ref(ledG), std::ref(ledB) );
    thread threadUpdate(dataUpdate, std::ref(d), std::ref(ledR), std::ref(ledG), std::ref(ledB) );
    threadUpdate.join();
    piscar.join();
    close(socketId);

    return 0;
}
