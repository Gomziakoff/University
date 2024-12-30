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

        // Событие, вызываемое при загрузке формы
        private void Form1_Load(object sender, EventArgs e)
        {
            MessageBox.Show("form loaded"); // Уведомление о загрузке формы
        }

        // Событие, вызываемое при закрытии формы
        private void Form1_FormClosed(object sender, FormClosedEventArgs e)
        {
            MessageBox.Show("form closed"); // Уведомление о закрытии формы
        }

        // Событие, вызываемое при щелчке мыши на форме
        private void Form1_MouseClick(object sender, MouseEventArgs e)
        {
            // Отображение координат щелчка мыши
            MessageBox.Show("X:" + e.X + "Y:" + e.Y);

            // Обновление формы (перерисовка)
            this.Refresh();
        }

        // Событие, вызываемое при нажатии клавиши
        private void Form1_KeyPress(object sender, KeyPressEventArgs e)
        {
            // Отображение нажатой клавиши
            MessageBox.Show("Нажата клавиша: " + e.KeyChar.ToString());

            // Закрытие приложения при нажатии клавиши Esc
            if (e.KeyChar == (char)27)
            {
                Application.Exit();
            }
        }

        // Событие, вызываемое при отображении формы после её загрузки
        private void Form1_Shown(object sender, EventArgs e)
        {
            MessageBox.Show("Форма показана"); // Уведомление о показе формы
        }

        private int drawAmount = 0; // Счетчик перерисовок формы

        // Событие, вызываемое при перерисовке формы
        private void Form1_Paint(object sender, PaintEventArgs e)
        {
            Graphics graphics = e.Graphics; // Получение графического контекста для рисования
            drawAmount++; // Увеличение счетчика перерисовок

            // Настройка шрифта и кисти для текста
            Font font = new Font("AriaL", 10);
            SolidBrush myBrush = new SolidBrush(Color.Black);

            // Установка позиции для текста
            Point point = new Point(40, 80);

            // Рисование текста с информацией о количестве перерисовок
            graphics.DrawString("Перерисовки:" + drawAmount.ToString() + "\n" + "DA=0" + "\n" + "A=1", font, myBrush, point);
        }

    }
}
