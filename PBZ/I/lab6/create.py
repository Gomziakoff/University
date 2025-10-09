import sqlite3

def create_tables():
    connection_params = {"database": "mydb.sqlite3"}
    connect = sqlite3.connect(connection_params["database"])
    cursor = connect.cursor()

    cursor.execute('''CREATE TABLE Товары (
    [NumТовара] INT PRIMARY KEY,
    Наименование VARCHAR(255),
    Единица_Измерения VARCHAR(50),
    Цена DECIMAL(10, 2)
);''')
    cursor.execute('''INSERT INTO Товары ([NumТовара], Наименование, Единица_Измерения, Цена)
VALUES 
(1, 'Ноутбук', 'шт.', 50000.00),
(2, 'Монитор', 'шт.', 15000.00),
(3, 'Клавиатура', 'шт.', 2000.00);''')
    cursor.execute('''CREATE TABLE Накладные (
    [NumНакладной] INT PRIMARY KEY,
    [NumТовара] INT,
    Количество INT,
    Стоимость DECIMAL(10, 2),
    FOREIGN KEY ([NumТовара]) REFERENCES Товары([NumТовара])
);''')
    cursor.execute('''INSERT INTO Накладные ([NumНакладной], [NumТовара], Количество, Стоимость)
VALUES
(101, 1, 10, 500000.00),
(102, 2, 5, 75000.00),
(103, 3, 20, 40000.00);''')
    cursor.execute('''CREATE TABLE Информация_О_Накладных ( 
    [NumНакладной] INT,
    Ответственное_Лицо VARCHAR(255),
    Дата_Отгрузки DATE,
    Средний_Балл DECIMAL(3, 2),
    PRIMARY KEY ([NumНакладной]),
    FOREIGN KEY ([NumНакладной]) REFERENCES Накладные([NumНакладной])
);''')
    cursor.execute('''INSERT INTO Информация_О_Накладных ([NumНакладной], Ответственное_Лицо, Дата_Отгрузки, Средний_Балл)
VALUES
(101, 'Иванов И.И.', '2024-09-21', 4.5),
(102, 'Петров П.П.', '2024-09-22', 4.7),
(103, 'Сидоров С.С.', '2024-09-23', 4.6);''')

    connect.commit()
    cursor.close()
    connect.close()


if __name__ == "__main__":
    create_tables()
    print("Tables created and data inserted successfully.")