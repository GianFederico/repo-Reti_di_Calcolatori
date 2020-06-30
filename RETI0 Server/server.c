#include <winsock.h>
#include <stdio.h>
#include <stdlib.h>
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
 char buf[BUFFERSIZE]; // buffer for data from the client
 printf("Waiting for a client to connect...");
 while (1) { /* oppure for (;;) */

   clientLen = sizeof(cad); // set the size of the client address
   if ((clientSocket = accept(MySocket, (struct sockaddr *)&cad, &clientLen)) < 0) {
    printf("accept() failed.\n");
    // CHIUSURA DELLA CONNESSIONE
    closesocket(MySocket);
    ClearWinSock();
    return 0;
   }
  printf("Handling client %s\n", inet_ntoa(cad.sin_addr));

  printf("Received: "); // Setup to print the echoed string
  int totalBytesRcvd = 0;
  int bytesRcvd = 0;
  while (totalBytesRcvd < 5){
   if ((bytesRcvd = recv(clientSocket, buf, BUFFERSIZE - 1, 0)) <= 0) {
    printf("recv() failed or connection closed prematurely");
    closesocket(clientSocket);
    ClearWinSock();
    system("pause");
    return -1;
   }
   totalBytesRcvd += bytesRcvd; // Keep tally of total bytes
   buf[bytesRcvd] = '\0'; // Add \0 so printf knows where to stop
   printf("%s\n", buf); // Print the echo buffer
   }

  if (send(clientSocket, buf, BUFFERSIZE - 1, 0) != BUFFERSIZE - 1) {
   printf("send() sent a different number of bytes than expected");
   closesocket(clientSocket);
   ClearWinSock();
   system("pause");
   return -1;
  }
  closesocket(clientSocket);

 }
}
