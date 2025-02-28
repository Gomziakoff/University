import sys
import os
import pandas as pd
import tempfile
import plotly.express as px
from PySide6.QtWidgets import (QApplication, QMainWindow, QVBoxLayout, QWidget,
                               QPushButton, QFileDialog, QComboBox, QLabel, QTableWidget,
                               QTableWidgetItem, QHBoxLayout, QStatusBar)
from PySide6.QtWebEngineWidgets import QWebEngineView
from PySide6.QtCore import QUrl


class DataVizApp(QMainWindow):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("Интерактивная визуализация данных")
        self.setGeometry(100, 100, 1000, 700)

        self.data = None  # Данные после загрузки
        self.temp_file = None  # Временный HTML-файл для графика

        # Центральный виджет
        central_widget = QWidget()
        self.setCentralWidget(central_widget)
        layout = QVBoxLayout(central_widget)

        # Кнопка загрузки файла
        self.btn_load = QPushButton("Загрузить CSV/Excel")
        self.btn_load.clicked.connect(self.load_data)
        layout.addWidget(self.btn_load)

        # Таблица данных
        self.table = QTableWidget()
        layout.addWidget(self.table)

        # Элементы управления графиком
        controls_layout = QHBoxLayout()
        self.combo_chart_type = QComboBox()
        self.combo_chart_type.addItems(["Линейный", "Гистограмма", "Диаграмма рассеяния", "Круговая"])
        self.combo_x = QComboBox()
        self.combo_y = QComboBox()
        self.btn_generate = QPushButton("Построить график")

        controls_layout.addWidget(QLabel("Тип графика:"))
        controls_layout.addWidget(self.combo_chart_type)
        controls_layout.addWidget(QLabel("X-ось:"))
        controls_layout.addWidget(self.combo_x)
        controls_layout.addWidget(QLabel("Y-ось:"))
        controls_layout.addWidget(self.combo_y)
        controls_layout.addWidget(self.btn_generate)
        layout.addLayout(controls_layout)

        self.btn_generate.clicked.connect(self.create_plot)

        # Веб-виджет для графика
        self.web_view = QWebEngineView()
        layout.addWidget(self.web_view)

        # Строка состояния
        self.status_bar = QStatusBar()
        self.setStatusBar(self.status_bar)

    def load_data(self):
        """Загружает CSV или Excel и отображает в таблице"""
        file_path, _ = QFileDialog.getOpenFileName(self, "Выберите файл", "", "CSV (*.csv);;Excel (*.xlsx)")
        if not file_path:
            return

        try:
            if file_path.endswith(".csv"):
                self.data = pd.read_csv(file_path)
            else:
                self.data = pd.read_excel(file_path)

            self.populate_table()
            self.combo_x.clear()
            self.combo_y.clear()
            self.combo_x.addItems(self.data.columns)
            self.combo_y.addItems(self.data.columns)
            self.status_bar.showMessage("Файл успешно загружен")
        except Exception as e:
            self.status_bar.showMessage(f"Ошибка загрузки: {e}")

    def populate_table(self):
        """Отображает данные в таблице"""
        self.table.setRowCount(self.data.shape[0])
        self.table.setColumnCount(self.data.shape[1])
        self.table.setHorizontalHeaderLabels(self.data.columns)

        for row in range(self.data.shape[0]):
            for col in range(self.data.shape[1]):
                self.table.setItem(row, col, QTableWidgetItem(str(self.data.iat[row, col])))

    def create_plot(self):
        """Генерирует график"""
        if self.data is None:
            self.status_bar.showMessage("Сначала загрузите данные")
            return

        x_col = self.combo_x.currentText()
        y_col = self.combo_y.currentText()
        chart_type = self.combo_chart_type.currentText()

        if not x_col or not y_col:
            self.status_bar.showMessage("Выберите столбцы для осей")
            return

        try:
            if chart_type == "Линейный":
                fig = px.line(self.data, x=x_col, y=y_col, title="Линейный график")
            elif chart_type == "Гистограмма":
                fig = px.bar(self.data, x=x_col, y=y_col, title="Гистограмма")
            elif chart_type == "Диаграмма рассеяния":
                fig = px.scatter(self.data, x=x_col, y=y_col, title="Диаграмма рассеяния")
            elif chart_type == "Круговая":
                fig = px.pie(self.data, names=x_col, values=y_col, title="Круговая диаграмма")

            self.display_plot(fig)
            self.status_bar.showMessage("График обновлён")
        except Exception as e:
            self.status_bar.showMessage(f"Ошибка построения графика: {e}")

    def display_plot(self, fig):
        """Сохраняет график и загружает его в WebEngineView"""
        if self.temp_file:
            os.remove(self.temp_file)  # Удаляем старый файл

        tmp_file = tempfile.NamedTemporaryFile(delete=False, suffix=".html")
        fig.write_html(tmp_file.name)
        tmp_file.close()
        self.temp_file = tmp_file.name

        # Исправляем путь для Windows
        file_path = os.path.abspath(self.temp_file).replace("\\", "/")
        file_url = QUrl(f"file:///{file_path}")
        print(f"График сохранён по пути: {file_url}")  # Вывод пути к файлу в логах

        self.web_view.setUrl(file_url)
        print(f"Размер QWebEngineView: {self.web_view.size()}")


if __name__ == "__main__":
    app = QApplication(sys.argv)
    window = DataVizApp()
    window.show()
    sys.exit(app.exec())