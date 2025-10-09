import numpy as np
import matplotlib.pyplot as plt
import random
import pandas as pd
import time


class NN:
    def __init__(self, alpha, n_samples, noise_ratio):
        self.noise_ratio = noise_ratio
        self.n_samples = n_samples // 2
        self.alpha = alpha
        self.W = np.array([0.1, 0.1])
        self.T = np.zeros(1)
        self.errors = []
        self.accuracy = 0

    def sigmoid(self, z):
        # Стабильная сигмоида для избежания переполнения
        return np.clip(1 / (1 + np.exp(-z)), 1e-7, 1 - 1e-7)

    def decision_boundary(self, x):
        return -2 * x + 12

    def create_dataset(self):
        X = []
        y = []
        for i in range(self.n_samples):
            x = random.randrange(-100, 100)
            y_coord = random.randrange(-300, self.decision_boundary(x))
            X.append([x, y_coord])
            y.append(0)
        for i in range(self.n_samples):
            x = random.randrange(-100, 100)
            y_coord = random.randrange(self.decision_boundary(x), 300)
            X.append([x, y_coord])
            y.append(1)
        n_noise_class0 = int(self.noise_ratio * self.n_samples)
        n_noise_class1 = int(self.noise_ratio * self.n_samples)
        for i in range(n_noise_class0):
            x = random.randrange(-100, 100)
            y_coord = self.decision_boundary(x) + random.randrange(0, 100)
            X[i] = [x, y_coord]
            y[i] = 1
        for i in range(n_noise_class1):
            x = random.randrange(-100, 100)
            y_coord = self.decision_boundary(x) - random.randrange(0, 100)
            X[self.n_samples + i] = [x, y_coord]
            y[self.n_samples + i] = 0
        return np.array(X), np.array(y)

    def plot_dataset(self, X, y):
        x_line = np.linspace(-100, 100, 400)
        y_line = self.decision_boundary(x_line)
        plt.figure(figsize=(10, 6))
        plt.scatter(X[y == 0][:, 0], X[y == 0][:, 1], color='blue', label='Class 0')
        plt.scatter(X[y == 1][:, 0], X[y == 1][:, 1], color='red', label='Class 1')
        plt.plot(x_line, y_line, color='black', linestyle='--', label='y = -2x + 12')
        plt.title('Генерация датасета с шумами и разделением по линии y = -2x + 12')
        plt.xlabel('X1')
        plt.ylabel('X2')
        plt.legend()
        plt.grid(True)
        plt.show()

    def cross_entropy_loss(self, x, e):
        y_hat = self.sigmoid(self.W @ x + self.T)
        return - (e * np.log(y_hat) + (1 - e) * np.log(1 - y_hat))

    def set_cross_entropy_loss(self, X, E):
        self.errors.append(np.mean([self.cross_entropy_loss(x, e) for x, e in zip(X, E)]))

    def predict(self, X):
        linear_output = self.W @ X + self.T
        return 1 if self.sigmoid(linear_output) >= 0.5 else 0

    def train(self, X, E):
        for x, e in zip(X, E):
            y_hat = self.sigmoid(self.W @ x + self.T)
            error = y_hat - e
            self.W -= self.alpha * error * y_hat * (1-y_hat) * x
            self.T -= self.alpha * error * y_hat * (1-y_hat)

    def adaptive_alpha(self, x):
        return 1 / (1 + sum(x ** 2))

    def adapt_train(self, X, E):
        for x, e in zip(X, E):
            y_hat = self.sigmoid(self.W @ x + self.T)
            error = y_hat - e
            self.W -= self.adaptive_alpha(x) *  error * y_hat * (1-y_hat) * x
            self.T -= self.adaptive_alpha(x) *  error * y_hat * (1-y_hat)

    def batch_train(self, X, E, batch_size):
        num_batches = len(X) // batch_size + (1 if len(X) % batch_size != 0 else 0)
        for batch_index in range(num_batches):
            W_update = np.zeros_like(self.W)
            T_update = np.zeros(1)
            for x, e in zip(X, E):
                y_hat = self.sigmoid(self.W @ x + self.T)
                error = y_hat - e
                W_update += error * y_hat * (1-y_hat) * x
                T_update += error * y_hat * (1-y_hat)
            self.W -= self.alpha * W_update
            self.T -= self.alpha * T_update

    def a(self, X, E, xk):
        # Precompute sigmoid outputs
        sigmoids = self.sigmoid(np.dot(X, self.W) + self.T)

        # Efficiently compute the dot products
        dot_products = np.dot(X, xk)

        # Compute 'a' using vectorized operations
        a_values = (sigmoids - E) * (1 + dot_products)

        return np.sum(a_values)

    def adaptive_batch_alpha(self, X, E):
        # Vectorized computation of sum_a and sum_b
        sigmoids = self.sigmoid(np.dot(X, self.W) + self.T)
        a_values = np.array([self.a(X, E, x) for x in X])  # Apply the optimized 'a' function

        # Calculate sum_a and sum_b using vectorized operations
        sum_a = np.sum((sigmoids - E) * a_values)
        sum_b = np.sum(a_values ** 2)

        # Return the adaptive learning rate
        return sum_a / sum_b

    def adapt_batch_train(self, X, E, batch_size):
        num_batches = len(X) // batch_size + (1 if len(X) % batch_size != 0 else 0)
        for batch_index in range(num_batches):
            W_update = np.zeros_like(self.W)
            T_update = np.zeros(1)
            for x, e in zip(X, E):
                y_hat = self.sigmoid(self.W @ x + self.T)
                error = y_hat - e
                W_update += error * y_hat * (1 - y_hat) * x
                T_update += error * y_hat * (1 - y_hat)
            self.W -= self.adaptive_batch_alpha(X, E) * W_update
            self.T -= self.adaptive_batch_alpha(X, E) * T_update

    def test(self, epoch, X, E):
        values = []
        right = 0
        error = np.mean([self.cross_entropy_loss(x, e) for x, e in zip(X, E)])
        for x, e in zip(X, E):
            if e == self.predict(x):
                right += 1
            values.append(self.predict(x))
        self.accuracy = right / (self.n_samples * 2) * 100
        print(f"Epoch: {epoch}: e={error}")
        return values


def collect_results(model_name, model, epochs, start_time):
    results = {
        "Model": model_name,
        "Test Error": model.errors[-1],  # Last error value
        "Epochs": epochs,
        "Training Time (s)": time.time() - start_time,  # Time in seconds
        "Right Answers": model.accuracy
    }
    return results


results_table = []
lin_model = NN(alpha=0.005, n_samples=100, noise_ratio=0.1)
lin_model_adapt = NN(alpha=0.005, n_samples=100, noise_ratio=0.1)
batch_model = NN(alpha=0.005, n_samples=100, noise_ratio=0.1)
batch_model_adapt = NN(alpha=0.005, n_samples=100, noise_ratio=0.1)

x, y = lin_model.create_dataset()

# Linear model
print("\nLinear model\n")
start_time = time.time()
last_epoch = -1
for epoch in range(1, 100):
    lin_model.train(x, y)
    lin_model.set_cross_entropy_loss(x, y)
    lin_model.test(epoch, x, y)
    if lin_model.errors[-1] <= 1e-5:
        last_epoch = epoch
        break
if last_epoch == -1:
    last_epoch = 99
print("Training results")
lin_values = lin_model.test(last_epoch, x, y)

# Collect results for the linear model
results_table.append(collect_results("Linear Model", lin_model, last_epoch, start_time))

# Linear model adaptive
print("\nLinear model adaptive\n")
start_time = time.time()
last_epoch = -1
for epoch in range(1, 100):
    lin_model_adapt.adapt_train(x, y)
    lin_model_adapt.set_cross_entropy_loss(x, y)
    lin_model_adapt.test(epoch, x, y)
    if lin_model_adapt.errors[-1] <= 1e-5:
        last_epoch = epoch
        break
if last_epoch == -1:
    last_epoch = 99
print("Training results")
lin_values_adapt = lin_model_adapt.test(last_epoch, x, y)

# Collect results for the linear model adaptive
results_table.append(collect_results("Linear Model Adaptive", lin_model_adapt, last_epoch, start_time))

# Batch model
print("\nBatch model\n")
start_time = time.time()
last_epoch = -1
for epoch in range(1, 100):
    batch_model.batch_train(x, y, batch_size=5)
    batch_model.set_cross_entropy_loss(x, y)
    batch_model.test(epoch, x, y)
    if batch_model.errors[-1] <= 1e-5:
        last_epoch = epoch
        break
if last_epoch == -1:
    last_epoch = 99
print("Training results")
batch_values = batch_model.test(last_epoch, x, y)

# Collect results for the batch model
results_table.append(collect_results("Batch Model", batch_model, last_epoch, start_time))

# Batch model adaptive
print("\nBatch model adapt\n")
start_time = time.time()
last_epoch = -1
for epoch in range(1, 100):
    batch_model_adapt.adapt_batch_train(x, y, batch_size=5)
    batch_model_adapt.set_cross_entropy_loss(x, y)
    batch_model_adapt.test(epoch, x, y)
    if batch_model_adapt.errors[-1] <= 1e-5:
        last_epoch = epoch
        break
if last_epoch == -1:
    last_epoch = 99
print("Training results")
batch_values_adapt = batch_model_adapt.test(last_epoch, x, y)

# Collect results for the batch model adaptive
results_table.append(collect_results("Batch Model Adaptive", batch_model_adapt, last_epoch, start_time))

# Display final table of results
df = pd.DataFrame(results_table)
print(df)
