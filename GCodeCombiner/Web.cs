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
        public static async Task UploadFile(string fileName, byte[] file)
        {   
            /*
            var content = $"------WebKitFormBoundaryHEwnGACfAY4a1D2c\r\nContent-Disposition: form-data; name=\"path\"\r\n\r\n/\r\n" + 
            "------WebKitFormBoundaryHEwnGACfAY4a1D2c\r\nContent-Disposition: form-data; name=\"/sd2.ncS\"\r\n\r\n503661\r\n" + 
            "------WebKitFormBoundaryHEwnGACfAY4a1D2c\r\nContent-Disposition: form-data; name=\"myfile[]\"; filename=\"/sd2.nc\"\r\n"+ 
            "Content-Type: application/octet-stream\r\n\r\n\r\n------WebKitFormBoundaryHEwnGACfAY4a1D2c--\r\n";
*/

            try
            {
                using (var client = new HttpClient())
                {
                    using (var content = new MultipartFormDataContent("------WebKitFormBoundary6pu7KoD3jOolREHM"))
                    {
                        var byteArrayContent = new ByteArrayContent(file);
                        
                        byteArrayContent.Headers.ContentType = MediaTypeHeaderValue.Parse("application/octet-stream");
                        
                        content.Add(byteArrayContent, "myfile", $"/{fileName}");

                        content.Add(new StringContent("/"), "path");

                        content.Add(new StringContent(file.Length.ToString()), $"/{fileName}S");

                        System.Windows.MessageBox.Show(await content.ReadAsStringAsync());

                        var url = "http://192.168.1.71/upload";
                        var result = await client.PostAsync(url, content);


                        var responce = await result.Content.ReadAsStringAsync();

                        System.Windows.MessageBox.Show(file.Length.ToString() + ":" + responce);
                    }
                }
            }
            catch (Exception ex)
            {
                System.Windows.MessageBox.Show(ex.ToString());
            }
        }
    }
}