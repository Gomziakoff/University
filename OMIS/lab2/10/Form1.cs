using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace _10
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            MessageBox.Show("form loaded");
        }

        private void Form1_FormClosed(object sender, FormClosedEventArgs e)
        {
            MessageBox.Show("form closed");
        }

        private void Form1_MouseClick(object sender, MouseEventArgs e)
        {
            MessageBox.Show("X:" + e.X + "Y:" + e.Y);
            this.Refresh();
        }

        private void Form1_KeyPress(object sender, KeyPressEventArgs e)
        {
            MessageBox.Show("Нажата клавиша: " + e.KeyChar.ToString()); 
            if (e.KeyChar == (char)27) { Application.Exit(); }
        }

        private void Form1_Shown(object sender, EventArgs e)
        {
            MessageBox.Show("Форма показана");
        }

        private int drawAmount = 0;
        private void Form1_Paint(object sender, PaintEventArgs e)
        {
            Graphics graphics = e.Graphics; drawAmount++; 
            Font font = new Font("AriaL", 10); 
            SolidBrush myBrush = new SolidBrush(Color.Black); 
            Point point = new Point(40, 80); 
            graphics.DrawString("Перерисовки:" + drawAmount.ToString() + "\n" + "DA=0" + "\n" + "A=1", font, myBrush, point);
        }

    }
}
