using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO;

namespace _16
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void openToolStripMenuItem_Click(object sender, EventArgs e)
        {
            OpenFileDialog openFileDialog1 = new OpenFileDialog();
            openFileDialog1.Filter = "(*.txt)|*.txt";
            if (openFileDialog1.ShowDialog() == DialogResult.OK)
            {
                string filePath = openFileDialog1.FileName;
                string filecontent = File.ReadAllText(filePath);
                richTextBox1.Text = filecontent;
            }
        }

        private void saveAsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            SaveFileDialog saveFileDialog = new SaveFileDialog(); saveFileDialog.Filter = "(*.txt)|*.txt"; saveFileDialog.InitialDirectory = Environment.GetFolderPath(Environment.SpecialFolder.Desktop); if (saveFileDialog.ShowDialog() == DialogResult.OK)
            {
                string filePath = saveFileDialog.FileName; try { File.WriteAllText(filePath, richTextBox1.Text); MessageBox.Show("Файл был успешно сохранен!"); }
                catch (Exception ex)
                {
                    MessageBox.Show(ex.Message);
                }
            }
        }

        private void exitToolStripMenuItem_Click(object sender, EventArgs e)
        {
            DialogResult dr = MessageBox.Show("Exit?", "Warning", MessageBoxButtons.YesNo);
            if (dr == DialogResult.Yes) {
                if (richTextBox1.Modified)
                {
                    DialogResult dr2 = MessageBox.Show("You have unsaved part, exit?", "Warning", MessageBoxButtons.YesNo);
                    if (dr2 == DialogResult.Yes)
                    {
                        Application.Exit();
                    }
                    else { return; }
                }
                Application.Exit();
            }
        }
    }
}
