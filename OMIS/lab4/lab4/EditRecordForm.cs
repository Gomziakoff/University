using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Runtime.Remoting.Metadata.W3cXsd2001;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace lab4
{
    public partial class EditRecordForm : Form
    {
        public int Year { get; private set; }
        public int Expenses { get; private set; }
        public int Income { get; private set; }

        public EditRecordForm(int year, int expenses, int income)
        {
            Year = year;
            Expenses = expenses;
            Income = income;

            InitializeComponent();

            textBoxYear.Text = year.ToString();
            textBoxIncome.Text = expenses.ToString();
            textBoxExpences.Text = expenses.ToString();
        }

        private void savebutton_Click(object sender, EventArgs e)
        {
            if (int.TryParse(textBoxYear.Text, out int year) &&
            int.TryParse(textBoxExpences.Text, out int expenses) &&
            int.TryParse(textBoxIncome.Text, out int income))
            {
                Year = year;
                Expenses = expenses;
                Income = income;
                this.DialogResult = DialogResult.OK;
                this.Close();
            }
            else
            {
                MessageBox.Show("Пожалуйста, введите корректные числовые значения.");
            }
        }

        private void cancelbutton_Click(object sender, EventArgs e)
        {
            DialogResult = DialogResult.Cancel;
            this.Close();
        }
    }
}
