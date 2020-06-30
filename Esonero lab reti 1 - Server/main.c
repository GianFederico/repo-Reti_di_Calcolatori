#include <winsock.h>
#include <stdio.h>
#include <stdlib.h>
#define PROTOPORT 27015 // default protocol port number
#define QLEN 6 // dimensione coda
#define BUFFERSIZE 600


int main(int argc, char *argv[]){
	int port;
	if (argc > 1) {
		port = atoi(argv[1]); // cnnversione a binario
	}
	else
		port = PROTOPORT; // porta di default
	if (port < 0) {
		printf("bad port number %s \n", argv[1]);
		return 0;
	}
	WSADATA wsaData; //initializzazione WinSock
	int iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
	if (iResult != 0) {
		printf("Error at WSAStartup()\n");
	return 0;
	}

	// CREAZIONE DELLA SOCKET
	int MySocket;
	MySocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (MySocket < 0) {
		printf("socket creation failed.\n");
		WSACleanup();
	return -1;
	}

	// ASSEGNAZIONE DI UN INDIRIZZO ALLA SOCKET
	struct sockaddr_in sad;
	memset(&sad, 0, sizeof(sad)); // sia ssicura che gli extra bytes contengano 0
	sad.sin_family = AF_INET;
	sad.sin_addr.s_addr = inet_addr( "127.0.0.1" );
	sad.sin_port = htons( 5000 );
	if (bind(MySocket, (struct sockaddr*) &sad, sizeof(sad)) <0) {
		printf("bind() failed.\n");
		closesocket(MySocket);
		WSACleanup();
		system("pause");
		return -1;
	}

	//SETTAGGIO DELLA SOCKET ALL'ASCOLTO
	if (listen (MySocket, QLEN) < 0) {
		printf("listen() failed.\n");
		closesocket(MySocket);
		WSACleanup();
		return -1;
	}


	struct sockaddr_in cad; // struttura per l'indirizzo del client
	int clientSocket; // descrittore della socket per il client
	int clientLen; // dimensione dell'indirizzo client
	char buf[BUFFERSIZE]; // buffer per data dal client
	// ACCETTARE UNA NUOVA CONNESSIONE
	printf("Attesa della connessione di un client...");
	while (1) {
		char conn[BUFFERSIZE]="Connessione stabilita\n";
		clientLen = sizeof(cad); // imposta la dimensione dell'indrizzo client
		if ((clientSocket = accept(MySocket, (struct sockaddr *)&cad, &clientLen)) < 0) {
			printf("accept() failed.\n");
			// CHIUSURA DELLA CONNESSIONE
			closesocket(MySocket);
			WSACleanup();
			return 0;
		}
		system("cls");
		printf("Client connesso.\nIP client: %s\n", inet_ntoa(cad.sin_addr));

	//INVIO CONFERMA DI AVVENUTA CONNESSIONE
		if (send(clientSocket, conn, BUFFERSIZE, 0) != BUFFERSIZE) {
			printf("send() sent a different number of bytes than expected");
			closesocket(clientSocket);
			WSACleanup();
			system("pause");
			return -1;
		}

	//RICEZIONE DATI DA PROCESSARE DAL CLIENT
		int totalBytesRcvd = 0;
		int bytesRcvd = 0;
		while (totalBytesRcvd < BUFFERSIZE - 1){
			if ((bytesRcvd = recv(clientSocket, buf, BUFFERSIZE, 0)) <= 0) {
				printf("recv() failed or connection closed prematurely");
				closesocket(clientSocket);
				WSACleanup();
				system("pause");
				return -1;
			}
			totalBytesRcvd += bytesRcvd;
			buf[bytesRcvd] = '\0'; // Aggiunge \0 così la printf si ferma
			}

	//SMISTAMENTO OPERAZIONE E OPERANDI
		char oper[1], op1[BUFFERSIZE/3], op2[BUFFERSIZE/3];
		memset( oper, '\0', sizeof( oper ));
		memset( op1, '\0', sizeof( op1 ));
		memset( op2, '\0', sizeof( op2 ));
		int i=0, j=0, k=0;
		while (i<=2){
			k=0;
			if (i==0){
				while (buf[j]!='*'){
					oper[j]=buf[j];
					j++;
				}
			}
			if (i==1) {
				j++;
				while (buf[j]!='*'){
					op1[k]=buf[j];
					j++;
					k++;
				}
			}
			if (i==2) {
				j++;
				while (buf[j]!='\0'){
					op2[k]=buf[j];
					j++;
					k++;
				}
			}
			i++;
		}

	//IDENTIFICAZIONE ED ESECUZIONE DELL'OPERAZIONE
		memset( buf, '\0', sizeof( buf ));
		char *pt;
		long a=strtol(op1, &pt, 10);
		long b=strtol(op2, &pt, 10);
		float div=0;
		char result[BUFFERSIZE*2/3];
		switch (oper[0]){
		case 'A': case 'a':
			strcat(buf, "Risultato dell'addizione: ");
			a=a+b;
			sprintf(result, "%ld", a);
			strcat(buf, result);
			break;
		case 'S': case 's':
			strcat(buf, "Risultato della sottrazione: ");
			a=a-b;
			sprintf(result, "%ld", a);
			strcat(buf, result);
			break;
		case 'M': case 'm':
			strcat(buf, "Risultato della moltiplicazione: ");
			a=a*b;
			sprintf(result, "%ld", a);
			strcat(buf, result);
			break;
		case 'D': case 'd':
			strcat(buf, "Risultato della divisione: ");
			div=(float)a/(float)b;
			gcvt(div, 6, result);
			strcat(buf, result);
			break;
		default:
			strcat(buf, "TERMINE PROCESSO CLIENT");
			break;
		}

	//INVIO RISULTATI AL CLIENT
		if (send(clientSocket, buf, BUFFERSIZE, 0) != BUFFERSIZE) {
					printf("send() sent a different number of bytes than expected");
					closesocket(clientSocket);
					WSACleanup();
					system("pause");
					return -1;
				}
		closesocket(clientSocket);
	}
}




