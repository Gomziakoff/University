import numpy as np
import matplotlib.pyplot as plt
import pandas as pd
import time


class MLP:
    def __init__(self, input_size, hide_size, output_size, alpha, dropout_rate=0.0,elastic_net_alpha=0.0, l1_ratio=0.5, loss_func = "mse"):
        self.input_size = input_size
        self.output_size = output_size
        self.hide_size = hide_size
        self.Wh = np.random.uniform(size=(input_size, hide_size))
        self.Wo = np.random.uniform(size=(hide_size, output_size))
        self.Th = np.random.uniform(size=(1, hide_size))
        self.To = np.random.uniform(size=(1, output_size))
        self.alpha = alpha
        self.dropout_rate = dropout_rate
        self.dropout_mask = None
        self.elastic_net_alpha = elastic_net_alpha
        self.l1_ratio = l1_ratio
        self.loss_func = loss_func

    def tanh(self, x):
        return np.tanh(x)

    def tanh_derivative(self, x):
        return 1 - np.tanh(x) ** 2

    def apply_regularization(self, weights):
        l1_term = self.l1_ratio * np.sign(weights)  # L1 регуляризация
        l2_term = (1 - self.l1_ratio) * weights  # L2 регуляризация
        return self.elastic_net_alpha * (l1_term + l2_term)

    def apply_dropout(self, layer_output):
        if self.dropout_rate > 0:
            # Create a mask with 1s and 0s based on dropout_rate
            self.dropout_mask = np.random.binomial(1, 1 - self.dropout_rate, size=layer_output.shape)
            return layer_output * self.dropout_mask  # Apply mask
        return layer_output

    def create_dataset(self, noise_percentage=0.5):
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

    def train(self, x, e, batch_size):
        num_samples = x.shape[0]
        for start in range(0, num_samples, batch_size):
            end = start + batch_size
            x_batch = x[start:min(end, num_samples)]
            e_batch = e[start:min(end, num_samples)].reshape(-1, self.output_size)
            yh = self.tanh(np.dot(x_batch, self.Wh) + self.Th)
            yh = self.apply_dropout(yh)
            # Output layer calculation
            yo = np.dot(yh, self.Wo) + self.To
            if self.loss_func == "mae":
                error = e_batch - yo
                deltao = self.sign(error) * self.tanh_derivative(yo)
                hidden_error = deltao.dot(self.Wo.T)
                deltah = self.sign(hidden_error) * self.tanh_derivative(yh)

                if self.dropout_rate > 0:
                    deltah *= self.dropout_mask

                self.Wo += yh.T.dot(deltao) * self.alpha / x_batch.shape[0] - self.apply_regularization(self.Wo)
                self.To += np.sum(deltao, axis=0, keepdims=True) * self.alpha / x_batch.shape[0]
                self.Wh += x_batch.T.dot(deltah) * self.alpha / x_batch.shape[0] - self.apply_regularization(self.Wh)
                self.Th += np.sum(deltah, axis=0, keepdims=True) * self.alpha / x_batch.shape[0]
            elif self.loss_func == "mse":
                error = e_batch - yo
                deltao = error * self.tanh_derivative(yo)
                hidden_error = deltao.dot(self.Wo.T)
                deltah = hidden_error * self.tanh_derivative(yh)

                if self.dropout_rate > 0:
                    deltah *= self.dropout_mask

                self.Wo += yh.T.dot(deltao) * self.alpha / x_batch.shape[0] - self.apply_regularization(self.Wo)
                self.To += np.sum(deltao, axis=0, keepdims=True) * self.alpha / x_batch.shape[0]
                self.Wh += x_batch.T.dot(deltah) * self.alpha / x_batch.shape[0] - self.apply_regularization(self.Wh)
                self.Th += np.sum(deltah, axis=0, keepdims=True) * self.alpha / x_batch.shape[0]

    def test(self, x):
        yh = self.tanh(np.dot(x, self.Wh) + self.Th)
        yo = np.dot(yh, self.Wo) + self.To
        return yo.flatten()

    def mean_squared_error(self, true_values, predictions):
        return np.mean((true_values - predictions) ** 2)

    def mean_absolute_error(self, true_values, predictions):
        return np.mean(np.abs(true_values - predictions))

    def sign(self, x):
        result = np.sign(x)  # NumPy встроенная функция
        result[result == 0] = 0   # Обработка нулей
        return result

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


def run_model(input_size=10, hide_size=4, output_size=1, alpha=0.2, batch_size=10, epochs=10000, noise = 0.,dropout_rate=0., elastic_net_alpha=0.0, l1_ratio=0.5, loss_func="mse"):
    model = MLP(input_size=input_size, hide_size=hide_size, output_size=output_size, alpha=alpha, dropout_rate=dropout_rate,elastic_net_alpha=elastic_net_alpha,l1_ratio=l1_ratio,loss_func=loss_func)
    y, e, x_values = model.create_dataset(noise_percentage=noise)
    start_time = time.time()

    for epoch in range(epochs):
        model.train(y, e, batch_size)
        if (epoch + 1) % 100 == 0:
            predictions = model.test(y)
            mse = model.mean_squared_error(e, predictions) if loss_func == "mse" else model.mean_absolute_error(e, predictions)
            print(f'Epoch {epoch + 1}, {loss_func}: {mse}')

    training_time = time.time() - start_time
    predictions = model.test(y)
    mse = model.mean_squared_error(e, predictions)
    model.draw_test_on_interval(0,50,e)

    return mse, training_time, epochs, loss_func


def show_results_table(results):
    df = pd.DataFrame(results, columns=['Model Type', 'Test Error','loss_func', 'Epochs', 'Training Time (s)'])
    print(df)


# Run and collect results for different models
results = []
mse, training_time, epochs, loss_func = run_model(batch_size=10,hide_size=4, epochs=10000,noise=0.5,dropout_rate=0., elastic_net_alpha=0., loss_func="mse")
results.append(["Just MSE", mse,loss_func, epochs, training_time])

mse, training_time, epochs, loss_func = run_model(batch_size=10,hide_size=4, epochs=10000,noise=0.5,dropout_rate=0., elastic_net_alpha=0.,  loss_func="mae")
results.append(["Just MAE", mse,loss_func, epochs, training_time])

mse, training_time, epochs, loss_func = run_model(batch_size=10,hide_size=4, epochs=10000,noise=0.5,dropout_rate=0.25, elastic_net_alpha=0., loss_func="mse")
results.append(["MSE drop", mse,loss_func, epochs, training_time])

mse, training_time, epochs, loss_func = run_model(batch_size=10,hide_size=4, epochs=10000,noise=0.5,dropout_rate=0.25, elastic_net_alpha=0.,  loss_func="mae")
results.append(["MAE drop", mse,loss_func, epochs, training_time])

mse, training_time, epochs, loss_func = run_model(batch_size=10,hide_size=4, epochs=10000,noise=0.5,dropout_rate=0., elastic_net_alpha=0.000001, loss_func="mse")
results.append(["MSE elasticnet", mse,loss_func, epochs, training_time])

mse, training_time, epochs, loss_func = run_model(batch_size=10,hide_size=4, epochs=10000,noise=0.5,dropout_rate=0., elastic_net_alpha=0.000001,  loss_func="mae")
results.append(["MAE elasticnet", mse,loss_func, epochs, training_time])
show_results_table(results)
