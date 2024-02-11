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
from kivy.uix.scrollview import ScrollView


class ScrollableBoxLayout(ScrollView):
    pass


class MainWindow(Screen):
    Id = NumericProperty(0)
    transport_spiner = ObjectProperty()
    number_spiner = ObjectProperty()
    def get_id(self):
        return (time.time())
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
        self.ids.number_spiner.values = ["1","2","3"]
        self.ids.number_spiner.text = "Выберите номер"
        if self.ids.transport_spiner.text == "Автобус":
            self.ids.number_spiner.values = ['1', '1А', '2', '2А', '3', '5', '6', '7', '8', '9', '10', '11', '11А', '12', '12А', '13', '13А', '14', '14Б', '14Ш', '15А', '15Б', '15В', '16', '17', '18', '19', '20', '21', '21А', '21Б', '22', '23', '23А', '23Б', '24', '25', '26', '27', '29', '30', '32', '33', '34', '35', '36', '37', '37А', '38', '39', '39А', '39Б', '41', '42', '44', '44А', '45', '46', '47', '48', '49', '77']
        elif self.ids.transport_spiner.text == "Троллейбус":
            self.ids.number_spiner.values = ['100', '101', '101А', '102', '103', '104', '105', '106', '106А', '107', '108', '108А', '110']
        elif self.ids.transport_spiner.text == "Маршрутка":
            self.ids.number_spiner.values = ['№1 (ОАО «Агротранс» – Центральная городская больница)', '№2 («Бернады – Магазин «Вам Рад»»)', '№4 (Автовокзал – Центральная городская больница)', '№5 («Новые-Козловичи» – Центральная городская больница)', '№6 (ВамРад – д.Косичи)', '№7 (д.Черни  –  рынок Варшавский)', '№9 (ОАО«Агротранс» – ул. Ленинградская)', '№10 («Брест-Западный» – Центральный ж/д вокзал)', '№12 (ул.Адамковская – гост. Дружба)', '№15   (Санта Бремор – ЦГБ)', '№17  (ППВ - ул.Радиальная)', '№18 (Центральный ж/д вокзал – Центральная городская больница)', '№19 (Обл. больница  – ул. 8-я Вересковая)', '№20 (Областная больница  – д. Тюхиничи)', '№21 (д.Вистичи – ЦГБ)', '№23 (Детская поликлиника №1  – Каменица-Жировецкая)', '№25 («Санта Бремор» – Строительный рынок)', '№35 («Санта Бремор» – ул. Филатова)']



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
