import numpy as np
import matplotlib.pyplot as plt
import pandas as pd
import time


class MLP:
    def __init__(self, input_size, hide_size, output_size, alpha):
        self.input_size = input_size
        self.output_size = output_size
        self.hide_size = hide_size
        self.Wh = np.random.uniform(size=(input_size, hide_size))
        self.Wo = np.random.uniform(size=(hide_size, output_size))
        self.Th = np.random.uniform(size=(1, hide_size))
        self.To = np.random.uniform(size=(1, output_size))
        self.alpha = alpha

    def tanh(self, x):
        return np.tanh(x)

    def tanh_derivative(self, x):
        return 1 - np.tanh(x) ** 2

    def create_dataset(self, noise_percentage=0.1):
        func = lambda a, b, c, d, x: a * np.cos(b * x) + c * np.sin(d * x)
        x = np.arange(0, 15, 0.1)
        y = func(0.1, 0.3, 0.08, 3, x)
        y = np.array([y[i - self.input_size: i] for i in range(self.input_size, len(y))])
        e = np.array([y[i + self.input_size][0] for i in range(len(y) - self.input_size)])

        # Calculate the number of noisy points based on the noise_percentage
        num_noisy_points = int(len(e) * (noise_percentage))
        noisy_indices = np.random.choice(len(e), num_noisy_points, replace=False)

        # Apply noise to the selected indices of e
        noise = np.random.uniform(-0.1, 0.1, size=num_noisy_points)  # Adjust the range of noise
        e[noisy_indices] += noise  # Add noise to the selected indices

        return y[:len(y) - self.input_size], e, x[:len(y) - self.input_size]

    def train(self, x, e, batch_size,mode="ba"):
        if mode == "ba":
            num_samples = x.shape[0]

            for start in range(0, num_samples, batch_size):
                end = start + batch_size
                x_batch = x[start:min(end, num_samples)]
                e_batch = e[start:min(end, num_samples)].reshape(-1, self.output_size)
                yi = np.dot(x_batch, self.Wh) + self.Th
                # Calculate the hidden layer output
                yh = self.tanh(np.dot(x_batch, self.Wh) + self.Th)
                # Output layer calculation
                yo = np.dot(yh, self.Wo) + self.To

                error = e_batch - yo
                deltao = error * self.tanh_derivative(yo)
                self.learning_rate_output = np.sum(error**2*self.tanh_derivative(yo)) / \
                                            (1 + np.sum(yh**2) * np.sum(error**2*self.tanh_derivative(yo)**2))
                hidden_error = deltao.dot(self.Wo.T)
                self.learning_rate_hidden = np.sum(hidden_error**2*self.tanh_derivative(yh)) / \
                                            (1 + (np.sum(yi**2)) * np.sum(hidden_error**2*self.tanh_derivative(yh)**2))
                self.learning_rate_hidden = self.alpha
                deltah = hidden_error * self.tanh_derivative(yh)

                self.Wo += yh.T.dot(deltao) * self.learning_rate_output / x_batch.shape[0]
                self.To += np.sum(deltao, axis=0, keepdims=True) * self.learning_rate_output / x_batch.shape[0]
                self.Wh += x_batch.T.dot(deltah) * self.learning_rate_hidden / x_batch.shape[0]
                self.Th += np.sum(deltah, axis=0, keepdims=True) * self.learning_rate_hidden / x_batch.shape[0]
        elif mode == "b":
            num_samples = x.shape[0]
            for start in range(0, num_samples, batch_size):
                end = start + batch_size
                x_batch = x[start:min(end, num_samples)]
                e_batch = e[start:min(end, num_samples)].reshape(-1, self.output_size)
                yh = self.tanh(np.dot(x_batch, self.Wh) + self.Th)
                # Output layer calculation
                yo = np.dot(yh, self.Wo) + self.To

                error = e_batch - yo
                deltao = error * self.tanh_derivative(yo)
                hidden_error = deltao.dot(self.Wo.T)
                deltah = hidden_error * self.tanh_derivative(yh)

                self.Wo += yh.T.dot(deltao) * self.alpha / x_batch.shape[0]
                self.To += np.sum(deltao, axis=0, keepdims=True) * self.alpha / x_batch.shape[0]
                self.Wh += x_batch.T.dot(deltah) * self.alpha / x_batch.shape[0]
                self.Th += np.sum(deltah, axis=0, keepdims=True) * self.alpha / x_batch.shape[0]

    def test(self, x):
        yh = self.tanh(np.dot(x, self.Wh) + self.Th)
        yo = np.dot(yh, self.Wo) + self.To
        return yo.flatten()

    def mean_squared_error(self, true_values, predictions):
        return np.mean((true_values - predictions) ** 2)

    def draw_test_on_interval(self, start, end,e, step=0.1):
        x_new = np.arange(start, end, step)
        func = lambda a, b, c, d, x: a * np.cos(b * x) + c * np.sin(d * x)
        y_new = func(0.1, 0.3, 0.08, 3, x_new)
        y_input = np.array([y_new[i - self.input_size: i] for i in range(self.input_size, len(y_new))])
        predictions = self.test(y_input)
        x_new_cropped = x_new[self.input_size:]
        print(e[:10])
        print(y_new[self.input_size:20])
        plt.figure(figsize=(12, 6))
        plt.plot(x_new_cropped, y_new[self.input_size:], label='Original Function', color='blue')
        plt.plot(x_new_cropped, predictions, label='Predictions', color='red', linestyle='dashed')
        plt.plot(x_new_cropped[:len(e)], e, label='noise', color='green', linestyle='dashed')
        plt.xlabel('X values')
        plt.ylabel('Function value')
        plt.title('Original Function vs. Predictions')
        plt.legend()
        plt.grid()
        plt.show()


def run_model(mode, input_size=10, hide_size=4, output_size=1, alpha=0.2, batch_size=10, epochs=10000):
    model = MLP(input_size=input_size, hide_size=hide_size, output_size=output_size, alpha=alpha)
    y, e, x_values = model.create_dataset(noise_percentage=0)
    start_time = time.time()

    for epoch in range(epochs):
        model.train(y, e, batch_size, mode=mode)
        if (epoch + 1) % 100 == 0:
            predictions = model.test(y)
            mse = model.mean_squared_error(e, predictions)
            print(f'Epoch {epoch + 1}, Mean Squared Error: {mse}')

    training_time = time.time() - start_time
    predictions = model.test(y)
    mse = model.mean_squared_error(e, predictions)
    model.draw_test_on_interval(0,50,e)

    return mse, training_time, epochs


def show_results_table(results):
    df = pd.DataFrame(results, columns=['Model Type', 'Test Error (MSE)', 'Epochs', 'Training Time (s)'])
    print(df)


# Run and collect results for different models
results = []
mse, training_time, epochs = run_model(mode="ba")
results.append(["Batch Adapt", mse, epochs, training_time])

mse, training_time, epochs = run_model(mode="b", batch_size=10)
results.append(["Batch Static", mse, epochs, training_time])

mse, training_time, epochs = run_model(mode="b", batch_size=1)
results.append(["Linear Static", mse, epochs, training_time])

mse, training_time, epochs = run_model(mode="ba", batch_size=1)
results.append(["Linear Adapt", mse, epochs, training_time])

# Show the table of results
show_results_table(results)
