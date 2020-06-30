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
#include <stdlib.h>
#include <string.h>
#define CHARMAX 255 // Dimensione massima delle stringhe in entrata e uscita
#define PORT 5000

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
	ServAddr.sin_family = AF_INET;
	ServAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	ServAddr.sin_port = htons(PORT);
	struct sockaddr_in ClientAddr;
	struct	hostent	*host;
	int cliAddrLen;
	char buf[CHARMAX];
	int recvMsgSize;
	const char*	ip;
	struct in_addr	addr;
	char* canonical_name;
	char nvoc[1];
	char voc[1];
	//char vocali[CHARMAX];

	// CREAZIONE DELLA SOCKET
	if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
		printf("socket() failed");

	// BIND DELLA SOCKET
	if ((bind(sock, (struct sockaddr *)&ServAddr, sizeof(ServAddr))) < 0)
		printf("bind() failed");

	while(1) {
		cliAddrLen = sizeof(ClientAddr);

		// RICEZIONE DELLA STRINGA DAL CLIENT
		recvMsgSize = recvfrom(sock, buf, CHARMAX, 0, (struct sockaddr*)&ClientAddr, &cliAddrLen);
		ip = inet_ntoa(ClientAddr.sin_addr);
		addr.s_addr	= inet_addr(ip);
		host = gethostbyaddr((char	*)	&addr,	4,	AF_INET);
		canonical_name	=	host->h_name;
		buf[recvMsgSize]='\0';
		printf("Messaggio ricevuto dal client '%s': %s\n", canonical_name, buf);//inet_ntoa(ClientAddr.sin_addr));

		// INVIO DELLA STRINGA "OK" AL CLIENT
		if (sendto(sock, "OK", 2, 0, (struct sockaddr *)&ClientAddr, sizeof(ClientAddr)) != 2)
			printf("sendto() sent different number of bytes than expected");

		//RICEZIONE NUMERO VOCALI
		recvfrom(sock, nvoc, CHARMAX, 0, (struct sockaddr*)&ClientAddr, &cliAddrLen);
		int v=atoi(nvoc);
		printf("%d vocali rilevate nella stringa ricevuta: ",v);

		//CICLO PER RICEZIONI VOCALI, VOCALI MAIUSCOLE E INVIO VOCALI
		for (int i=0;i<v;i++){
			recvfrom(sock, voc, 1, 0, (struct sockaddr*)&ClientAddr, &cliAddrLen);
			printf("%c",voc[0]);
			strupr(voc);
			if (i<v-1)
				printf(", ");
			sendto(sock, voc, 1, 0, (struct sockaddr *)&ClientAddr, sizeof(ClientAddr));
		}
		printf("\n");
	}
}



