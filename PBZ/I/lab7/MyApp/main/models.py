from django.db import models

class Товары(models.Model):
    numТовара = models.AutoField(primary_key=True)
    Наименование = models.CharField(max_length=255)
    Единица_измерения = models.CharField(max_length=255)
    Цена = models.IntegerField(default=0)
    def __str__(self):
        return str(self.numТовара)

class Накладные(models.Model):
    numНакладной = models.AutoField(primary_key=True)
    numТовара = models.IntegerField()
    Количество = models.IntegerField(default=0)
    Стоимость = models.IntegerField(default=0)
    def __str__(self):
        return str(self.numНакладной)

class Информация_о_накладных(models.Model):
    numНакладной = models.IntegerField()
    Ответственное_Лицо = models.CharField(max_length=255)
    Дата_Отгрузки = models.DateField()
    Средний_Балл = models.IntegerField(default=0)
    def __str__(self):
        return str(self.numНакладной)

