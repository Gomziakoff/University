from PySide6.QtWidgets import (QApplication, QWidget, QVBoxLayout, QHBoxLayout,
                              QLabel, QLineEdit, QTextEdit, QPushButton)
from PySide6.QtCore import Qt, QSize
from PySide6.QtGui import QFont
from app.chatbot import ChatBot

class ChatWindow(QWidget):
    def __init__(self):
        super().__init__()
        self.chatbot = ChatBot()
        self.init_ui()
        self.set_styles()

    def init_ui(self):
        self.setWindowTitle('Музыка Беларуси')
        self.setMinimumSize(QSize(800, 600))

        main_layout = QVBoxLayout()
        main_layout.setContentsMargins(20, 20, 20, 20)
        main_layout.setSpacing(15)

        self.chat_history = QTextEdit()
        self.chat_history.setReadOnly(True)
        main_layout.addWidget(self.chat_history)

        input_layout = QHBoxLayout()
        self.user_input = QLineEdit()
        self.user_input.setPlaceholderText("Введите ваш вопрос о музыке Беларуси...")
        self.user_input.returnPressed.connect(self.process_input)
        input_layout.addWidget(self.user_input)

        self.send_btn = QPushButton("➤")
        self.send_btn.clicked.connect(self.process_input)
        input_layout.addWidget(self.send_btn)

        main_layout.addLayout(input_layout)
        self.setLayout(main_layout)

    def set_styles(self):
        self.setStyleSheet("""
            QWidget {
                background-color: #2b2b2b;
            }
            QTextEdit {
                background-color: #3c3f41;
                color: #bababa;
                border: 2px solid #4d4d4d;
                border-radius: 5px;
                font-size: 14px;
            }
            QLineEdit {
                background-color: #3c3f41;
                color: #bababa;
                border: 2px solid #4d4d4d;
                border-radius: 15px;
                padding: 8px 15px;
                font-size: 14px;
            }
            QPushButton {
                background-color: #4CAF50;
                color: white;
                border: none;
                border-radius: 15px;
                min-width: 40px;
                font-size: 16px;
            }
            QPushButton:hover {
                background-color: #45a049;
            }
        """)
        self.send_btn.setCursor(Qt.CursorShape.PointingHandCursor)

    def process_input(self):
        user_text = self.user_input.text().strip()
        if not user_text:
            return

        self.chat_history.append(f"<span style='color:#4CAF50'>Вы:</span> {user_text}")
        self.user_input.clear()

        response = self.chatbot.find_best_match(user_text)
        if not response:
            response = "Пока не могу ответить на этот вопрос. Попробуйте переформулировать."

        self.chat_history.append(f"<span style='color:#2196F3'>Бот:</span> {response}\n")