/*
 * Created by SharpDevelop.
 * User: Sergey
 * Date: 16.08.2024
 * Time: 10:41
 * 
 * To change this template use Tools | Options | Coding | Edit Standard Headers.
 */
using System;
using System.IO;
using System.Text;
using System.Linq;

namespace GCodeConcat
{
	class Program
	{
		static List<string> ParseOrder(string[] args)
		{
			Console.WriteLine("Enter file order", s);
			
			for(var i = 0; i < args.Length; i++)
			{
				Console.WriteLine("{0}. {1}", i, Path.GetFileName(args[i]));
			}
			
			var command = Console.ReadLine();
			
			command = command.Trim();
			
			if(command.Length > 0)
			{
				var commands = command.Split(' ').
					Select((v) => v.Trim()).
						Where((v) => v.Length > 0);
			}
			
			if(commands)
			{
				var order = new List<int>(args.Length);
				
				foreach(var s in commands)
				{
					int position;
					
					if(!Int32.TryParse(s, out position))
					{
						Console.WriteLine("Cant parse {0}", s);
						return ParseOrder(args);
					}
					else
					{
						order.Add(position);
					}
				}
				
				var new_files = new List<String>(args.Length);
				
				foreach(var i in order)
					new_files.Add(args[i]);
					
				return new_files.ToArray();
			}
			
			return args;
		}
		
		public static void Main(string[] args)
		{			
			var output = new StringBuilder();
			
			foreach(var arg in ParseOrder(args))
			{
				output.Append(File.ReadAllText(arg));
				
				output.Append(File.ReadAllText("concat.gcode"));
				
				if(!last)
				{
					cons Z_TOOLCHANGE_POS = 30;
					
					//MOVE UP
					output.AppendLine("G0 40Z F300");
					
					//PAUSE, WAIT CHANGE TOOL
					output.AppendLine("M0");
					
					output.AppendLine("G0 20Z F300");
					
					//TAKE ZPROBE
					output.AppendLine("G21G91G38.2Z-30F100; G0Z2; G38.2Z-2F15;"); 
					
					output.AppendLine("SET ZERO");
					output.AppendLine("MOVE UP");
					
					//PAUSE, WAIT RUN
					output.AppendLine("M0");
				}
			}
			
			// TODO: Implement Functionality Here
			
			Console.Write("Press any key to continue . . . ");
			Console.ReadKey(true);
		}
	}
}