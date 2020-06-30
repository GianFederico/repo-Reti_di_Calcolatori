#include <winsock.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define PROTOPORT 27015 // default protocol port number
#define QLEN 6 // size of request queue
#define BUFFERSIZE 512



void ClearWinSock() {
	WSACleanup();
}

int main(int argc, char *argv[]){
	int port;
	if (argc > 1) {
		port = atoi(argv[1]); // if argument specified convert argument to binary
	}
	else
		port = PROTOPORT; // use default port number
	if (port < 0) {
		printf("bad port number %s \n", argv[1]);
		return 0;
	}
	WSADATA wsaData; //initialize WinSock
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
		ClearWinSock();
	return -1;
	}

	// ASSEGNAZIONE DI UN INDIRIZZO ALLA SOCKET
	struct sockaddr_in sad;
	memset(&sad, 0, sizeof(sad)); // ensures that extra bytes contain 0
	sad.sin_family = AF_INET;
	sad.sin_addr.s_addr = inet_addr( "127.0.0.1" );
	sad.sin_port = htons( 5000 );
	if (bind(MySocket, (struct sockaddr*) &sad, sizeof(sad)) <0) {
		printf("bind() failed.\n");
		closesocket(MySocket);
		ClearWinSock();
		system("pause");
		return -1;
	}

	//SETTAGGIO DELLA SOCKET ALL'ASCOLTO
	if (listen (MySocket, QLEN) < 0) {
		printf("listen() failed.\n");
		closesocket(MySocket);
		ClearWinSock();
		return -1;
	}

	// ACCETTARE UNA NUOVA CONNESSIONE
	struct sockaddr_in cad; // structure for the client address
	int clientSocket; // socket descriptor for the client
	int clientLen; // the size of the client address
	char conn[BUFFERSIZE]="Connessione avvenuta\n";
	char buf1[BUFFERSIZE]; // buffer for data from the client
	memset( buf1, '\0', sizeof( buf1 ));
	char buf2[BUFFERSIZE]; // buffer for data from the client
	memset( buf2, '\0', sizeof( buf2 ));
	printf("Waiting for a client to connect...");
	while (1) { /* oppure for (;;) */

			clientLen = sizeof(cad); // set the size of the client address
			if ((clientSocket = accept(MySocket, (struct sockaddr *)&cad, &clientLen)) < 0) {
				printf("accept() failed.\n");
				// CHIUSURA DELLA CONNESSIONE
				system("pause");
				closesocket(MySocket);
				ClearWinSock();
				return 0;
			}
			struct	hostent	*host;
			const	char*	ip	=	inet_ntoa(cad.sin_addr);
			struct	in_addr	addr;
			addr.s_addr	=	inet_addr(ip);
			host	=	gethostbyaddr((char	*)	&addr,	4,	AF_INET);
			char*	canonical_name	=	host->h_name;

		printf("Nome client: %s\n IP Client %s\n", canonical_name, inet_ntoa(cad.sin_addr));

		if (send(clientSocket, conn, BUFFERSIZE, 0) != BUFFERSIZE) {
			printf("send() sent a different number of bytes than expected");
			closesocket(clientSocket);
			ClearWinSock();
			system("pause");
			return -1;
		}

		printf("Received: "); // Setup to print the echoed string
		int totalBytesRcvd = 0;
		int bytesRcvd = 0;
		while (totalBytesRcvd < BUFFERSIZE - 1){// || buf1[bytesRcvd] == '\0'){
			if ((bytesRcvd = recv(clientSocket, buf1, 50, 0)) <= 0) {
				printf("recv() failed or connection closed prematurely");
				closesocket(clientSocket);
				ClearWinSock();
				system("pause");
				return -1;
			}
			totalBytesRcvd += bytesRcvd; // Keep tally of total bytes
			buf1[bytesRcvd] = '\0'; // Add \0 so printf knows where to stop
			printf("%s\n", buf1); // Print the echo buffer
			totalBytesRcvd = BUFFERSIZE;
			}


		printf("Received: "); // Setup to print the echoed string
		totalBytesRcvd = 0;
		bytesRcvd = 0;
		while (totalBytesRcvd < BUFFERSIZE - 1){
			if ((bytesRcvd = recv(clientSocket, buf2, 50, 0)) <= 0) {
				printf("recv() failed or connection closed prematurely");
				closesocket(clientSocket);
				ClearWinSock();
				system("pause");
				return -1;
			}
			totalBytesRcvd += bytesRcvd; // Keep tally of total bytes
			buf2[bytesRcvd] = '\0'; // Add \0 so printf knows where to stop
			printf("%s\n", buf2); // Print the echo buffer
			totalBytesRcvd = BUFFERSIZE;
			}



		strupr(buf1);
		if (send(clientSocket, buf1, BUFFERSIZE, 0) != BUFFERSIZE) {
					printf("send() sent a different number of bytes than expected");
					closesocket(clientSocket);
					ClearWinSock();
					system("pause");
					return -1;
				}

		sleep(1);

		strlwr(buf2);
		if (send(clientSocket, buf2, BUFFERSIZE, 0) != BUFFERSIZE) {
				printf("send() sent a different number of bytes than expected");
				closesocket(clientSocket);
				ClearWinSock();
				system("pause");
				return -1;
				}






		closesocket(clientSocket);

	}

}



