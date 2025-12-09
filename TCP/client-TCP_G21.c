/*  _._._._._._._._._._._._._._._._._._

    Corso Reti di Calcolatori - CORSO M-Z 2025/2026
    Esercizio n.1 - Client TCP di MARSICO Luciano Antonio
    
    _._._._._._._._._._._._._._._._._._
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

int main() {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2,2), &wsaData);

    char nomeServer[200];
    printf("Inserisci il nome del server (es. localhost): ");
    scanf("%s", nomeServer);

    struct hostent *host = gethostbyname(nomeServer); // funzione per convertire in IP il nome del server tramite il DNS
    char *serverIP = inet_ntoa(*(struct in_addr*)host->h_addr);


    // config socket
    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serverAddr;

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(serverIP);
    serverAddr.sin_port = htons(6543);

    connect(sock,(struct sockaddr*)&serverAddr,sizeof(serverAddr)); // consente connessione con la porta del server

    char buffer[1024]; // piccoli interi, rischio of

    // punto 5, stampa stringa con terminatore ricevuta dal server
    int r = recv(sock, buffer, sizeof(buffer)-1,0);
    buffer[r] = '\0';
    printf("Server: %s\n", buffer);

    // punto 6, inoltro comando
    char cmd[10];
    printf("Inserisci una lettera (A/S/M/D o un altro input per terminare): ");
    scanf("%s", cmd);
    send(sock, cmd, strlen(cmd),0); // strlen per invio preciso dei byte

    // punto 8, ricezione della risposta del server dopo il cmd e la stampa
    r = recv(sock, buffer, sizeof(buffer)-1,0);
    buffer[r] = '\0';
    printf("Server: %s\n", buffer);

    // chiusura anticipata
    if(strcmp(buffer,"TERMINE PROCESSO CLIENT")==0) {
        closesocket(sock);
        return 0;
    }

    // inserimento due interi
    int op1, op2;
    printf("Inserisci primo intero: "); scanf("%d",&op1);
    printf("Inserisci secondo intero: "); scanf("%d",&op2);

    // conversione interi in stringhe
    char opbuf[50];
    sprintf(opbuf,"%d",op1);
    send(sock, opbuf, strlen(opbuf),0);
    sprintf(opbuf,"%d",op2);
    send(sock, opbuf, strlen(opbuf),0);

    // punto 10, risultato finale
    r = recv(sock, buffer, sizeof(buffer)-1,0);
    buffer[r] = '\0';
    printf("Risultato ottenuto: %s\n", buffer);


    // pulizia, chiusura della connessione e del programma
    closesocket(sock);
    WSACleanup();
    return 0;
}
