from kivy.app import App
from kivy.uix.screenmanager import ScreenManager, Screen
from kivy.uix.floatlayout import FloatLayout
from kivy.uix.button import Button
from kivy.uix.anchorlayout import AnchorLayout
from kivy.lang import Builder
from kivy.properties import NumericProperty, ObjectProperty, ListProperty
import time
from kivy.uix.filechooser import FileChooserIconView
from kivy.uix.modalview import ModalView
from kivy.uix.camera import Camera
from kivy.uix.modalview import ModalView
class MainWindow(Screen):
    Id = NumericProperty(0)
    transport_spiner = ObjectProperty()
    number_spiner = ObjectProperty()
    def get_id(self):
        return int(time.time())
    def show_file_dialog(self, *args):
        dialog = ModalView(size_hint=(0.9, 0.9))
        file_chooser = FileChooserIconView(filters=['*.png', '*.jpg', '*.jpeg', '*.gif', '*.bmp'])
        file_chooser.bind(on_submit=self.load_image)
        dialog.add_widget(file_chooser)
        close_button = Button(text="Отмена", size_hint_y=None, height=40)
        close_button.bind(on_press=dialog.dismiss)
        dialog.add_widget(close_button)
        dialog.open()
    def load_image(self, instance, selection, touch):
        if selection:
            self.ids['image_button'].source = selection[0]
            instance.parent.dismiss()

    def update_values(self):
        self.ids.number_spiner.text = "qwe"
        self.ids.number_spiner.values = ["1","2","3"]
        if self.ids.transport_spiner.text == "Автобус":
            self.ids.number_spiner.text = "Adl"




class StoryWindow(Screen):
    pass


class AddWindow(Screen):
    pass


class WindowManager(ScreenManager):
    pass


kv = Builder.load_file('main.kv')


class MainApp(App):
    def build(self):
        return kv


if __name__ == '__main__':
    MainApp().run()
