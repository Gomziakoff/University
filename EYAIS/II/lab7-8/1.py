import tkinter as tk
from tkinter import ttk
import speech_recognition as sr
import threading

class SpeechApp:
    def __init__(self, root):
        self.root = root
        self.root.title("Приложение распознавания речи")

        style = ttk.Style()
        style.theme_use("clam")
        style.configure("TButton", font=("Arial", 12), padding=6)
        style.configure("TLabel", font=("Arial", 12))
        style.configure("TCombobox", font=("Arial", 11))
        style.configure("TLabelframe.Label", font=("Arial", 13, "bold"))

        self.label = tk.Label(root, text="Нажмите кнопку и говорите:", font=("Arial", 16))
        self.label.pack(pady=20)

        self.result_text = tk.Text(root, height=10, width=50)
        self.result_text.pack(pady=20)

        self.language_var = tk.StringVar(value='ru-RU')  # По умолчанию русский
        self.language_label = tk.Label(root, text="Выберите язык:", font=("Arial", 12))
        self.language_label.pack(pady=5)

        self.language_combobox = ttk.Combobox(root, textvariable=self.language_var, values=[
            'ru-RU', 'en-US', 'de-DE', 'fr-FR'])
        self.language_combobox.pack(pady=5)

        self.recognize_button = tk.Button(root, text="Распознать речь", command=self.start_recognition_thread, font=("Arial", 12))
        self.recognize_button.pack(pady=20)

    def start_recognition_thread(self):
        threading.Thread(target=self.recognize_speech).start()

    def recognize_speech(self):
        recognizer = sr.Recognizer()
        recognizer.pause_threshold = 1
        with sr.Microphone() as source:
            self.label.config(text="Слушаю...")
            # self.root.update()
            #recognizer.adjust_for_ambient_noise(source, duration=1)
            try:
                audio = recognizer.listen(source, timeout=3, phrase_time_limit=12)
            except sr.WaitTimeoutError as e:
                self.label.config(text=f"Не удалось получить результаты; {e}.")
            try:
                language = self.language_var.get()
                text = recognizer.recognize_google(audio, language=language)
                print(text)
                self.result_text.delete(1.0, tk.END)
                self.result_text.insert(tk.END, text)
                self.label.config(text="Нажмите кнопку и говорите:")

                self.respond_to_command(text)

            except sr.UnknownValueError:
                self.label.config(text="Не удалось распознать аудио.")
            except sr.RequestError as e:
                self.label.config(text=f"Не удалось получить результаты; {e}.")
            except sr.WaitTimeoutError as e:
                self.label.config(text=f"Не удалось получить результаты; {e}.")
    def respond_to_command(self, text):
        if "привет" in text.lower():
            self.label.config(text="Привет! Как я могу помочь?")
        elif "стоп" in text.lower():
            self.label.config(text="А я что машина?")
        elif "как дела" in text.lower():
            self.label.config(text="У меня все хорошо, спасибо!")

if __name__ == '__main__':
    root = tk.Tk()
    app = SpeechApp(root)
    root.mainloop()