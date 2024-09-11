using System;
using System.IO;
using System.Text;
using System.Threading.Tasks;

using System.Net;
using System.Net.Http;
using System.Net.Http.Json;
using System.Net.Http.Headers;

namespace GCodeCombiner
{
    public class Web
    {
        public static async Task UploadFile(string fileName, String contentData)
        {   
            var boundary = "----WebKitFormBoundaryHEwnGACfAY4a1D2c";

            var content_src = $"--{boundary}\r\nContent-Disposition: form-data; name=\"path\"\r\n\r\n/\r\n" + 
            $"--{boundary}\r\nContent-Disposition: form-data; name=\"/{fileName}S\"\r\n\r\n{contentData.Length}\r\n" + 
            $"--{boundary}\r\nContent-Disposition: form-data; name=\"myfile[]\"; filename=\"/{fileName}\"\r\n"+ 
            $"Content-Type: application/octet-stream\r\n\r\n{contentData}\r\n--{boundary}--\r\n";

            try
            {
                using (var client = new HttpClient())
                {
                        var content = new ByteArrayContent(Encoding.UTF8.GetBytes(content_src));
                        
                        content.Headers.Add("content-type", $"multipart/form-data; boundary={boundary}");

                        System.Windows.MessageBox.Show(await content.ReadAsStringAsync());

                        var url = "http://192.168.1.71/upload";

                        var result = await client.PostAsync(url, content);

                        var responce = await result.Content.ReadAsStringAsync();
                }
            }
            catch (Exception ex)
            {
                System.Windows.MessageBox.Show(ex.ToString());
            }
        }
    }
}