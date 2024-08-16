/*
 * Created by SharpDevelop.
 * User: Sergey
 * Date: 16.08.2024
 * Time: 14:55
 * 
 * To change this template use Tools | Options | Coding | Edit Standard Headers.
 */
using System;
using System.Collections.Generic;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;

namespace GCodeConcat
{
	/// <summary>
	/// Interaction logic for ConcatWindow.xaml
	/// </summary>
	public partial class ConcatWindow : Window
	{
		public ConcatWindow()
		{
			InitializeComponent();
		}
		
		void Grid_Drop(object sender, DragEventArgs e)
		{
			throw new NotImplementedException();
		}
		
		void Grid_PreviewDrop(object sender, DragEventArgs e)
		{
			throw new NotImplementedException();
		}
	}
}