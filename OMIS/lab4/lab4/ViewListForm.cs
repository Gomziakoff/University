using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace lab4
{
    public partial class ViewListForm : Form
    {
        private ListBox listBox;

        public ViewListForm()
        {
            this.Text = "Просмотр списка";
            this.Size = new Size(400, 300);

            listBox = new ListBox
            {
                Dock = DockStyle.Fill
            };

            this.Controls.Add(listBox);
        }

        // Метод для добавления записи в ListBox
        public void AddRecord(string record)
        {
            listBox.Items.Add(record);
        }
    }
}
