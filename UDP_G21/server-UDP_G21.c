/*  _._._._._._._._._._._._._._._._._._

    Corso Reti di Calcolatori - CORSO M-Z 2025/2026
    Esercizio n.2 - Server UDP di MARSICO Luciano Antonio
    
    _._._._._._._._._._._._._._._._._._
 */

#include <stdio.h>
#include <winsock2.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#pragma comment(lib,"ws2_32.lib")

int main() {

    WSADATA wsaData;
    WSAStartup(MAKEWORD(2,2),&wsaData);

    // configurazione server

    SOCKET sock = socket(AF_INET, SOCK_DGRAM,0);
    struct sockaddr_in serverAddr, clientAddr;
    int clientLength = sizeof(clientAddr);

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(6543);
    
    bind(sock,(struct sockaddr*)&serverAddr,sizeof(serverAddr));

    char buffer[1024];


    // loop
    while(1) { // riceve messaggio e memorizza addr del mittente
        int r = recvfrom(sock, buffer, sizeof(buffer)-1,0,(struct sockaddr*)&clientAddr,&clientLength);
        buffer[r]='\0';

        char c = tolower(buffer[0]); // come cmd in TCP

        char response[50];
        int readOrNot = 0; // booleano, settato a 1 se l'operazione richiede due operandi, 0 se termina il programma

        switch(c) { // menu
            case 'a': strcpy(response,"ADDIZIONE"); readOrNot = 1; 
            break;
            case 's': strcpy(response,"SOTTRAZIONE"); readOrNot = 1; 
            break;
            case 'm': strcpy(response,"MOLTIPLICAZIONE"); readOrNot = 1; 
            break;
            case 'd': strcpy(response,"DIVISIONE"); readOrNot = 1; 
            break;
            default: strcpy(response,"TERMINE PROCESSO CLIENT"); readOrNot = 0; 
            break;
        }

        sendto(sock,response,strlen(response),0,(struct sockaddr*)&clientAddr,clientLength);

        if(!readOrNot) continue; // quando readOrNot è 0, il server non legge i due operandi

        // ricezione dei due interi
        r = recvfrom(sock, buffer, sizeof(buffer)-1,0,(struct sockaddr*)&clientAddr,&clientLength);
        int op1 = atoi(buffer);
        r = recvfrom(sock, buffer, sizeof(buffer)-1,0,(struct sockaddr*)&clientAddr,&clientLength);
        int op2 = atoi(buffer);

        int risultato = 0;
        char res_str[50];

        switch(c) {
            case 'a': risultato = op1 + op2; 
            break;
            case 's': risultato = op1 - op2; 
            break;
            case 'm': risultato = op1 * op2; 
            break;

            case 'd': 
                if(op2!=0) risultato = op1/op2;
                else { strcpy(res_str,"ERRORE: divisione per zero");
                     sendto(sock,res_str,strlen(res_str),0,(struct sockaddr*)&clientAddr,clientLength); 
                     continue;
                    }
                break;
        }
        sprintf(res_str,"%d", risultato); // converte risultato in stringa 

        sendto(sock,res_str,strlen(res_str),0,(struct sockaddr*)&clientAddr,clientLength); //invio del risultato al client
    
    }

    //pulizia e terminazione del programma
    closesocket(sock);
    WSACleanup();
    return 0;
}


