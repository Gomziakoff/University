from django.contrib import admin
from .models import Товары, Накладные, Информация_о_накладных

@admin.register(Товары)
class ТоварыAdmin(admin.ModelAdmin):
    search_fields = ['numТовара','Наименование']
    list_display = ['numТовара','Наименование','Единица_измерения','Цена']


@admin.register(Накладные)
class НакладныеAdmin(admin.ModelAdmin):
    search_fields = ['numНакладной', 'numТовара']
    list_display = ['numНакладной', 'numТовара', 'Количество', 'Стоимость']


@admin.register(Информация_о_накладных)
class Информация_о_накладныхAdmin(admin.ModelAdmin):
    search_fields = ['numНакладной', 'Ответственное_Лицо']
    list_display = ['numНакладной', 'Ответственное_Лицо', 'Дата_Отгрузки', 'Средний_Балл']
