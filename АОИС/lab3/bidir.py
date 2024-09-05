import numpy as np
import random
import matplotlib.pyplot as plt
from typing import List


class Bidirectional:
    def __init__(self, vectors: List, n: int, m: int):
        self.n, self.m = n, m
        self.x = np.atleast_2d([vectors[i][:n] for i in range(len(vectors))])
        self.x = np.where(self.x <= 0, -1, self.x)
        self.y = np.atleast_2d(
            [vectors[i][len(vectors[0]) - m:] for i in range(len(vectors))]
        )
        self.y = np.where(self.y <= 0, -1, self.y)
        self.w = np.sum(
            [
                np.atleast_2d(self.x[i]).T @ np.atleast_2d(self.y[i])
                for i in range(len(vectors))
            ],
            axis=0,
        )

    def next_y(self, x):
        ans = np.where(x @ self.w < 0, -1, x @ self.w)
        ans = np.where(ans >= 1, 1, ans)
        return ans

    def next_x(self, y):
        ans = np.where(y @ self.w.T < 0, -1, y @ self.w.T)
        ans = np.where(ans >= 1, 1, ans)
        return ans

    def calc(self, vector: List):
        if len(vector) == self.n:
            prev_x = vector
            prev_x = np.where(np.atleast_2d(prev_x) <= 0, -1, prev_x)
            prev_y = self.next_y(prev_x)
            for _ in range(100):
                y = np.where(self.next_y(prev_x) == 0, prev_y, self.next_y(prev_x))
                x = np.where(self.next_x(y) == 0, prev_x, self.next_x(y))
                if np.allclose(y,prev_y,atol=0):
                    return np.where(y <= 0,0,y)
                prev_x = x
                prev_y = y
        else:
            prev_y = vector
            prev_y = np.where(np.atleast_2d(prev_y) <= 0, -1, prev_y)
            prev_x = self.next_x(prev_y)
            for _ in range(100):
                x = np.where(self.next_x(prev_y) == 0, prev_x, self.next_x(prev_y))
                y = np.where(self.next_y(x) == 0, prev_y, self.next_y(x))
                if np.allclose(x,prev_x,atol=0):
                    return np.where(x <= 0,0,x)
                prev_x = x
                prev_y = y
        return


def test_model(model, input_vector, num_bits_changed):
    input_vector_copy = input_vector.copy()
    n = len(input_vector_copy)

    # Генерация индексов для изменения случайных битов
    indices = random.sample(range(n), num_bits_changed)

    # Инвертирование выбранных битов
    for index in indices:
        input_vector_copy[index] = 1 - input_vector_copy[index]

    # Предсказание с измененным вектором
    predicted_result = model.calc(input_vector_copy)

    return predicted_result

def num_predicts(model,input_vector,max_bits_to_change):
    num_correct_predictions = []
    for num_bits_to_change in range(1, max_bits_to_change + 1):
        predicted_result = test_model(model, input_vector, num_bits_to_change)
        expected_result = model.calc(input_vector)
        num_correct = np.sum(predicted_result == expected_result)
        num_correct_predictions.append(num_correct)
    return num_correct_predictions

if __name__ == "__main__":
    Y1 = [0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0]
    Y2 = [1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0]
    Y3 = [0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 0, 1, 0]

    model = Bidirectional([Y1, Y2, Y3], 18, 4)

    print("1.Ручной ввод")
    print("2.Графичек")
    while(True):
        choose = int(input("enter choose"))
        if choose == 1:
            input_vector = input("Введите 18 или 4 значений через пробел: ").split()
            input_vector = [int(x) for x in input_vector]
            while len(input_vector) < 4 or len(input_vector) < 18:
                if len(input_vector) < 4:
                    remaining_values = 4 - len(input_vector)
                    additional_input = input(
                        f"Введите еще {remaining_values} {'значения' if remaining_values > 1 else 'значение'} через пробел: ").split()
                    input_vector.extend([int(x) for x in additional_input])
                elif len(input_vector) == 4:
                    break
                elif len(input_vector) < 18:
                    remaining_values = 18 - len(input_vector)
                    additional_input = input(
                        f"Введите еще {remaining_values} {'значения' if remaining_values > 1 else 'значение'} через пробел: ").split()
                    input_vector.extend([int(x) for x in additional_input])
                elif len(input_vector) > 18:
                    print("Вы ввели слишком много значений.")
                    input_vector = input("Введите 18 или 4 значений через пробел: ").split()
                    input_vector = [int(x) for x in input_vector]
            print(model.calc(input_vector))

        if choose == 2:
            input_vector1 = [0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0]
            input_vector2 = [1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1]
            input_vector3 = [0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1]
            max_bits_to_change = 18

            num_correct_predictions1 = num_predicts(model, input_vector1,max_bits_to_change)
            num_correct_predictions2 = num_predicts(model, input_vector2, max_bits_to_change)
            num_correct_predictions3 = num_predicts(model, input_vector3, max_bits_to_change)

            plt.plot(range(1, max_bits_to_change + 1), num_correct_predictions1, marker='o')
            plt.plot(range(1, max_bits_to_change + 1), num_correct_predictions2, marker='o')
            plt.plot(range(1, max_bits_to_change + 1), num_correct_predictions3, marker='o')
            plt.title('Number of Correct Predictions vs Number of Changed Bits')
            plt.xlabel('Number of Changed Bits')
            plt.ylabel('Number of Correct Predictions')
            plt.grid(True)
            plt.show()
