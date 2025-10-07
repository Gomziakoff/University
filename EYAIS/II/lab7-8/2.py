import tkinter as tk
from tkinter import ttk
import pyttsx3


class TTSApp:
    def __init__(self, root):
        self.root = root
        self.root.title("🎙 Система синтеза речи")
        self.root.geometry("600x550")
        self.root.resizable(False, False)

        style = ttk.Style()
        style.theme_use("clam")
        style.configure("TButton", font=("Arial", 12), padding=6)
        style.configure("TLabel", font=("Arial", 12))
        style.configure("TCombobox", font=("Arial", 11))
        style.configure("TLabelframe.Label", font=("Arial", 13, "bold"))

        engine = pyttsx3.init()
        self.voices = engine.getProperty("voices")
        self.voice_map = {v.name: v.id for v in self.voices}

        text_frame = ttk.LabelFrame(root, text=" Текст для озвучивания ")
        text_frame.pack(fill="both", padx=15, pady=10)

        self.text_entry = tk.Text(text_frame, height=8, wrap="word", font=("Arial", 12))
        self.text_entry.pack(fill="both", padx=10, pady=10)

        settings_frame = ttk.LabelFrame(root, text=" Настройки ")
        settings_frame.pack(fill="x", padx=15, pady=10)

        ttk.Label(settings_frame, text="Голос:").grid(row=0, column=0, padx=5, pady=5, sticky="w")
        self.voice_var = tk.StringVar(value=list(self.voice_map.keys())[0])
        self.voice_combobox = ttk.Combobox(settings_frame, textvariable=self.voice_var,
                                           values=list(self.voice_map.keys()), width=40, state="readonly")
        self.voice_combobox.grid(row=0, column=1, padx=5, pady=5, sticky="ew")

        ttk.Label(settings_frame, text="Скорость:").grid(row=1, column=0, padx=5, pady=5, sticky="w")
        self.speed_scale = tk.Scale(settings_frame, from_=50, to=300, orient="horizontal", length=300)
        self.speed_scale.set(150)
        self.speed_scale.grid(row=1, column=1, padx=5, pady=5)

        ttk.Label(settings_frame, text="Громкость:").grid(row=2, column=0, padx=5, pady=5, sticky="w")
        self.volume_scale = tk.Scale(settings_frame, from_=0.0, to=1.0, resolution=0.1,
                                     orient="horizontal", length=300)
        self.volume_scale.set(1.0)
        self.volume_scale.grid(row=2, column=1, padx=5, pady=5)

        settings_frame.columnconfigure(1, weight=1)

        buttons_frame = ttk.Frame(root)
        buttons_frame.pack(pady=20)

        self.speak_button = ttk.Button(buttons_frame, text="▶ ПРОИЗНЕСТИ", command=self.speak_text)
        self.speak_button.grid(row=0, column=0, padx=10)

    def speak_text(self):
        text = self.text_entry.get("1.0", tk.END).strip()
        if not text:
            return
        self.speak(text)

    def speak(self, text):
        engine = pyttsx3.init()

        selected_name = self.voice_var.get()
        voice_id = self.voice_map.get(selected_name, list(self.voice_map.values())[0])

        engine.setProperty("voice", voice_id)
        engine.setProperty("rate", self.speed_scale.get())
        engine.setProperty("volume", self.volume_scale.get())

        engine.say(text)
        engine.runAndWait()


if __name__ == "__main__":
    root = tk.Tk()
    app = TTSApp(root)
    root.mainloop()
