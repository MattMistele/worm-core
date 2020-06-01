using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace worm_core_new
{
    class Program
    {
        [DllImport("user32.dll", SetLastError = true)]
        [return: MarshalAs(UnmanagedType.Bool)]
        static extern bool SystemParametersInfo(uint uiAction, uint uiParam, String pvParam, uint fWinIni);

        private const uint SPI_SETDESKWALLPAPER = 0x14;
        private const uint SPIF_UPDATEINIFILE = 0x1;

        static void Main(string[] args)
        {
            // Change desktop background
            SystemParametersInfo(SPI_SETDESKWALLPAPER, 0, @"C:/Users/Matthew/Pictures/Drivers License.JPG", SPIF_UPDATEINIFILE);

            //Encrypting file on desktop 

            //Get system name, user logged in

            //HTTP GET request for IP Address 

            //Location from keyboard layout or IP Address for another GET 

            //Call kade's function or work with kade to send everything to server 
        }
    }
}
