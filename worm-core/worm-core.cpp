#include <iostream>
#include <windows.h>

int main()
{
    std::cout << "Hello World!\n";


	// Connect with the server, send/recieve messages

	// Change desktop background
	int return_value = SystemParametersInfo(SPI_SETDESKWALLPAPER, 0, (PVOID)L"C:/Users/Matthew/Pictures/Prom/prom.JPG", SPIF_UPDATEINIFILE);
	
	// Encrypt file on desktop

}
