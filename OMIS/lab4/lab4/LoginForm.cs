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
    public partial class LoginForm : Form
    {
        public LoginForm()
        {
            InitializeComponent();
        }

        private void buttonLogin_Click(object sender, EventArgs e)
        {
            string login = textBoxLogin.Text; 
            string password = textBoxPassword.Text;

            if (UserManager.IsValidLogin(login,password))
            {
                MessageBox.Show("Успешный вход!");
                this.DialogResult = DialogResult.OK;
            }
            else
            {
                MessageBox.Show("Неверный логин или пароль!");
            }
        }

        private void buttonRegister_Click(object sender, EventArgs e)
        {
            string login = textBoxLogin.Text;
            string password = textBoxPassword.Text;

            if (UserManager.RegisterUser(login, password))
            {
                MessageBox.Show("Регистрация успешна!");
            }
            else
            {
                MessageBox.Show("Логин уже существует!");
            }
        }
    }
}
