using System;
using System.IO;
using System.Text;
using System.Threading.Tasks;
using System.Collections.Generic;

using System.Net;
using System.Net.Http;
using System.Net.Http.Headers;

using Newtonsoft.Json;
using Newtonsoft.Json.Linq;
using System.Windows.Documents;
using System.Collections;

using System.Net.WebSockets;
using System.Threading;
using System.Windows;
using System.Security.Policy;
using static System.Net.WebRequestMethods;

namespace GCodeCombiner
{
    public class CncFile
    {
        public String Name;
        public String Size;

        public String FullName
        {
            get { return $"{Name} {Size}"; }
        }
    }

    public class Web
    {
        static ClientWebSocket websocket;

        private static void HandleMessage(byte[] buffer, int count)
        {
            System.Windows.MessageBox.Show($"Received {System.Text.Encoding.ASCII.GetString(buffer, 0, count)}");
        }

        public static async void WebscoketSetup()
        {
            new Thread(async () =>
            {
                using (var client = new HttpClient())
                {
                    var url = $"http://192.168.1.71/command?commandText=?";

                    while (true)
                    {
                        await client.GetAsync(url);

                        Thread.Sleep(1000);
                    }
                }


            }).Start();

            using (websocket = new ClientWebSocket())
            {
                byte[] buffer = new byte[256];

                while (true)
                {
                    await websocket.ConnectAsync(new Uri("ws://192.168.1.71:81"), CancellationToken.None);

                    System.Windows.MessageBox.Show($"Connected");

                    while (websocket.State == WebSocketState.Open)
                    {
                        var result = await websocket.ReceiveAsync(new ArraySegment<byte>(buffer), CancellationToken.None);

                        HandleMessage(buffer, result.Count);

                        if (result.MessageType == WebSocketMessageType.Close)
                        {
                            await websocket.CloseAsync(WebSocketCloseStatus.NormalClosure, string.Empty, CancellationToken.None);
                        }
                    }
                }
            }
        }
        public static async Task UploadFile(string fileName, String contentData)
        {
            var boundary = "----WebKitFormBoundaryHEwnGACfAY4a1D2c";

            var content_src = $"--{boundary}\r\nContent-Disposition: form-data; name=\"path\"\r\n\r\n/\r\n" +
            $"--{boundary}\r\nContent-Disposition: form-data; name=\"/{fileName}S\"\r\n\r\n{contentData.Length}\r\n" +
            $"--{boundary}\r\nContent-Disposition: form-data; name=\"myfile[]\"; filename=\"/{fileName}\"\r\n" +
            $"Content-Type: application/octet-stream\r\n\r\n";

            // Create the request and set parameters
            HttpWebRequest request = (HttpWebRequest)WebRequest.Create("http://192.168.1.71/upload");
            request.ContentType = $"multipart/form-data; boundary={boundary}";

            request.Method = "POST";
            request.KeepAlive = true;

            request.Credentials = System.Net.CredentialCache.DefaultCredentials;

            var requestStream = new StreamWriter(request.GetRequestStream(), Encoding.ASCII);

            requestStream.Write(content_src);

            requestStream.Flush();

            int bytesRead;

            byte[] buffer = new byte[2048];

            using (var fileStream = new MemoryStream(Encoding.ASCII.GetBytes(contentData)))
            {
                while ((bytesRead = fileStream.Read(buffer, 0, buffer.Length)) != 0)
                {
                    requestStream.BaseStream.Write(buffer, 0, bytesRead);
                }

                requestStream.BaseStream.Flush();

                fileStream.Close();
            }

            requestStream.Write("\r\n--" + boundary + "--\r\n");

            requestStream.Close();

            using (StreamReader reader = new StreamReader(request.GetResponse().GetResponseStream()))
            {
                System.Windows.MessageBox.Show(reader.ReadToEnd());
            };
        }
        
        public static async Task RunFile(String fileName)
        {
            using (var client = new HttpClient())
            {
                var url = $"http://192.168.1.71/command?commandText=[ESP220]/{fileName}&PAGEID";

                await client.GetAsync(url);
            }
        }

        public static async Task<List<CncFile>> GetFileList()
        {
            using (var client = new HttpClient())
            {
                var url = "http://192.168.1.71/upload?path=/&PAGEID=0";

                var result = await client.GetAsync(url);

                var responce = await result.Content.ReadAsStringAsync();

                var files = new List<CncFile>();

                dynamic obj = JObject.Parse(responce);

                foreach(var file in obj.files.Children())
                {
                    var fileName = (string)file.name;

                    if (fileName.StartsWith("System Volume Information"))
                        continue;

                    files.Add(new CncFile { Name = file.name, Size = file.size });
                }

                return files;
            }
        }
    }
}