using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace lab4
{
    public class UserManager
    {
        private const string UsersFilePath = "users.txt";

        // Метод для проверки, есть ли логин в файле
        public static bool IsLoginExist(string login)
        {
            if (!File.Exists(UsersFilePath)) return false;

            var users = File.ReadAllLines(UsersFilePath);
            return users.Any(line => line.Split(':')[0] == login);
        }

        // Метод для проверки, верны ли логин и пароль
        public static bool IsValidLogin(string login, string password)
        {
            if (!File.Exists(UsersFilePath)) return false;

            var users = File.ReadAllLines(UsersFilePath);
            return users.Any(line => line == $"{login}:{password}");
        }

        // Метод для регистрации нового пользователя
        public static bool RegisterUser(string login, string password)
        {
            if (IsLoginExist(login))
            {
                return false; // Логин уже существует
            }

            using (StreamWriter writer = new StreamWriter(UsersFilePath, true))
            {
                writer.WriteLine($"{login}:{password}");
            }
            return true;
        }
    }
}
