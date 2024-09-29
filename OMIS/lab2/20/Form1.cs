using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace _20
{
    public partial class Form1 : Form
    {
        private DependencyDialog dependencyDialog = new DependencyDialog(); 
        private AggregationDialog aggregationDialog = new AggregationDialog();
        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_MouseClick(object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Left)
            {
                dependencyDialog.SetMessage("Открыто через зависимость типа использования.");
                dependencyDialog.ShowDialog();
            }
            else if (e.Button == MouseButtons.Right)
            {
                aggregationDialog.SetMessage("Открыточерез агрегацию.");
                aggregationDialog.ShowDialog();
            }
        }
    }
}
