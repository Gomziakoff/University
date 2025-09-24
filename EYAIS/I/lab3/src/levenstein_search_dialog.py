from PySide6.QtWidgets import QDialog, QVBoxLayout, QLineEdit, QSpinBox, QTableWidget, QPushButton, QLabel, \
    QTableWidgetItem

from text_processor import TextProcessor


class LevenshteinSearchDialog(QDialog):
    def __init__(self, parent=None, words=None):
        super().__init__(parent)
        self.words = words
        self.setWindowTitle("Поиск по редакционному расстоянию")
        self.setGeometry(200, 200, 400, 300)

        layout = QVBoxLayout()

        self.target_word_edit = QLineEdit()
        self.max_distance_spin = QSpinBox()
        self.max_distance_spin.setRange(0, 10)
        self.max_distance_spin.setValue(2)

        self.results_table = QTableWidget()
        self.results_table.setColumnCount(2)
        self.results_table.setHorizontalHeaderLabels(["Слово", "Расстояние"])

        search_button = QPushButton("Найти")
        search_button.clicked.connect(self.perform_search)

        layout.addWidget(QLabel("Целевое слово:"))
        layout.addWidget(self.target_word_edit)
        layout.addWidget(QLabel("Максимальное расстояние:"))
        layout.addWidget(self.max_distance_spin)
        layout.addWidget(search_button)
        layout.addWidget(self.results_table)

        self.setLayout(layout)

    def perform_search(self):
        target = self.target_word_edit.text().lower()
        max_dist = self.max_distance_spin.value()

        self.results_table.setRowCount(0)

        if not target:
            return

        unique_words = list(set(self.words))  # Убираем дубликаты
        for word in unique_words:
            distance = TextProcessor.levenshtein_distance(target, word.lower())
            if distance <= max_dist:
                row = self.results_table.rowCount()
                self.results_table.insertRow(row)
                self.results_table.setItem(row, 0, QTableWidgetItem(word))
                self.results_table.setItem(row, 1, QTableWidgetItem(str(distance)))

        self.results_table.sortItems(1)  # Сортировка по расстоянию
        self.results_table.resizeColumnsToContents()
