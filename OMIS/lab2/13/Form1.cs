using System;
using System.IO;
using System.Linq;
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
            try
            {
                int value = Convert.ToInt32(valueTB.Text);
                double result = Convert.ToDouble(Math.Sqrt(value));
                resultTB.Text = result.ToString();
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.ToString());
                return;
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

        // Новый обработчик для кнопки "Add List"
        private void addList_button_Click(object sender, EventArgs e)
        {
            // Открываем диалог для выбора файла
            OpenFileDialog openFileDialog = new OpenFileDialog();
            openFileDialog.Filter = "Text Files|*.txt";
            openFileDialog.Title = "Select a Text File";

            // Если файл выбран
            if (openFileDialog.ShowDialog() == DialogResult.OK)
            {
                try
                {
                    // Чтение строк из файла
                    string[] lines = File.ReadAllLines(openFileDialog.FileName);

                    // Дебаг: Вывод всех строк для проверки
                    Console.WriteLine("Loaded lines:");
                    foreach (var line in lines)
                    {
                        Console.WriteLine($"'{line}'");
                    }

                    // Фильтруем строки, оставляя только те, которые можно преобразовать в числа
                    var numbers = lines
                                  .Where(line => double.TryParse(line, out _)) // Проверка на числа с плавающей точкой
                                  .ToArray();

                    // Дебаг: Вывод отфильтрованных строк
                    Console.WriteLine("Filtered numbers:");
                    foreach (var number in numbers)
                    {
                        Console.WriteLine($"'{number}'");
                    }

                    // Проверяем, не пустой ли файл после фильтрации
                    if (numbers.Length == 0)
                    {
                        MessageBox.Show("No valid numbers found in the file.");
                        return;
                    }

                    // Очистить текущий список перед загрузкой нового
                    listBox1.Items.Clear();

                    // Добавляем числа в ListBox
                    listBox1.Items.AddRange(numbers);
                }
                catch (Exception ex)
                {
                    MessageBox.Show("Error loading file: " + ex.Message);
                }
            }
        }
    }
}
