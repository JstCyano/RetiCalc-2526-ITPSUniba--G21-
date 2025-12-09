/*  _._._._._._._._._._._._._._._._._._

    Corso Reti di Calcolatori - CORSO M-Z 2025/2026
    Esercizio n.1 - Server TCP di calcolo di MARSICO Luciano Antonio
    
    _._._._._._._._._._._._._._._._._._
 */


#include <stdio.h> 
#include <winsock2.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h> 

#pragma comment(lib, "ws2_32.lib") // funzioni di Winsock

int main() {
    WSADATA wsaData;
    SOCKET listenSocket;

    struct sockaddr_in serverAddr, clientAddr;
    char buffer[1024]; //array dove salvare dati ricevuti

    // configurazione server

    WSAStartup(MAKEWORD(2,2), &wsaData);

    listenSocket = socket(AF_INET, SOCK_STREAM, 0); 
    
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY; 
    serverAddr.sin_port = htons(6543);

    bind(listenSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    listen(listenSocket, 1); // per gestire il backlog secondo quanto richiesto dalla traccia
    printf("Il Server TCP e' in ascolto sulla porta 6543.\n");


    // loop
    while (1) {  
        int clientLength = sizeof(clientAddr); // punto 1
        SOCKET clientSocket = accept(listenSocket, (struct sockaddr*)&clientAddr, &clientLength); 

        if (clientSocket != INVALID_SOCKET) {
             printf("\n--- Nuovo Client Connesso con successo ---\n");

            // punto 4 (conferma connessione avvenuta tramite msg_conn)
            char *msg_conn = "connessione avvenuta";
            send(clientSocket, msg_conn, strlen(msg_conn), 0);

            // punto 7
            int bytesR = recv(clientSocket, buffer, sizeof(buffer)-1, 0); //dimensione max del buffer
            if (bytesR <= 0) {  // in caso di errore
                closesocket(clientSocket); 
                continue; 
            }

            buffer[bytesR] = '\0'; //terminatore di stringa

            char scelta = tolower(buffer[0]); // distinzione min-maiusc per A-a, S-s, M-m, D-d
            char risp[50]; // risposta che il server invia al client

            // finto booleano per leggere o meno gli operandi inseriti dall'user
            int readOrNot = 0; // uso 0 e 1 per non includere stdbool.h

            switch(scelta) {
                case 'a': strcpy(risp,"ADDIZIONE"); readOrNot = 1; 
                break;
                case 's': strcpy(risp,"SOTTRAZIONE"); readOrNot = 1; 
                break;
                case 'm': strcpy(risp,"MOLTIPLICAZIONE"); readOrNot = 1; 
                break;
                case 'd': strcpy(risp,"DIVISIONE"); readOrNot = 1; 
                break;
                default: strcpy(risp,"TERMINE PROCESSO CLIENT"); readOrNot = 0; // termina processo senza leggere operandi
                break;
            }
            

            // invia al client conferma
            send(clientSocket, risp, strlen(risp), 0);

            if (readOrNot) {
                int op1, op2;
                char op_buffer[50]; // array temporaneo per leggere operandi come stringhe

                // per il primo op
                bytesR = recv(clientSocket, op_buffer, sizeof(op_buffer)-1, 0);
                if (bytesR <= 0) {
                    closesocket(clientSocket); 
                    continue; 
                }

                op_buffer[bytesR] = '\0'; 
                op1 = atoi(op_buffer); //converte stringa in int

                // per il secondo op
                bytesR = recv(clientSocket, op_buffer, sizeof(op_buffer)-1, 0);

                if (bytesR <= 0) { 
                    closesocket(clientSocket); 
                    continue; 
                }
                
                op_buffer[bytesR] = '\0';
                op2 = atoi(op_buffer);

                int risultato = 0;
                char risultato_str[50];

                switch(scelta) {
                    case 'a': risultato = op1 + op2; 
                    break;
                    case 's': risultato = op1 - op2; 
                    break;
                    case 'm': risultato = op1 * op2; 
                    break;
                    case 'd': 
                        if (op2 != 0) {
                            risultato = op1 / op2;
                        } else { //check per divisione per 0
                            strcpy(risultato_str,"ERRORE: divisione per zero"); 
                            send(clientSocket,risultato_str,strlen(risultato_str),0); 
                            closesocket(clientSocket); 
                            continue; 
                        }
                    break;
                }

                sprintf(risultato_str,"%d", risultato); // sprintf converte risultato in stringa
                send(clientSocket,risultato_str,strlen(risultato_str),0); // invio del risultato al client
            }

            closesocket(clientSocket); // chiusura socket del client attuale
            printf("Client disconnesso con successo. In attesa di nuove connessioni.\n");
        }
    }

    //pulizia e terminazione del programma
    closesocket(listenSocket);
    WSACleanup();
    return 0; 
}
