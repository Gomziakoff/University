import tkinter as tk
from email.policy import default
from tkinter import ttk
import sqlite3
from tkinter import messagebox
from ttkthemes import ThemedTk
from tkinter import filedialog
import datetime

from create import create_tables


class DatabaseApp:
    def __init__(self, master, connection_params):
        self.master = master
        self.connection_params = connection_params
        self.master.title("Медицинская лаборатория")

        self.notebook = ttk.Notebook(master)
        self.notebook.pack(expand=True, fill='both')

        self.conn = sqlite3.connect(**connection_params)
        self.cursor = self.conn.cursor()

        self.table_names = self.get_table_names()
        self.default_values = None

        for table_name in self.table_names:
            frame = tk.Frame(self.notebook)
            self.notebook.add(frame, text=table_name)
            self.create_table_view(frame, table_name)

    def get_table_names(self):
        self.cursor.execute("""
        SELECT name 
        FROM sqlite_master 
        WHERE type='table' 
        AND name!='sqlite_sequence';
        """)
        table_names = [row[0] for row in self.cursor.fetchall()]
        return table_names

    def create_table_view(self, frame, table_name):
        self.cursor.execute(f"PRAGMA table_info({table_name});")
        columns = [row[1] for row in self.cursor.fetchall()]

        tree = ttk.Treeview(frame, columns=columns, show='headings', selectmode='browse')
        tree.pack(expand=True, fill='both')

        for col in columns:
            tree.heading(col, text=col, command=lambda c=col: self.sort_treeview(tree, table_name, c, False))
            tree.column(col, width=100, anchor='center')

        self.populate_treeview(tree, table_name)

        add_button = tk.Button(frame, text="Добавить", command=lambda: self.add_row(tree, table_name))
        add_button.pack(side=tk.LEFT, padx=10)

        delete_button = tk.Button(frame, text="Удалить", command=lambda: self.delete_row(tree, table_name))
        delete_button.pack(side=tk.LEFT, padx=10)

        edit_button = tk.Button(frame, text="Изменить", command=lambda: self.edit_row(tree, table_name))
        edit_button.pack(side=tk.LEFT, padx=10)

        refresh_button = tk.Button(frame, text="Обновить", command=lambda: self.populate_treeview(tree, table_name))
        refresh_button.pack(side=tk.LEFT, padx=10)

        search_entry = tk.Entry(frame)
        search_entry.pack(side=tk.LEFT, padx=10)

        search_button = tk.Button(frame, text="Поиск", command=lambda: self.search_treeview(tree, search_entry.get()))
        search_button.pack(side=tk.LEFT, padx=10)

        show_all_button = tk.Button(frame, text="Отчет",
                                    command=lambda: self.show_all_records_as_table(table_name))
        show_all_button.pack(side=tk.LEFT, padx=10)

        view_record_button = tk.Button(frame, text="Просмотр запись-за-записью",
                                       command=lambda: self.view_record_by_record(table_name))
        view_record_button.pack(side=tk.LEFT, padx=10)

        if table_name == "Результат":
            generate_receipt_button = tk.Button(frame, text="Напечатать результат",
                                                command=lambda: self.generate_receipt(tree))
            generate_receipt_button.pack(side=tk.LEFT, padx=10)

            generate_receipt_button = tk.Button(frame, text="Подробные результаты анализов",
                                                command=lambda: self.display_results())
            generate_receipt_button.pack(side=tk.LEFT, padx=10)

    def populate_treeview(self, tree, table_name):
        self.cursor.execute(f"SELECT * FROM {table_name};")
        data = self.cursor.fetchall()

        tree.delete(*tree.get_children())

        for row in data:
            tree.insert('', 'end', values=row)

    def add_row(self, tree, table_name):
        self.cursor.execute(f"PRAGMA table_info({table_name});")
        columns = [row[1] for row in self.cursor.fetchall()]

        self.update_default_values()

        add_dialog = tk.Toplevel(self.master)
        add_dialog.title("Добавить строку")

        entry_widgets = []
        for col in columns:
            # Создаем метку для каждого столбца
            label = tk.Label(add_dialog, text=col)
            label.grid(row=columns.index(col), column=0, padx=10, pady=5, sticky='e')

            # Получаем значение по умолчанию
            if self.default_values and col in self.default_values:
                default_value = (
                    self.default_values[col]() if callable(self.default_values[col]) else self.default_values[col]
                )
            else:
                default_value = ""

            # Устанавливаем значение в поле
            entry = tk.Entry(add_dialog)
            entry.insert(0, default_value)  # Устанавливаем значение по умолчанию
            entry.grid(row=columns.index(col), column=1, padx=10, pady=5, sticky='w')
            entry_widgets.append(entry)

        def insert_row():
            # Собираем значения из всех полей
            values = [entry.get() for entry in entry_widgets]

            # Проверка на пустые данные
            if any(value.strip() == "" for value in values):
                messagebox.showerror("Ошибка", "Все поля должны быть заполнены.")
                return

            try:
                placeholders = ', '.join(['?' for _ in values])
                query = f"INSERT INTO {table_name} ({', '.join(columns)}) VALUES ({placeholders});"
                self.cursor.execute(query, values)
                self.conn.commit()
                self.populate_treeview(tree, table_name)
                add_dialog.destroy()
            except sqlite3.Error as err:
                messagebox.showerror("Ошибка базы данных", f"Произошла ошибка: {err}")

        # Кнопка для подтверждения добавления строки
        submit_button = tk.Button(add_dialog, text="Подтвердить", command=insert_row)
        submit_button.grid(row=len(columns), columnspan=2, pady=10)

    def delete_row(self, tree, table_name):
        selected_item = tree.selection()
        if not selected_item:
            messagebox.showwarning("Предупреждение", "Пожалуйста, выберите строку для удаления.")
            return

        confirm = messagebox.askyesno("Подтверждение", "Вы уверены, что хотите удалить эту строку?")
        if not confirm:
            return

        values = tree.item(selected_item)['values']

        where_clause = ' AND '.join([f"{column} = ?" for column in tree['columns']])

        query = f"DELETE FROM {table_name} WHERE {where_clause};"
        self.cursor.execute(query, values)
        self.conn.commit()

        self.populate_treeview(tree, table_name)

    def edit_row(self, tree, table_name):
        selected_item = tree.selection()
        if not selected_item:
            messagebox.showwarning("Предупреждение", "Пожалуйста, выберите строку для изменения.")
            return

        values = tree.item(selected_item)['values']

        self.cursor.execute(f"PRAGMA table_info({table_name});")
        columns = [row[1] for row in self.cursor.fetchall()]

        edit_dialog = tk.Toplevel(self.master)
        edit_dialog.title("Изменить строку")

        entry_widgets = []
        for col, value in zip(columns, values):
            label = tk.Label(edit_dialog, text=col)
            label.grid(row=columns.index(col), column=0, padx=10, pady=5, sticky='e')
            entry = tk.Entry(edit_dialog)
            entry.insert(0, value)
            entry.grid(row=columns.index(col), column=1, padx=10, pady=5, sticky='w')
            entry_widgets.append(entry)

        def update_row():
            new_values = [entry.get() for entry in entry_widgets]
            set_clause = ', '.join([f"{column} = ?" for column in columns])
            where_clause = ' AND '.join([f"{column} = ?" for column in columns])
            query = f"UPDATE {table_name} SET {set_clause} WHERE {where_clause};"
            self.cursor.execute(query, new_values + values)
            self.conn.commit()
            self.populate_treeview(tree, table_name)
            edit_dialog.destroy()

        submit_button = tk.Button(edit_dialog, text="Подтвердить", command=update_row)
        submit_button.grid(row=len(columns), columnspan=2, pady=10)

    def sort_treeview(self, tree, table_name, column, reverse):
        query = f"SELECT * FROM {table_name} ORDER BY {column} {'DESC' if reverse else 'ASC'};"
        self.cursor.execute(query)
        data = self.cursor.fetchall()

        tree.delete(*tree.get_children())

        for row in data:
            tree.insert('', 'end', values=row)

        tree.heading(column, command=lambda: self.sort_treeview(tree, table_name, column, not reverse))

    def search_treeview(self, tree, search_term):
        for item in tree.get_children():
            values = tree.item(item)['values']

            if any(str(search_term).lower() in str(value).lower() for value in values):
                tree.selection_add(item)
            else:
                tree.selection_remove(item)

    def generate_receipt(self, tree):
        selected_item = tree.selection()
        if not selected_item:
            messagebox.showwarning("Предупреждение", "Пожалуйста, выберите строку для генерации отчета.")
            return

        # Получаем выбранные значения из Treeview
        values = tree.item(selected_item)['values']
        образец_id = values[0]

        # Генерируем отчет
        self.generate_receipt_function(образец_id)

    def generate_receipt_function(self, образец_id):
        # Запрашиваем путь для сохранения файла
        file_path = filedialog.asksaveasfilename(defaultextension=".txt", filetypes=[("Text files", "*.txt")])

        if not file_path:
            return

        # Открываем файл для записи отчета
        with open(file_path, 'w', encoding='utf-8') as receipt_file:
            receipt_file.write("Отчет о результате анализа\n")
            receipt_file.write("============================\n\n")

            # Запрос для получения данных об образце
            self.cursor.execute('''
                SELECT Образец.ОбразецID, Образец.Тип_образца, Образец.Дата_поступления, 
                       Образец.Пациент_ФИО, Анализ.Наименование_анализа, Лаборант.ФИО, Врач.ФИО, 
                       Результат.дата_проведения, Результат.результат_данные, Анализ.Стоимость
                FROM Результат
                JOIN Образец ON Результат.id_образца = Образец.ОбразецID
                JOIN Анализ ON Результат.id_анализа = Анализ.АнализID
                JOIN Лаборант ON Результат.id_лаборанта = Лаборант.ЛаборантID
                JOIN Врач ON Результат.id_врача = Врач.ВрачID
                WHERE Результат.РезультатID = ?
            ''', (образец_id,))

            # Получаем данные о результатах
            receipt_data = self.cursor.fetchone()
            print(receipt_data)
            if receipt_data:
                receipt_file.write(f"ID образца: {receipt_data[0]}\n")
                receipt_file.write(f"Тип образца: {receipt_data[1]}\n")
                receipt_file.write(f"Дата поступления: {receipt_data[2]}\n")
                receipt_file.write(f"ФИО пациента: {receipt_data[3]}\n")
                receipt_file.write(f"Анализ: {receipt_data[4]}\n")
                receipt_file.write(f"Лаборант: {receipt_data[5]}\n")
                receipt_file.write(f"Врач: {receipt_data[6]}\n")
                receipt_file.write(f"Дата проведения анализа: {receipt_data[7]}\n")
                receipt_file.write(f"Результат: {receipt_data[8]}\n")
                receipt_file.write("============================\n")
                receipt_file.write(f"Стоимость услуги: {receipt_data[9]}\n")
                receipt_file.write("============================\n")
            else:
                receipt_file.write("Информация о результате не найдена.\n")

        # Показываем сообщение об успешном создании отчета
        messagebox.showinfo("Генерация отчета", "Отчет успешно создан.")

    def update_default_values(self):
        def get_next_seq(table_name):
            """Возвращает следующий номер последовательности для таблицы, или 1, если таблица пуста."""
            result = self.cursor.execute(f"SELECT seq FROM sqlite_sequence WHERE name = ?", (table_name,)).fetchone()
            return (result[0] if result else 0) + 1

        self.default_values = {
            'ОбразецID': lambda: get_next_seq('Образец'),
            'АнализID': lambda: get_next_seq('Анализ'),
            'ЛаборантID': lambda: get_next_seq('Лаборант'),
            'ВрачID': lambda: get_next_seq('Врач'),
            'РезультатID': lambda: get_next_seq('Результат'),
            'дата_проведения': lambda: datetime.datetime.now().strftime("%d-%m-%Y"),
            'Дата_поступления': lambda: datetime.datetime.now().strftime("%d-%m-%Y")
        }

    def get_analysis_results(self):
        query = '''
            SELECT 
                Образец.ОбразецID, 
                Образец.Тип_образца, 
                Образец.Дата_поступления, 
                Анализ.Наименование_анализа, 
                Лаборант.ФИО, 
                Врач.ФИО, 
                Образец.Пациент_ФИО,
                Результат.дата_проведения, 
                Результат.результат_данные, 
                Анализ.Стоимость
            FROM Результат
            JOIN Образец ON Результат.id_образца = Образец.ОбразецID
            JOIN Анализ ON Результат.id_анализа = Анализ.АнализID
            JOIN Лаборант ON Результат.id_лаборанта = Лаборант.ЛаборантID
            JOIN Врач ON Результат.id_врача = Врач.ВрачID
        '''
        self.cursor.execute(query)
        results = self.cursor.fetchall()

        return results

    def show_all_records_as_table(self, table_name):
        # Получить столбцы таблицы
        self.cursor.execute(f"PRAGMA table_info({table_name});")
        columns = [row[1] for row in self.cursor.fetchall()]

        # Получить данные таблицы
        self.cursor.execute(f"SELECT * FROM {table_name};")
        records = self.cursor.fetchall()

        # Создать новое окно
        table_window = tk.Toplevel(self.master)
        table_window.title(f"Отчет: {table_name}")

        # Создать Treeview для отображения данных
        tree = ttk.Treeview(table_window, columns=("Column", "Value"), show="headings")
        tree.pack(expand=True, fill="both")

        # Настроить столбцы
        tree.heading("Column", text="Название столбца")
        tree.heading("Value", text="Элемент")
        tree.column("Column", width=200, anchor="w")
        tree.column("Value", width=400, anchor="w")

        # Заполнить таблицу
        for record in records:
            for col, val in zip(columns, record):
                tree.insert("", "end", values=(col, val))

        # Добавить кнопку для закрытия окна
        close_button = tk.Button(table_window, text="Закрыть", command=table_window.destroy)
        close_button.pack(pady=10)

    def display_results(self):
        # Получаем результаты анализа
        results = self.get_analysis_results()

        # Создаем окно tkinter
        root = tk.Tk()
        root.title("Результаты анализов")

        # Создаем таблицу для отображения результатов
        columns = ["ID", "Тип образца", "Дата поступления", "Анализ", "Лаборант", "Врач", "Пациент", "Дата проведения",
                   "Результат", "Стоимость"]
        tree = ttk.Treeview(root, columns=columns, show="headings")

        # Настроим заголовки
        for col in columns:
            tree.heading(col, text=col)

        # Заполняем таблицу данными
        for row in results:
            tree.insert("", "end", values=row)

        # Адаптируем ширину колонок под содержимое
        for col in columns:
            max_width = max(len(col), max(len(str(item)) for item in [row[columns.index(col)] for row in results]))
            tree.column(col, width=max_width*8)

        # Располагаем таблицу на экране
        tree.pack(expand=True, fill="both")

        # Запускаем главный цикл tkinter
        root.mainloop()

    def view_record_by_record(self, table_name):
        # Окно для просмотра записей
        record_window = tk.Toplevel(self.master)
        record_window.title(f"Запись за записью: {table_name}")

        # Получение всех записей из таблицы
        self.cursor.execute(f"SELECT * FROM {table_name}")
        records = self.cursor.fetchall()
        columns = [desc[0] for desc in self.cursor.description]

        # Индекс текущей записи
        current_index = [0]

        # Обновление отображения текущей записи
        def update_view():
            if records:
                record = records[current_index[0]]
                for i, col in enumerate(columns):
                    labels[i]['text'] = f"{col}: {record[i]}"
            else:
                messagebox.showinfo("Информация", "Нет записей для отображения.")

        # Переход к следующей записи
        def next_record():
            if current_index[0] < len(records) - 1:
                current_index[0] += 1
                update_view()

        # Переход к предыдущей записи
        def previous_record():
            if current_index[0] > 0:
                current_index[0] -= 1
                update_view()

        # Отображение заголовков и значений
        labels = []
        for i, col in enumerate(columns):
            label = tk.Label(record_window, text="")
            label.pack(anchor="w", padx=10, pady=2)
            labels.append(label)

        # Кнопки навигации
        nav_frame = tk.Frame(record_window)
        nav_frame.pack(pady=10)

        prev_button = tk.Button(nav_frame, text="Назад", command=previous_record)
        prev_button.grid(row=0, column=0, padx=5)

        next_button = tk.Button(nav_frame, text="Вперед", command=next_record)
        next_button.grid(row=0, column=1, padx=5)

        # Закрытие окна
        close_button = tk.Button(record_window, text="Закрыть", command=record_window.destroy)
        close_button.pack(pady=10)

        # Инициализация просмотра
        update_view()


if __name__ == "__main__":
    connection_params = {"database": "mydb.sqlite3"}
    try:
        create_tables()
        root = ThemedTk(theme="winxpblue")
        app = DatabaseApp(root, connection_params)
        root.mainloop()

    except sqlite3.Error as err:
        messagebox.showerror("Ошибка базы данных", f"Произошла ошибка: {err}")
        print(f"Error: {err}")