import numpy as np
import skfuzzy as fuzz
import matplotlib.pyplot as plt

# Базовое множество
x = np.arange(0, 101, 1)  # Диапазон от 0 до 100

# Функции принадлежности для каждого правила
rule1 = np.zeros_like(x)  # Правило 1: 0 от 0 до 100
rule2 = np.zeros_like(x)  # Правило 2: 0 от 0 до 100
rule3 = np.zeros_like(x)  # Правило 3: 0 от 0 до 100
rule4 = fuzz.trapmf(x, [60, 80, 100, 100])  # Правило 4: 0 от 0 до 60, затем возрастает до 1 от 60 до 80, и остается 1 до 100

# Отображение отдельных графиков
plt.figure(figsize=(6, 4))
plt.plot(x, rule1, 'b', label="Правило 1")
plt.title("Правило 1")
plt.xlabel("x")
plt.ylabel("Принадлежность")
plt.xticks(np.arange(0, 101, 10))
plt.yticks(np.arange(0, 1.1, 0.2))
plt.ylim([-0.05, 1.05])
plt.legend()
plt.show()

plt.figure(figsize=(6, 4))
plt.plot(x, rule2, 'b', label="Правило 2")
plt.title("Правило 2")
plt.xlabel("x")
plt.ylabel("Принадлежность")
plt.xticks(np.arange(0, 101, 10))
plt.yticks(np.arange(0, 1.1, 0.2))
plt.ylim([-0.05, 1.05])
plt.legend()
plt.show()

plt.figure(figsize=(6, 4))
plt.plot(x, rule3, 'b', label="Правило 3")
plt.title("Правило 3")
plt.xlabel("x")
plt.ylabel("Принадлежность")
plt.xticks(np.arange(0, 101, 10))
plt.yticks(np.arange(0, 1.1, 0.2))
plt.ylim([-0.05, 1.05])
plt.legend()
plt.show()

plt.figure(figsize=(6, 4))
plt.plot(x, rule4, 'b', label="Правило 4")
plt.title("Правило 4")
plt.xlabel("x")
plt.ylabel("Принадлежность")
plt.xticks(np.arange(0, 101, 10))
plt.yticks(np.arange(0, 1.1, 0.2))
plt.ylim([-0.05, 1.05])
plt.legend()
plt.show()

# Объединенный график
plt.figure(figsize=(10, 6))
plt.plot(x, rule1, 'r', label="Правило 1", linewidth=2)
plt.plot(x, rule2, 'g', label="Правило 2", linewidth=2)
plt.plot(x, rule3, 'b', label="Правило 3", linewidth=2)
plt.plot(x, rule4, 'm', label="Правило 4", linewidth=2)
plt.title("Объединенный график всех правил")
plt.xlabel("x")
plt.ylabel("Принадлежность")
plt.xticks(np.arange(0, 101, 10))
plt.yticks(np.arange(0, 1.1, 0.2))
plt.ylim([-0.05, 1.05])
plt.legend(loc="upper left")  # Легенда в правом верхнем углу
plt.grid(True)  # Добавляем сетку для наглядности
plt.show()