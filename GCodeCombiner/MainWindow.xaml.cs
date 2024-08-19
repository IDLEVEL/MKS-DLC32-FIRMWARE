using System;
using System.IO;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Windows;
using System.Windows.Controls;
using Microsoft.Win32;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Media;

namespace GCodeCombiner
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        private ListView _list_view;
        private TextBox _gcode_box;

        private IList<FileItem> _file_items = new ObservableCollection<FileItem>();

        public MainWindow()
        {
            InitializeComponent();

            _list_view = FindName("FileItems") as ListView;
            _gcode_box = FindName("GCodeViewer") as TextBox;

            _list_view.ItemsSource = _file_items;
            
        }

        private String ReadFileIfExistsOrEmpty(string path)
        {
            if(File.Exists(path))
                return File.ReadAllText(path);

            return null;
        }

        private void SaveGcode(object sender, EventArgs e)
        {
            var fileDialog = new SaveFileDialog();
            fileDialog.Filter = "GCode Files|*.*";
            fileDialog.Title = "Select GCode file(s)...";
            fileDialog.FileName = "output.gcode";
            
            var success = fileDialog.ShowDialog();

            if (success.GetValueOrDefault())
            {
                var gcodeDir = Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "gcode");

                var allGcode = new System.Text.StringBuilder();

                var appendGcode = ReadFileIfExistsOrEmpty(Path.Combine(gcodeDir, "append.gcode"));
                var prependGcode = ReadFileIfExistsOrEmpty(Path.Combine(gcodeDir, "prepend.gcode"));
                var concatGcode = ReadFileIfExistsOrEmpty(Path.Combine(gcodeDir, "concat.gcode"));
                
                if(prependGcode != null)
                {
                    allGcode.AppendLine(prependGcode);
                }

                var selected_file_items = _file_items.Where(f => f.Selected).ToArray();

                for(int i = 0; i < selected_file_items.Length; i++)
                {
                    var lines = File.ReadAllText(selected_file_items[i].FilePath);

                    allGcode.AppendLine(lines.Replace("M30", ""));

                    if(i < selected_file_items.Length - 1)
                        allGcode.AppendLine(concatGcode);
                }

                if(appendGcode != null)
                {
                    allGcode.AppendLine(appendGcode);
                }

                allGcode.AppendLine("M30");

                File.WriteAllText(fileDialog.FileName, allGcode.ToString());
            }
        }

        private void DeleteFileItem(object sender, EventArgs args)
        {
            var fileItem = (FileItem)(((Button)sender).DataContext);

            _file_items.Remove(fileItem);
        }

        private Button previousViewedButton;

        private void ViewFileItem(object sender, EventArgs args)
        {
            var button = ((Button)sender);
            var fileItem = (FileItem)(button.DataContext);

            _gcode_box.Dispatcher.Invoke(() =>
                _gcode_box.Text = File.ReadAllText(fileItem.FilePath));

            if(previousViewedButton != null)
                previousViewedButton.Background = new SolidColorBrush(Colors.White);
            
            previousViewedButton = button;
            button.Background = new SolidColorBrush(Colors.Red);
        }

        private void OpenFile(object sender, EventArgs e)
        {
            var fileDialog = new OpenFileDialog();
            fileDialog.Filter = "GCode Files (*.*)|*.*";
            fileDialog.Title = "Select GCode file(s)...";
            fileDialog.Multiselect = true; // Set to false or never mention this line for single file select

            var success = fileDialog.ShowDialog();

            if (success.GetValueOrDefault())
            {
                foreach(var file in fileDialog.FileNames)
                    _file_items.Add(new FileItem {FilePath = file, Selected = true});
            }
        }


        private void ImagePanel_Drop(object sender, DragEventArgs e)
        {

            if (e.Data.GetDataPresent(DataFormats.FileDrop))
            {
                // Note that you can have more than one file.
                string[] files = (string[])e.Data.GetData(DataFormats.FileDrop);

                foreach(var file in files)
                    _file_items.Add(new FileItem {FilePath = file, Selected = true});
            }
        }
    }
}
