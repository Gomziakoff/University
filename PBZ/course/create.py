import sqlite3


def create_tables():
    connection_params = {"database": "mydb.sqlite3"}

    connect = sqlite3.connect(connection_params["database"])
    cursor = connect.cursor()

    cursor.execute('''
        CREATE TABLE IF NOT EXISTS Образец (
            ОбразецID INTEGER PRIMARY KEY AUTOINCREMENT,
            Дата_поступления Text,
            Тип_образца Text,
            Пациент_ФИО Text
        )
    ''')

    cursor.execute('''
        CREATE TABLE IF NOT EXISTS Анализ (
            АнализID INTEGER PRIMARY KEY AUTOINCREMENT,
            Наименование_анализа TEXT,
            Стоимость REAL
        )
    ''')

    cursor.execute('''
        CREATE TABLE IF NOT EXISTS Лаборант (
            ЛаборантID INTEGER PRIMARY KEY AUTOINCREMENT,
            ФИО TEXT,
            Специализация TEXT
        )
    ''')

    cursor.execute('''
                CREATE TABLE IF NOT EXISTS Врач  (
            ВрачID INTEGER PRIMARY KEY AUTOINCREMENT,
            ФИО TEXT NOT NULL,
            специальность TEXT
        );
    ''')

    cursor.execute('''
            CREATE TABLE IF NOT EXISTS Результат (
        РезультатID INTEGER PRIMARY KEY AUTOINCREMENT, 
        id_образца INTEGER NOT NULL,                    
        id_анализа INTEGER NOT NULL,                    
        id_лаборанта INTEGER NOT NULL,                   
        id_врача INTEGER NOT NULL,                      
        дата_проведения DATE NOT NULL,                  
        результат_данные TEXT NOT NULL,                 
        FOREIGN KEY (id_образца) REFERENCES Образец(ОбразецID),
        FOREIGN KEY (id_анализа) REFERENCES Анализ(АнализID),
        FOREIGN KEY (id_лаборанта) REFERENCES Лаборант(ЛаборантID),
        FOREIGN KEY (id_врача) REFERENCES Врач(ВрачID)
    );
    ''')

    connect.commit()
    connect.close()


create_tables()