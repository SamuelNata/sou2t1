#include <cstdio>		//printf()
#include <cstdlib>		//exit()
#include <cstring>		//memset - seta todos os valores de sin_zero para 0.
#include <netinet/in.h>	//htons - converte o numero da porta para o formato certo (big-endian ou little-endian)
#include <arpa/inet.h>	//inet_addr - converte a string com o ip no numero (para ipv6 não funciona, deve usar inet_pton: "presentation to network"; inet_ntop faz o inverso)
#include <sys/socket.h>	//soket
#include <unistd.h>		//close()
//#include <stdint.h>

#define PORTNUM 4325

typedef struct{
 int32_t time;
 char led_color;
 int32_t pot_adc;
 float humidity;
 float temperature;
} Things_t;

int main(){
	// variaveis do servidor
	struct sockaddr_in endereco;
	int socketId;

	//variaveis sobre a conexao com clientes
	struct sockaddr_in cliente;
	socklen_t tamEnderecoClient = sizeof(struct sockaddr );
	int conexaoClienteId;

	//dado que será enviado
	Things_t d;
	d.led_color='-';
	d.pot_adc=0;
	d.humidity=0;
	d.temperature=0;
	d.time=2;
	int byteslidos;

	//configurando endereço
	memset(&endereco, 0, sizeof(endereco));
	endereco.sin_family = AF_INET;
	endereco.sin_port = htons(PORTNUM);
	endereco.sin_addr.s_addr = inet_addr("192.168.7.1");

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
	return 0;
}


