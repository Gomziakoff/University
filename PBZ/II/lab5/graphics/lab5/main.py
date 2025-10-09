import numpy as np
import skfuzzy as fuzz
import matplotlib.pyplot as plt

# Базовые множества
time_of_day = np.arange(0, 25, 1)  # Время суток от 0 до 24 (часы)
people_count = np.arange(0, 501, 1)  # Количество людей от 0 до 500
power_supply = np.arange(0, 101, 1)  # Подача электроэнергии от 0 до 100 процентов

# Функции принадлежности для "Время суток" (трапецеидальные функции)
night = fuzz.trapmf(time_of_day, [0, 0, 3, 6])  # Ночь
morning = fuzz.trapmf(time_of_day, [4, 6, 10, 12])  # Утро
day = fuzz.trapmf(time_of_day, [8, 12, 18, 20])  # День
evening = fuzz.trapmf(time_of_day, [14, 18, 22, 24])  # Вечер

# Функции принадлежности для "Количество людей"
few_people = fuzz.trapmf(people_count, [0, 0, 50, 100])  # Мало людей
medium_people = fuzz.trapmf(people_count, [50, 150, 250, 350])  # Средне
many_people = fuzz.trapmf(people_count, [200, 300, 500, 500])  # Много людей

# Функции принадлежности для "Подача электроэнергии"
low_power = fuzz.trapmf(power_supply, [0, 0, 20, 30])  # Низкая подача
medium_power = fuzz.trapmf(power_supply, [20, 50, 80, 90])  # Средняя подача
high_power = fuzz.trapmf(power_supply, [60, 80, 100, 100])  # Высокая подача

# Отображение графиков по очереди

# Время суток
plt.figure(figsize=(6, 4))
plt.plot(time_of_day, night, 'b')
plt.title("Ночь")
plt.xlabel("Время суток (часы)")
plt.ylabel("Принадлежность")
plt.xticks(np.arange(0, 25, 2))
plt.yticks(np.arange(0, 1.1, 0.2))
plt.ylim([-0.05, 1.05])
plt.show()

plt.figure(figsize=(6, 4))
plt.plot(time_of_day, morning, 'b')
plt.title("Утро")
plt.xlabel("Время суток (часы)")
plt.ylabel("Принадлежность")
plt.xticks(np.arange(0, 25, 2))
plt.yticks(np.arange(0, 1.1, 0.2))
plt.ylim([-0.05, 1.05])
plt.show()

plt.figure(figsize=(6, 4))
plt.plot(time_of_day, day, 'b')
plt.title("День")
plt.xlabel("Время суток (часы)")
plt.ylabel("Принадлежность")
plt.xticks(np.arange(0, 25, 2))
plt.yticks(np.arange(0, 1.1, 0.2))
plt.ylim([-0.05, 1.05])
plt.show()

plt.figure(figsize=(6, 4))
plt.plot(time_of_day, evening, 'b')
plt.title("Вечер")
plt.xlabel("Время суток (часы)")
plt.ylabel("Принадлежность")
plt.xticks(np.arange(0, 25, 2))
plt.yticks(np.arange(0, 1.1, 0.2))
plt.ylim([-0.05, 1.05])
plt.show()

# Количество людей
plt.figure(figsize=(6, 4))
plt.plot(people_count, few_people, 'b')
plt.title("Мало людей")
plt.xlabel("Количество людей")
plt.ylabel("Принадлежность")
plt.xticks(np.arange(0, 501, 50))
plt.yticks(np.arange(0, 1.1, 0.2))
plt.ylim([-0.05, 1.05])
plt.show()

plt.figure(figsize=(6, 4))
plt.plot(people_count, medium_people, 'b')
plt.title("Среднее количество людей")
plt.xlabel("Количество людей")
plt.ylabel("Принадлежность")
plt.xticks(np.arange(0, 501, 50))
plt.yticks(np.arange(0, 1.1, 0.2))
plt.ylim([-0.05, 1.05])
plt.show()

plt.figure(figsize=(6, 4))
plt.plot(people_count, many_people, 'b')
plt.title("Много людей")
plt.xlabel("Количество людей")
plt.ylabel("Принадлежность")
plt.xticks(np.arange(0, 501, 50))
plt.yticks(np.arange(0, 1.1, 0.2))
plt.ylim([-0.05, 1.05])
plt.show()

# Подача электроэнергии
plt.figure(figsize=(6, 4))
plt.plot(power_supply, low_power, 'b')
plt.title("Низкая подача")
plt.xlabel("Подача электроэнергии (%)")
plt.ylabel("Принадлежность")
plt.xticks(np.arange(0, 101, 10))
plt.yticks(np.arange(0, 1.1, 0.2))
plt.ylim([-0.05, 1.05])
plt.show()

plt.figure(figsize=(6, 4))
plt.plot(power_supply, medium_power, 'b')
plt.title("Средняя подача")
plt.xlabel("Подача электроэнергии (%)")
plt.ylabel("Принадлежность")
plt.xticks(np.arange(0, 101, 10))
plt.yticks(np.arange(0, 1.1, 0.2))
plt.ylim([-0.05, 1.05])
plt.show()

plt.figure(figsize=(6, 4))
plt.plot(power_supply, high_power, 'b')
plt.title("Высокая подача")
plt.xlabel("Подача электроэнергии (%)")
plt.ylabel("Принадлежность")
plt.xticks(np.arange(0, 101, 10))
plt.yticks(np.arange(0, 1.1, 0.2))
plt.ylim([-0.05, 1.05])
plt.show()
