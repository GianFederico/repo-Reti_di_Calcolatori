#include <winsock.h>
#include <stdio.h>
#include <stdlib.h>
#define BUFFERSIZE 512
#define PROTOPORT 5000 // Numero di porta di default

void ClearWinSock() {
	WSACleanup();
}

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
		ClearWinSock();
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
		ClearWinSock();
		system("pause");
		return -1;
	}

	//char* inputString = "AAAAA"; // Stringa da inviare
	int stringLen = BUFFERSIZE; // Determina la lunghezza
	int bytesRcvd;
	int totalBytesRcvd = 0;
	char buf[BUFFERSIZE]; // buffer for data from the server
	char buf2[BUFFERSIZE]; // buffer for data from the server
	printf("Received: "); // Setup to print the echoed string

	while (totalBytesRcvd < stringLen - 1) {
		if ((bytesRcvd = recv(Csocket, buf, BUFFERSIZE - 1, 0)) <= 0) {
			printf("recv() failed or connection closed prematurely");
			closesocket(Csocket);
			ClearWinSock();
			system("pause");
			return -1;
		}
		totalBytesRcvd += bytesRcvd; // Keep tally of total bytes
		buf[bytesRcvd] = '\0'; // Add \0 so printf knows where to stop
		printf("%s", buf); // Print the echo buffer
	}


	char strin[50];
	memset( strin, '\0', sizeof( strin ));
	printf("Prima stringa: ");
	scanf("%s", strin);


	if (send(Csocket, strin, 50, 0) != 50) {
			printf("send() sent a different number of bytes than expected");
			closesocket(Csocket);
			ClearWinSock();
			system("pause");
		return -1;
	}

	memset( strin, '\0', sizeof( strin ));
	printf("Seconda stringa: ");
	scanf("%s", strin);

	if (send(Csocket, strin, 50, 0) != 50) {
			printf("send() sent a different number of bytes than expected");
			closesocket(Csocket);
			ClearWinSock();
			system("pause");
		return -1;
	}


	totalBytesRcvd = 0;
	printf("\nPrima stringa in MAIUSCOLO: ");
	while (totalBytesRcvd < BUFFERSIZE - 1) {
		if ((bytesRcvd = recv(Csocket, buf, BUFFERSIZE - 1, 0)) <= 0) {
					printf("recv() failed or connection closed prematurely");
					closesocket(Csocket);
					ClearWinSock();
					system("pause");
					return -1;
				}
				totalBytesRcvd += bytesRcvd; // Keep tally of total bytes
				buf[bytesRcvd] = '\0'; // Add \0 so printf knows where to stop
				printf("%s", buf); // Print the echo buffer
			}


	totalBytesRcvd = 0;
	printf("\nSeconda stringa in MINUSCOLO: ");
	while (totalBytesRcvd < BUFFERSIZE - 1) {
			if ((bytesRcvd = recv(Csocket, buf2, BUFFERSIZE - 1, 0)) <= 0) {
						printf("recv() failed or connection closed prematurely");
						closesocket(Csocket);
						ClearWinSock();
						system("pause");
						return -1;
					}
					totalBytesRcvd += bytesRcvd; // Keep tally of total bytes
					buf2[bytesRcvd] = '\0'; // Add \0 so printf knows where to stop
					printf("%s", buf2); // Print the echo buffer
				}


	// CHIUSURA DELLA CONNESSIONE
	closesocket(Csocket);
	ClearWinSock();
	printf("\n"); // Print a final linefeed
	system("pause");
	return(0);
}
