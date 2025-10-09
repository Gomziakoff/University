using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Windows.Forms;

namespace lab4
{
    public partial class ViewListForm : Form
    {
        private ListBox listBox;
        private Button sortAscButton;
        private Button sortDescButton;
        private List<string> records;

        public ViewListForm()
        {
            this.Text = "Просмотр списка";
            this.Size = new Size(400, 300);

            // Инициализация списка записей
            records = new List<string>();

            listBox = new ListBox
            {
                Dock = DockStyle.Top,
                Height = 200
            };

            // Кнопка для сортировки по возрастанию
            sortAscButton = new Button
            {
                Text = "Сортировка по возрастанию",
                Dock = DockStyle.Bottom
            };
            sortAscButton.Click += SortAscButton_Click;

            // Кнопка для сортировки по убыванию
            sortDescButton = new Button
            {
                Text = "Сортировка по убыванию",
                Dock = DockStyle.Bottom
            };
            sortDescButton.Click += SortDescButton_Click;

            // Добавление элементов управления на форму
            this.Controls.Add(listBox);
            this.Controls.Add(sortAscButton);
            this.Controls.Add(sortDescButton);
        }

        // Метод для добавления записи в ListBox
        public void AddRecord(string record)
        {
            records.Add(record);
            listBox.Items.Add(record);
        }

        // Сортировка по возрастанию
        private void SortAscButton_Click(object sender, EventArgs e)
        {
            var sortedRecords = records.OrderBy(r => r).ToList();
            listBox.Items.Clear();
            foreach (var record in sortedRecords)
            {
                listBox.Items.Add(record);
            }
        }

        // Сортировка по убыванию
        private void SortDescButton_Click(object sender, EventArgs e)
        {
            var sortedRecords = records.OrderByDescending(r => r).ToList();
            listBox.Items.Clear();
            foreach (var record in sortedRecords)
            {
                listBox.Items.Add(record);
            }
        }
    }
}
