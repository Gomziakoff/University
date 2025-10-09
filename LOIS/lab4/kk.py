import numpy as np

# Задаём диапазон значений для входных переменных
x1 = np.random.uniform(-10, 10, 1000)  # 100 случайных значений для x1
x2 = np.random.uniform(-10, 10, 1000)  # 100 случайных значений для x2
x3 = np.random.uniform(-10, 10, 1000)  # 100 случайных значений для x3

# Рассчитываем y по формуле
y = x1**2 + x2 - x3

# Объединяем данные в одном массиве
data = np.column_stack((x1, x2, x3, y))

# Сохраняем данные в файл .dat
np.savetxt('training_data.dat', data, delimiter='\t', fmt='%.6f')
