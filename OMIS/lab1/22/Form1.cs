using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace _22
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            Dialog dialog = new Dialog(this);
            dialog.ShowDialog();
        }
        public string RichTextBoxText1
        {
            get { return richTextBox1.Text; }
            set { richTextBox1.Text = value; }
        }
    }
}
