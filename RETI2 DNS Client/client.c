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
	char name[50];
	printf("Nome server:  ");
	scanf("%s", name);
	struct	hostent	*host;
	struct	in_addr*	ina;
	host	=	gethostbyname(name);
	if	(host	==	NULL)	{
		fprintf(stderr,	"gethostbyname() failed.\n");
		exit(EXIT_FAILURE);
		system("pause");
	}	else	{
		ina	=	(struct	in_addr*)	host->h_addr_list[0];
		printf("Risultato di gethostbyname(%s):	%s\n",	name, inet_ntoa(*ina));
	}

	/*const	char*	ip	=	"127.0.0.1";
	struct	in_addr	addr;
	addr.s_addr	=	inet_addr(ip);
	host	=	gethostbyaddr((char	*)	&addr,	4,	AF_INET);
	char*	canonical_name	=	host->h_name;
	printf("Risultato	di	gethostbyaddr(%s):	%s\n",
	ip,	canonical_name);
	//return	EXIT_SUCCESS;*/

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
	sad.sin_addr.s_addr = inet_addr(inet_ntoa(*ina)); // IP del server
	sad.sin_port = htons( port ); // Server port

	// CONNESSIONE AL SERVER
	if (connect(Csocket, (struct sockaddr *)&sad, sizeof(sad)) < 0) {
		printf( "Failed to connect.\n" );
		closesocket(Csocket);
		ClearWinSock();
		system("pause");
		return -1;
	}

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





