/*  _._._._._._._._._._._._._._._._._._

    Corso Reti di Calcolatori - CORSO M-Z 2025/2026
    Esercizio n.2 - Client UDP di MARSICO Luciano Antonio
   
    _._._._._._._._._._._._._._._._._._
 */

#include <stdio.h>
#include <winsock2.h>
#include <string.h>

#pragma comment(lib,"ws2_32.lib")

int main(){
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2,2), &wsaData);

     // config. socket e server
    SOCKET sock = socket (AF_INET, SOCK_DGRAM,0);

    char serverName[100];
    printf("Inserisci nome del server (es. localhost): ");
    scanf("%s",serverName);

    struct hostent *host = gethostbyname(serverName); // funzione per convertire in IP il nome del server tramite il DNS
    char *serverIP = inet_ntoa(*(struct in_addr*)host->h_addr); 

    struct sockaddr_in serverAddr;
    int serverLength = sizeof(serverAddr);
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(serverIP);
    serverAddr.sin_port = htons(6543);

    char buffer[1024]; // array dati

    // 6, read del char richiesto
    char cmd[10]; 
    printf("Inserisci lettera (A/S/M/D o altro per terminare): "); 
    scanf("%s",cmd);
    sendto(sock,cmd,strlen(cmd),0,(struct sockaddr*)&serverAddr,serverLength);

    // 8, operazione
    int r = recvfrom(sock, buffer,sizeof(buffer)-1,0,(struct sockaddr*)&serverAddr,&serverLength);
    buffer[r]='\0';
    printf("Server: %s\n", buffer);

    // terminazione anticipata tramite cmd
    if(strcmp(buffer,"TERMINE PROCESSO CLIENT")==0) { 
    closesocket(sock); 
    WSACleanup(); 
    return 0; 
}

    // invio operandi
    int op1, op2;
    printf("Inserisci primo intero: "); 
    scanf("%d", &op1);
    printf("Inserisci secondo intero: "); 
    scanf("%d", &op2);

    char opbuf[50];
    sprintf(opbuf, "%d", op1);
    sendto(sock, opbuf, strlen(opbuf),0, (struct sockaddr*) &serverAddr, serverLength);
    sprintf(opbuf, "%d", op2); 
    sendto(sock, opbuf, strlen(opbuf),0, (struct sockaddr*) &serverAddr, serverLength);

    // 10) ricezione risultato
    r = recvfrom(sock, buffer, sizeof(buffer)-1, 0, (struct sockaddr*) &serverAddr, &serverLength);
    buffer[r] = '\0';
    printf("Risultato: %s\n", buffer);


    // chiusura socket, pulizia buffer e termine del programma
    closesocket(sock);
    WSACleanup();
    return 0;
}

