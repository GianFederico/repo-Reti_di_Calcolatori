#if defined WIN32
	#include <winsock.h>
#else
	#define closesocket close
	#include <sys/socket.h>
	#include <arpa/inet.h>
	#include <unistd.h>
#endif
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#define CHARMAX 255 // dimensione massima delle strighe in entrata e uscita

void ClearWinSock() {
	#if defined WIN32
		WSACleanup();
	#endif
}

int main() {
	#if defined WIN32
		WSADATA wsaData;
		int iResult = WSAStartup(MAKEWORD(2 ,2), &wsaData);
		if (iResult != 0) {
			printf ("error at WSASturtup\n");
			return EXIT_FAILURE;
		}
	#endif

	int sock;
	struct sockaddr_in ServAddr;
	int fromSize;
	char buffer[CHARMAX];
	char okin[2];
	int respStringLen;
	char name[50];
	int port=0;
	struct hostent *host;
	struct in_addr *ina;
	char vocali[CHARMAX];
	int v=0;
	char nvoc[CHARMAX];
	char vocin[1];

	printf("Nome server: ");
	scanf("%s", name);
	printf("Numero di porta (default = 5000): ");
	scanf("%d", &port);
	host = gethostbyname(name);
	if	(host == NULL)	{
	fprintf(stderr,	"gethostbyname() failed.\n");
		exit(EXIT_FAILURE);
		system("pause");
	} else {
		ina	= (struct in_addr*)	host->h_addr_list[0];
		printf("IP ottenuto tramite il nome dell'host(%s): %s\n", name, inet_ntoa(*ina));
	}

	// CREAZIONE DELLA SOCKET
	if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
		printf("socket() failed");

	// COSTRUZIONE DELL'INDIRIZZO DEL SERVER
	memset(&ServAddr, 0, sizeof(ServAddr));
	ServAddr.sin_family = PF_INET;
	ServAddr.sin_port = htons(port);
	ServAddr.sin_addr.s_addr = inet_addr(inet_ntoa(*ina));

	// RICHIESTA STRINGA VIA INPUT DA INVIARE AL SERVER
	printf("Inserisci la stringa di benvenuto da inviare al server\n");
	scanf("%s", buffer);
	if ((strlen(buffer)) > CHARMAX)
		printf("Stringa troppo lunga");

	// INVIO DELLA STRINGA AL SERVER
	if (sendto(sock, buffer, strlen(buffer), 0, (struct sockaddr*)&ServAddr, sizeof(ServAddr)) != strlen(buffer))
		printf("sendto() sent different number of bytes than expected");

	// RICEZIONE STRINGA "OK" DAL SERVER
	sleep(1);
	fromSize = sizeof(ServAddr);
	respStringLen = recvfrom(sock, okin, 2, 0, (struct sockaddr*)&ServAddr, &fromSize);
	okin[respStringLen] = '\0';
	printf("Ricevuto dal server: %s\n", okin);

	// RICHISTA STRINGA DA INPUT
	memset(buffer, '\0', sizeof(buffer));
	printf("Inserisci la stringa\n");
	fflush(stdin);
	gets(buffer);


	// CONTEGGIO E SALVATAGGIO VOCALI
	for (int i=0;i<strlen(buffer); i++){
		if ((buffer[i] == 'a')||(buffer[i] == 'e')||(buffer[i] == 'i')||(buffer[i] == 'o')||(buffer[i] == 'u')||(buffer[i] == 'A')||(buffer[i] == 'E')||(buffer[i] == 'I')||(buffer[i] == 'O')||(buffer[i] == 'U')){
			vocali[v]=buffer[i];
			v++;
		}
	}
	vocali[v]='\0';
	sprintf(nvoc, "%d", v);

	// INVIO NUMERO DI VOCALI
	sendto(sock, nvoc, strlen(nvoc), 0, (struct sockaddr*)&ServAddr, sizeof(ServAddr));

	printf("%s Vocali da inviare: %s\n", nvoc, vocali);

	// INVIO DI OGNI VOCALE E RICEZIONE DELLE STESSE IN MAIUSCOLO
	printf("Vocali della stringa inviata in MAIUSCOLO: ");
	for (int i=0; i<v; i++){
		sendto(sock, &vocali[i], 1, 0, (struct sockaddr*)&ServAddr, sizeof(ServAddr));
		recvfrom(sock, vocin, 1, 0, (struct sockaddr*)&ServAddr, &fromSize);
		if (i>0){
			printf("\n\t\t\t\t\t   ");
		}
		printf("%c", vocin[0]);
	}
	printf("\n");

	// CHIUSURA DELLA SOCKET
	closesocket(sock);
	ClearWinSock();
	system("pause");
	return EXIT_SUCCESS;
}

