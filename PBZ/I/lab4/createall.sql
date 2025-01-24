-- Создание таблицы Товары
CREATE TABLE Товары (
    [NumТовара] INT PRIMARY KEY,
    Наименование VARCHAR(255),
    Единица_Измерения VARCHAR(50),
    Цена DECIMAL(10, 2)
);
GO

-- Вставка данных в таблицу Товары
INSERT INTO Товары ([NumТовара], Наименование, Единица_Измерения, Цена)
VALUES 
(1, 'Ноутбук', 'шт.', 50000.00),
(2, 'Монитор', 'шт.', 15000.00),
(3, 'Клавиатура', 'шт.', 2000.00);
GO

-- Процедура для добавления нового товара
CREATE PROCEDURE InsertNewProduct
    @NumТовара INT,
    @Наименование VARCHAR(255),
    @Единица_Измерения VARCHAR(50),
    @Цена DECIMAL(10, 2)
AS
BEGIN
    INSERT INTO Товары ([NumТовара], Наименование, Единица_Измерения, Цена)
    VALUES (@NumТовара, @Наименование, @Единица_Измерения, @Цена);
END;
GO

-- Функция для получения цены товара по его номеру
CREATE FUNCTION GetProductPrice(@NumТовара INT)
RETURNS DECIMAL(10, 2)
AS
BEGIN
    DECLARE @Цена DECIMAL(10, 2);
    SELECT @Цена = Цена FROM Товары WHERE [NumТовара] = @NumТовара;
    RETURN @Цена;
END;
GO

-- Триггер для проверки цены товара
CREATE TRIGGER PriceCheck
ON Товары
FOR INSERT
AS
BEGIN
    IF EXISTS (SELECT * FROM inserted WHERE Цена > 100000)
    BEGIN
        ROLLBACK TRANSACTION;
        RAISERROR('Цена не может превышать 100,000!', 16, 1);
    END
END;
GO

-- Создание таблицы Накладные
CREATE TABLE Накладные (
    [NumНакладной] INT PRIMARY KEY,
    [NumТовара] INT,
    Количество INT,
    Стоимость DECIMAL(10, 2),
    FOREIGN KEY ([NumТовара]) REFERENCES Товары([NumТовара])
);
GO

-- Вставка данных в таблицу Накладные
INSERT INTO Накладные ([NumНакладной], [NumТовара], Количество, Стоимость)
VALUES
(101, 1, 10, 500000.00),
(102, 2, 5, 75000.00),
(103, 3, 20, 40000.00);
GO

-- Процедура для вставки новой накладной
CREATE PROCEDURE InsertInvoice
    @NumНакладной INT,
    @NumТовара INT,
    @Количество INT,
    @Стоимость DECIMAL(10, 2)
AS
BEGIN
    INSERT INTO Накладные ([NumНакладной], [NumТовара], Количество, Стоимость)
    VALUES (@NumНакладной, @NumТовара, @Количество, @Стоимость);
END;
GO

-- Функция для получения общей стоимости накладной
CREATE FUNCTION GetInvoiceTotal(@NumНакладной INT)
RETURNS DECIMAL(10, 2)
AS
BEGIN
    DECLARE @Total DECIMAL(10, 2);
    SELECT @Total = SUM(Стоимость) FROM Накладные WHERE [NumНакладной] = @NumНакладной;
    RETURN @Total;
END;
GO

-- Триггер для проверки количества в накладной
CREATE TRIGGER QuantityCheck
ON Накладные
FOR INSERT, UPDATE
AS
BEGIN
    IF EXISTS (SELECT * FROM inserted WHERE Количество < 1)
    BEGIN
        ROLLBACK TRANSACTION;
        RAISERROR('Количество не может быть меньше 1!', 16, 1);
    END
END;
GO

-- Создание таблицы Информация_О_Накладных
CREATE TABLE Информация_О_Накладных ( 
    [NumНакладной] INT,
    Ответственное_Лицо VARCHAR(255),
    Дата_Отгрузки DATE,
    Средний_Балл DECIMAL(3, 2),
    PRIMARY KEY ([NumНакладной]),
    FOREIGN KEY ([NumНакладной]) REFERENCES Накладные([NumНакладной])
);
GO

-- Вставка данных в таблицу Информация_О_Накладных
INSERT INTO Информация_О_Накладных ([NumНакладной], Ответственное_Лицо, Дата_Отгрузки, Средний_Балл)
VALUES
(101, 'Иванов И.И.', '2024-09-21', 4.5),
(102, 'Петров П.П.', '2024-09-22', 4.7),
(103, 'Сидоров С.С.', '2024-09-23', 4.6);
GO

-- Процедура для обновления среднего балла
CREATE PROCEDURE UpdateAvgScore
    @NumНакладной INT,
    @Средний_Балл DECIMAL(3, 2)
AS
BEGIN
    UPDATE Информация_О_Накладных
    SET Средний_Балл = @Средний_Балл
    WHERE [NumНакладной] = @NumНакладной;
END;
GO

-- Функция для получения ответственного лица по номеру накладной
CREATE FUNCTION GetResponsiblePerson(@NumНакладной INT)
RETURNS VARCHAR(255)
AS
BEGIN
    DECLARE @Ответственное_Лицо VARCHAR(255);
    SELECT @Ответственное_Лицо = Ответственное_Лицо FROM Информация_О_Накладных WHERE [NumНакладной] = @NumНакладной;
    RETURN @Ответственное_Лицо;
END;
GO

-- Триггер для логирования изменения среднего балла
CREATE TRIGGER LogAvgScoreChange
ON Информация_О_Накладных
AFTER UPDATE
AS
BEGIN
    IF UPDATE(Средний_Балл)
    BEGIN
        DECLARE @NumНакладной INT, @OldScore DECIMAL(3, 2), @NewScore DECIMAL(3, 2);
        SELECT @NumНакладной = inserted.[NumНакладной], @NewScore = inserted.Средний_Балл, @OldScore = deleted.Средний_Балл
        FROM inserted
        JOIN deleted ON inserted.[NumНакладной] = deleted.[NumНакладной];
        PRINT 'Средний балл для накладной ' + CAST(@NumНакладной AS VARCHAR) + ' изменен с ' + CAST(@OldScore AS VARCHAR) + ' на ' + CAST(@NewScore AS VARCHAR);
    END
END;
GO
