using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace lab4
{
    public partial class MainForm : Form
    {
        private string currentFilePath;
        public MainForm()
        {
            InitializeComponent();
        }

        private void LoadData(string filePath)
        {
            if (!File.Exists(filePath))
            {
                MessageBox.Show("Файл записей не найден.");
                return;
            }
            currentFilePath = filePath;
            var dataTable = new DataTable();
            dataTable.Columns.Add("Год", typeof(int));
            dataTable.Columns.Add("Расходы", typeof(int));
            dataTable.Columns.Add("Доходы", typeof(int));

            using (var reader = new StreamReader(filePath))
            {
                while (!reader.EndOfStream)
                {
                    var line = reader.ReadLine();
                    var values = line.Split(',');

                    if (values.Length == 3 &&
                        int.TryParse(values[0], out int year) &&
                        int.TryParse(values[1], out int expenses) &&
                        int.TryParse(values[2], out int income))
                    {
                        dataTable.Rows.Add(year, expenses, income);
                    }
                }
            }

            dataGridView1.DataSource = dataTable;
        }

        private void создатьНовыйСписокToolStripMenuItem_Click(object sender, EventArgs e)
        {
            using (var saveFileDialog = new SaveFileDialog())
            {
                saveFileDialog.Filter = "CSV файлы (*.csv)|*.csv";
                saveFileDialog.Title = "Выберите путь для сохранения нового списка";

                if (saveFileDialog.ShowDialog() == DialogResult.OK)
                {
                    var filePath = saveFileDialog.FileName;
                    CreateNewListFile(filePath);
                    LoadData(filePath); // Обновляем DataGridView с новым списком
                }
            }
        }

        private void CreateNewListFile(string filePath)
        {
            using (var writer = new StreamWriter(filePath))
            {
                writer.WriteLine("2023,2000,5000");
                writer.WriteLine("2024,300,6000");
                writer.WriteLine("2021,200,5000");
                writer.WriteLine("2022,3000,6000");
                writer.WriteLine("2023,20040,5000");
                writer.WriteLine("2022,30006,6000");
                writer.WriteLine("2021,20030,5000");
                writer.WriteLine("2007,4000,6000");
                writer.WriteLine("2023,2000,5000");
                writer.WriteLine("2022,6000,6000");
                writer.WriteLine("2023,60001,5000");
                writer.WriteLine("2024,3000,6000");
                writer.WriteLine("2023,2000,5000");
                writer.WriteLine("2024,3000,6000");
                writer.WriteLine("2023,2000,5000");
                writer.WriteLine("2024,3000,6000");
                writer.WriteLine("2023,2000,5000");
                writer.WriteLine("2024,3000,6000");
            }
        }

        private void открытьСуществующийСписокToolStripMenuItem_Click(object sender, EventArgs e)
        {
            using (var loadFileDialog = new OpenFileDialog())
            {
                loadFileDialog.Filter = "CSV файлы (*.csv)|*.csv";
                loadFileDialog.Title = "Выберите путь для доступа к списку";

                if (loadFileDialog.ShowDialog() == DialogResult.OK)
                {
                    var filePath = loadFileDialog.FileName;
                    LoadData(filePath); // Обновляем DataGridView с новым списком
                }
            }
        }

        private void просмотрСпискаToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (dataGridView1.DataSource == null)
            {
                MessageBox.Show("Список записей не загружен.");
                return;
            }

            // Создание окна для просмотра списка
            var viewForm = new ViewListForm();

            // Передача данных из DataGridView в ViewListForm
            foreach (DataGridViewRow row in dataGridView1.Rows)
            {
                if (!row.IsNewRow) // Исключаем пустую строку в конце DataGridView
                {
                    var year = row.Cells["Год"].Value?.ToString();
                    var expenses = row.Cells["Расходы"].Value?.ToString();
                    var income = row.Cells["Доходы"].Value?.ToString();

                    viewForm.AddRecord($"Год: {year}, Расходы: {expenses}, Доходы: {income}");
                }
            }

            viewForm.ShowDialog();
        }

        private void редактироватьЗаписьToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (dataGridView1.SelectedRows.Count == 0)
            {
                MessageBox.Show("Пожалуйста, выберите запись для редактирования.");
                return;
            }

            // Получение выбранной строки
            var selectedRow = dataGridView1.SelectedRows[0];

            // Извлечение текущих значений
            int year = (int)selectedRow.Cells["Год"].Value;
            int expenses = (int)selectedRow.Cells["Расходы"].Value;
            int income = (int)selectedRow.Cells["Доходы"].Value;

            // Открытие формы для редактирования
            using (var editForm = new EditRecordForm(year, expenses, income))
            {
                if (editForm.ShowDialog() == DialogResult.OK)
                {
                    // Обновление значений в DataGridView
                    selectedRow.Cells["Год"].Value = editForm.Year;
                    selectedRow.Cells["Расходы"].Value = editForm.Expenses;
                    selectedRow.Cells["Доходы"].Value = editForm.Income;

                    SaveData();
                    LoadData(currentFilePath);
                }
            }
           
        }
        private void SaveData()
        {
            if (string.IsNullOrEmpty(currentFilePath))
            {
                MessageBox.Show("Нет открытого файла для сохранения.");
                return;
            }

            using (var writer = new StreamWriter(currentFilePath))
            {
                foreach (DataGridViewRow row in dataGridView1.Rows)
                {
                    if (!row.IsNewRow) // Исключаем пустую строку в конце DataGridView
                    {
                        var year = row.Cells["Год"].Value?.ToString();
                        var expenses = row.Cells["Расходы"].Value?.ToString();
                        var income = row.Cells["Доходы"].Value?.ToString();
                        writer.WriteLine($"{year},{expenses},{income}");
                    }
                }
            }
        }

        private void удалитьЗаписьToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (dataGridView1.SelectedRows.Count > 0)
            {
                foreach (DataGridViewRow selectedRow in dataGridView1.SelectedRows)
                {
                    if (!selectedRow.IsNewRow)
                    {
                        dataGridView1.Rows.Remove(selectedRow);
                    }
                }
                SaveData(); // Сохранение изменений после удаления
            }
            else
            {
                MessageBox.Show("Пожалуйста, выберите запись для удаления.");
            }
        }
    }
}
