# gui.py
import re
from PySide6.QtCore import Qt
from PySide6.QtWidgets import (QApplication, QDialog, QFileDialog, QHBoxLayout,
                               QLabel, QListWidget, QListWidgetItem, QMainWindow,
                               QMessageBox, QPushButton, QTableWidget,
                               QTableWidgetItem, QTextEdit, QVBoxLayout, QWidget)

from data_manager import DataManager
from text_processor import TextProcessor


class SentenceDetailWindow(QDialog):
    def __init__(self, sentence_data):
        super().__init__()
        self.setWindowTitle("Анализ предложения")
        self.setGeometry(200, 200, 800, 400)
        self.sentence_data = sentence_data
        self.init_ui()

    def init_ui(self):
        layout = QVBoxLayout()
        layout.addWidget(QLabel(f"Предложение: {self.sentence_data['sentence_text']}"))

        self.table = QTableWidget()
        self.table.setColumnCount(7)
        self.table.setHorizontalHeaderLabels([
            "Слово", "Лемма", "Часть речи", "Падеж", "Число", "Время", "Роль"
        ])

        self.table.setRowCount(len(self.sentence_data["words"]))
        for row, word in enumerate(self.sentence_data["words"]):
            self.table.setItem(row, 0, QTableWidgetItem(word["word"]))
            self.table.setItem(row, 1, QTableWidgetItem(word["lemma"]))
            self.table.setItem(row, 2, QTableWidgetItem(word["pos"]))
            self.table.setItem(row, 3, QTableWidgetItem(word["case"]))
            self.table.setItem(row, 4, QTableWidgetItem(word["number"]))
            self.table.setItem(row, 5, QTableWidgetItem(word["tense"]))
            self.table.setItem(row, 6, QTableWidgetItem(word["role"]))

        self.table.resizeColumnsToContents()
        layout.addWidget(self.table)
        self.setLayout(layout)


class MainWindow(QMainWindow):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("Анализатор текста")
        self.setGeometry(100, 100, 800, 600)
        self.text_processor = TextProcessor()
        self.original_data = []
        self.init_ui()

    def init_ui(self):
        main_layout = QVBoxLayout()

        self.text_edit = QTextEdit()
        self.text_edit.setPlaceholderText("Введите текст здесь...")
        main_layout.addWidget(self.text_edit)

        buttons_layout = QHBoxLayout()
        self.btn_process = QPushButton("Анализировать текст")
        self.btn_open = QPushButton("Открыть файл")
        self.btn_save = QPushButton("Сохранить анализ")
        self.btn_load = QPushButton("Загрузить анализ")  # Новая кнопка
        buttons_layout.addWidget(self.btn_open)
        buttons_layout.addWidget(self.btn_process)
        buttons_layout.addWidget(self.btn_save)
        buttons_layout.addWidget(self.btn_load)  # Добавляем кнопку в layout
        main_layout.addLayout(buttons_layout)

        search_layout = QHBoxLayout()
        self.search_edit = QTextEdit()
        self.search_edit.setMaximumHeight(30)
        self.btn_search = QPushButton("Поиск")
        search_layout.addWidget(self.search_edit)
        search_layout.addWidget(self.btn_search)
        main_layout.addLayout(search_layout)

        self.sentence_list = QListWidget()
        main_layout.addWidget(self.sentence_list)

        container = QWidget()
        container.setLayout(main_layout)
        self.setCentralWidget(container)

        self.btn_process.clicked.connect(self.process_text)
        self.btn_open.clicked.connect(self.open_file_dialog)
        self.btn_save.clicked.connect(self.save_results)
        self.btn_load.clicked.connect(self.load_results)  # Подключаем новую кнопку
        self.btn_search.clicked.connect(self.search_text)
        self.sentence_list.itemDoubleClicked.connect(self.show_sentence_detail)

    def process_text(self):
        text = self.text_edit.toPlainText()
        if text:
            try:
                self.original_data = self.text_processor.process_text(text)
                self.update_sentence_list(self.original_data)
            except Exception as e:
                QMessageBox.critical(self, "Ошибка", f"Ошибка обработки текста: {str(e)}")
        else:
            QMessageBox.warning(self, "Ошибка", "Введите текст для анализа")

    def update_sentence_list(self, data):
        self.sentence_list.clear()
        for sentence in data:
            item = QListWidgetItem(sentence["sentence_text"])
            item.setData(Qt.UserRole, sentence)
            self.sentence_list.addItem(item)

    def show_sentence_detail(self, item):
        sentence_data = item.data(Qt.UserRole)
        detail_window = SentenceDetailWindow(sentence_data)
        detail_window.exec()

    def search_text(self):
        keyword = self.search_edit.toPlainText().strip()
        if keyword:
            filtered = DataManager.search_data(self.original_data, keyword)
            self.update_sentence_list(filtered)
        else:
            self.update_sentence_list(self.original_data)

    def open_file_dialog(self):
        file_dialog = QFileDialog(self)
        file_dialog.setNameFilter("Текстовые файлы (*.txt *.rtf *.pdf *.doc *.docx)")
        if file_dialog.exec():
            file_paths = file_dialog.selectedFiles()
            if file_paths:
                self.load_file(file_paths[0])

    def load_file(self, file_path):
        try:
            text = self.read_file(file_path)
            self.text_edit.setText(text)
        except Exception as e:
            QMessageBox.critical(self, "Ошибка", f"Ошибка загрузки файла: {str(e)}")

    def read_file(self, file_path):
        if file_path.endswith(".txt"):
            with open(file_path, "r", encoding="utf-8") as f:
                return f.read()
        elif file_path.endswith(".pdf"):
            from PyPDF2 import PdfReader
            with open(file_path, "rb") as f:
                return "".join(page.extract_text() for page in PdfReader(f).pages)
        elif file_path.endswith(".docx"):
            from docx import Document
            return "\n".join(p.text for p in Document(file_path).paragraphs)
        elif file_path.endswith(".rtf"):
            from striprtf import rtf_to_text
            with open(file_path, "r", encoding="utf-8") as f:
                return rtf_to_text(f.read())
        else:
            raise ValueError("Неподдерживаемый формат файла")

    def save_results(self):
        filename, _ = QFileDialog.getSaveFileName(
            self, "Сохранить анализ", "", "JSON файлы (*.json)")
        if filename:
            try:
                DataManager.save_data(self.original_data, filename)
                QMessageBox.information(self, "Успех", "Данные успешно сохранены")
            except Exception as e:
                QMessageBox.critical(self, "Ошибка", f"Ошибка сохранения: {str(e)}")

    def load_results(self):
        """Загрузка сохраненного анализа из файла"""
        filename, _ = QFileDialog.getOpenFileName(
            self,
            "Загрузить сохраненный анализ",
            "",
            "JSON файлы (*.json)"
        )

        if filename:
            try:
                self.original_data = DataManager.load_data(filename)
                self.update_sentence_list(self.original_data)
                QMessageBox.information(
                    self,
                    "Успех",
                    "Анализ успешно загружен из файла!"
                )
            except Exception as e:
                QMessageBox.critical(
                    self,
                    "Ошибка",
                    f"Не удалось загрузить анализ:\n{str(e)}"
                )

if __name__ == "__main__":
    app = QApplication([])
    window = MainWindow()
    window.show()
    app.exec()