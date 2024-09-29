using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Reflection.Emit;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace _20
{
    public partial class AggregationDialog : Form
    {
        public AggregationDialog()
        {
            InitializeComponent();
        }
        public void SetMessage(string message) { label1.Text = message; }
    }
}
