import datetime
import time

import winsound
from astral import LocationInfo
from astral.sun import sun
from pytz import timezone


class SimulatedTime:
    def __init__(self, initial_time):
        self.current_time = initial_time

    def get_current_time(self):
        return self.current_time

    def advance_time(self, seconds):
        self.current_time += datetime.timedelta(seconds=seconds)

    def set_time(self, new_time):
        self.current_time = new_time


# Настройка местоположения для расчета времени сумерек
city_name = "Брест"
latitude = 55.7558
longitude = 37.6176
timezone_str = "Europe/Moscow"
location = LocationInfo(city_name, "Belarus", timezone_str, latitude, longitude)


def get_time_of_dusk(simulated_time):
    """Возвращает время сумерек для текущей даты."""
    current_time = simulated_time.get_current_time()
    s = sun(location.observer, date=current_time.date())
    return s["dusk"].astimezone(timezone(timezone_str))


def format_time(dt):
    """Форматирует дату и время в красивый строковый формат."""
    return dt.strftime("%d %B %H:%M")

class WateringSystem:
    def __init__(self, simulated_time):
        self.simulated_time = simulated_time
        self.last_watering_time_type3 = simulated_time.get_current_time()

    def water_plants_type1(self):
        """Полив для растений типа 1 (водяные растения)."""
        print(f"[{format_time(self.simulated_time.get_current_time())}] Полив для растений типа 1: "
              "Поплавковый выключатель регулирует уровень воды в бассейне.")

    def water_plants_type2(self):
        """Полив для растений типа 2 (полив в 06:00 и 20:00)."""
        print(f"[{format_time(self.simulated_time.get_current_time())}] Полив для растений типа 2: "
              "Полив в течение 3 минут.")

    def water_plants_type3(self):
        """Полив для растений типа 3 (полив каждые 2 дня вечером)."""
        print(f"[{format_time(self.simulated_time.get_current_time())}] Полив для растений типа 3: "
              "Полив в течение 2 минут.")
        self.last_watering_time_type3 = self.simulated_time.get_current_time()

def beep():
    winsound.Beep(1000, 1000)
    print("*Сработал датчик сумерек*")
    print("*Звуковой сигнал*")

def simulate_one_day(simulated_time, watering_system):
    """Симуляция одного дня (24 часа) с выводом логов."""
    end_time = simulated_time.get_current_time() + datetime.timedelta(days=333)

    while simulated_time.get_current_time() < end_time:
        current_time = simulated_time.get_current_time()

        # Полив для растений типа 1 (постоянный контроль уровня воды)
        watering_system.water_plants_type1()

        # Полив для растений типа 2 (в 06:00 и 20:00)
        if current_time.hour == 6 and current_time.minute == 0:
            watering_system.water_plants_type2()
        if current_time.hour == 20 and current_time.minute == 0:
            watering_system.water_plants_type2()

        # Полив для растений типа 3 (каждые 2 дня вечером после сумерек)
        dusk_time = get_time_of_dusk(simulated_time)

        if current_time.hour == dusk_time.hour and (current_time - watering_system.last_watering_time_type3).days >= 2:
            beep()
            watering_system.water_plants_type3()
        elif current_time.hour == dusk_time.hour:
            beep()

        # Увеличиваем время на 1 час
        simulated_time.advance_time(3600)
        print("_" * 50)
        time.sleep(3)

beep()
beep()
# Инициализация симулированного времени
initial_time = datetime.datetime.now(timezone(timezone_str)).replace(hour=0, minute=0, second=0, microsecond=0)
simulated_time = SimulatedTime(initial_time)

# Инициализация системы полива
watering_system = WateringSystem(simulated_time)

# Запуск симуляции одного дня
print("Начало симуляции одного дня:")
simulate_one_day(simulated_time, watering_system)
print("Симуляция завершена.")