#include <winsock.h>
#include <stdio.h>
#include <stdlib.h>
#define BUFFERSIZE 600
#define PROTOPORT 5000 // Numero di porta di default

int main(void) {
	int port = PROTOPORT;
	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2 ,2), &wsaData);
	if (iResult != 0) {
		printf ("error at WSASturtup\n");
			return -1;
		}

	// CREAZIONE DELLA SOCKET
	int Csocket;
	Csocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (Csocket < 0) {
		printf("socket creation failed.\n");
		closesocket(Csocket);
		WSACleanup();
		system("pause");
		return -1;
	}

	// COSTRUZIONE DELL’INDIRIZZO DEL SERVER
	struct sockaddr_in sad;
	memset(&sad, 0, sizeof(sad));
	sad.sin_family = AF_INET;
	sad.sin_addr.s_addr = inet_addr("127.0.0.1"); // IP del server
	sad.sin_port = htons( port ); // Server port

	// CONNESSIONE AL SERVER
	if (connect(Csocket, (struct sockaddr *)&sad, sizeof(sad)) < 0) {
		printf( "Failed to connect.\n" );
		closesocket(Csocket);
		WSACleanup();
		system("pause");
		return -1;
	}

	int bytesRcvd;
	int totalBytesRcvd = 0;
	char buf[BUFFERSIZE]; // buffer per data dal server

	//RICEZIONE CONFERMA DI AVVENUTA CONNESSIONE
	while (totalBytesRcvd < BUFFERSIZE - 1) {
		if ((bytesRcvd = recv(Csocket, buf, BUFFERSIZE, 0)) <= 0) {
			printf("recv() failed or connection closed prematurely");
			closesocket(Csocket);
			WSACleanup();
			system("pause");
			return -1;
		}
		totalBytesRcvd += bytesRcvd; // tiene conto dei bytes totali
		buf[bytesRcvd] = '\0'; // Aggiunge \0 così la printf sa dove fermarsi
		printf("%s", buf); // stampa l'echo buffer
	}

	//STAMPA MENU
	printf("\nOperazioni possibili:\n-Addizione: a/A. \n-Sottrazione: s/S. \n-Moltiplicazione: m/M. \n-Divisione: d/D.\n\n");

	//RICHIESTA OPERAZIONE E OPERANDI
	char oper[1], op1[BUFFERSIZE/3], op2[BUFFERSIZE/3];
	printf("Operazione scelta: ");
	gets(oper);
	printf("Primo operando: ");
	gets(op1);
	printf("Secondo operando: ");
	gets(op2);
	if(!strcmp(oper,"D")||!strcmp(oper,"d")){
		while(!strcmp(op2,"0")){
			printf("Non e' possibile dividere per 0 \nReinserisci il denominatore: ");
			gets(op2);
		}
	}

	//COSTRUZIONE STRINGA DA INVIARE AL SERVER
	strcat(oper, "*");
	strcat(oper, op1);
	strcat(oper, "*");
	strcat(oper, op2);

	//INVIO AL SERVER
	if (send(Csocket, oper, BUFFERSIZE, 0) != BUFFERSIZE) {
			printf("send() sent a different number of bytes than expected");
			closesocket(Csocket);
			WSACleanup();
			system("pause");
		return -1;
	}

	//RICEZIONE RISULTATI DAL SERVER
	totalBytesRcvd = 0;
	while (totalBytesRcvd < BUFFERSIZE - 1) {
		if ((bytesRcvd = recv(Csocket, buf, BUFFERSIZE - 1, 0)) <= 0) {
					printf("recv() failed or connection closed prematurely");
					closesocket(Csocket);
					WSACleanup();
					system("pause");
					return -1;
				}
				totalBytesRcvd += bytesRcvd; // tiene conto dei byte totali
				buf[bytesRcvd] = '\0'; // Aggiunge \0 così la printf sa dove fermarsi
			}
	if (!(strcmp(buf, "TERMINE PROCESSO CLIENT"))){
		printf("Operazione non supportata.\nChiusura connessione.");}
	else{
		printf("%s", buf); }


	// CHIUSURA DELLA CONNESSIONE
	closesocket(Csocket);
	WSACleanup();
	printf("\n");
	system("pause");
	return(0);
}





