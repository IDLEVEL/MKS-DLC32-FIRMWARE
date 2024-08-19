using System;
using System.IO;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Windows;
using System.Windows.Controls;
using Microsoft.Win32;

namespace GCodeCombiner
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        private ListView _list_view;

        private IList<FileItem> _file_items = new ObservableCollection<FileItem>();

        public MainWindow()
        {
            InitializeComponent();

            _list_view = FindName("FileItems") as ListView;

            _list_view.ItemsSource = _file_items;
            
        }

        private void SaveGcode(object sender, EventArgs e)
        {
        }

        private void DeleteFileItem(object sender, EventArgs args)
        {
            var button = (args as RoutedEventArgs).Source as Button;
            var fileItem = button.DataContext as FileItem;

            _file_items.Remove(fileItem);
        }

        private void OpenFile(object sender, EventArgs e)
        {
            OpenFileDialog fileDialog = new OpenFileDialog();
            fileDialog.Filter = "JPEG Files (*.*)|*.*";
            fileDialog.Title = "Select image file(s)...";
            fileDialog.Multiselect = true; // Set to false or never mention this line for single file select

            var success = fileDialog.ShowDialog();

            if (success.Value)
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
