using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace _13
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void calc_button_Click(object sender, EventArgs e)
        {
            try { int value = Convert.ToInt32(valueTB.Text); 
                double result = Convert.ToDouble(Math.Sqrt(value)); 
                resultTB.Text = result.ToString(); 
            } 
            catch (Exception ex) 
            { 
                MessageBox.Show(ex.ToString()); return; 
            }
        }

        private void del_button_Click(object sender, EventArgs e)
        {
            if (listBox1.SelectedIndex != -1) 
            { 
                listBox1.Items.RemoveAt(listBox1.SelectedIndex); 
            }
        }

        private void add_button_Click(object sender, EventArgs e)
        {
            if (toAddTextBox.Text.Length > 0) 
            { 
                listBox1.Items.Add(toAddTextBox.Text); 
                toAddTextBox.Text = ""; 
            }
        }

        private void listBox1_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (listBox1.SelectedIndex != -1) 
            { 
                selectedTB.Text = listBox1.Items[listBox1.SelectedIndex].ToString(); 
            }
        }
    }
}
