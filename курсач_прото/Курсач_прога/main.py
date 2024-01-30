from kivy.app import App
from kivy.uix.button import Button
from kivy.uix.boxlayout import BoxLayout
from kivy.uix.pagelayout import PageLayout
from kivy.uix.anchorlayout import AnchorLayout
from kivy.properties import ObjectProperty
from kivy.config import Config
from kivy.uix.screenmanager import Screen, ScreenManager
from kivy.core.window import Window


class MenuScreen(Screen):
    pass


class StoryScreen(Screen):
    pass


class AdditionScreen(Screen):
    pass



class MainApp(App):
    def build(self):
        screen_manager = ScreenManager()
        screen_manager.add_widget(MenuScreen(name="menu_screen"))
        screen_manager.add_widget(StoryScreen(name="story_screen"))
        screen_manager.add_widget(AdditionScreen(name="add_screen"))
        return screen_manager


if __name__ == '__main__':
    MainApp().run()
