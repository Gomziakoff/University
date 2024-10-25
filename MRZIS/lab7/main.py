import numpy as np
import matplotlib.pyplot as plt
import pandas as pd
import time


class MLP:
    def __init__(self, input_size, hide_size, output_size, alpha, dropout_rate=0.5):
        self.input_size = input_size
        self.output_size = output_size
        self.hide_size = hide_size
        self.Wh = np.random.uniform(size=(input_size, hide_size))
        self.Wo = np.random.uniform(size=(hide_size, output_size))
        self.Th = np.random.uniform(size=(1, hide_size))
        self.To = np.random.uniform(size=(1, output_size))
        self.alpha = alpha
        self.dropout_rate = dropout_rate

    def create_dataset(self, noise_percentage=0.1):
        func = lambda a, b, c, d, x: a * np.cos(b * x) + c * np.sin(d * x)
        x = np.arange(0, 15, 0.1)
        y = func(0.1, 0.3, 0.08, 3, x)
        y = np.array([y[i - self.input_size: i] for i in range(self.input_size, len(y))])
        e = np.array([y[i + self.input_size][0] for i in range(len(y) - self.input_size)])

        # Apply noise
        num_noisy_points = int(len(e) * (noise_percentage))
        noisy_indices = np.random.choice(len(e), num_noisy_points, replace=False)
        noise = np.random.uniform(-0.1, 0.1, size=num_noisy_points)
        e[noisy_indices] += noise

        return y[:len(y) - self.input_size], e, x[:len(y) - self.input_size]

    def sigmoid(self, x):
        return 1 / (1 + np.exp(-x))

    def sigmoid_derivative(self, x):
        return x * (1 - x)

    def dropout(self, layer_output):
        # Apply dropout to hidden layer
        mask = np.random.binomial(1, 1 - self.dropout_rate, size=layer_output.shape)
        return layer_output * mask

    def forward(self, X):
        # Hidden layer
        self.hidden_input = np.dot(X, self.Wh) - self.Th
        self.hidden_output = self.sigmoid(self.hidden_input)

        # Apply dropout
        self.hidden_output = self.dropout(self.hidden_output)

        # Output layer
        self.output_input = np.dot(self.hidden_output, self.Wo) - self.To
        self.output = self.sigmoid(self.output_input)

        return self.output

    def backward(self, X, y_true, output):
        # Output layer error and gradient
        error = y_true - output
        output_gradient = error * self.sigmoid_derivative(output)

        # Hidden layer error and gradient
        hidden_error = output_gradient.dot(self.Wo.T)
        hidden_gradient = hidden_error * self.sigmoid_derivative(self.hidden_output)

        # Reshape X for proper matrix multiplication
        X = X.reshape(1, -1)  # Ensure X is 2D: (1, input_size)

        # Update weights and biases
        self.Wo += self.hidden_output.T.dot(output_gradient) * self.alpha
        self.To -= np.sum(output_gradient, axis=0) * self.alpha
        self.Wh += X.T.dot(hidden_gradient) * self.alpha
        self.Th -= np.sum(hidden_gradient, axis=0) * self.alpha

        return error

    def train(self, X_train, y_train, epochs=100, loss_func="MSE"):
        errors = []
        for epoch in range(epochs):
            for X, y in zip(X_train, y_train):
                output = self.forward(X)
                error = self.backward(X, y, output)

            if loss_func == "MSE":
                error_value = np.mean(np.square(y_train - self.forward(X_train)))
            elif loss_func == "MAE":
                error_value = np.mean(np.abs(y_train - self.forward(X_train)))

            errors.append(error_value)
            print(f'Epoch {epoch + 1}/{epochs}, Error: {error_value}')

        return errors

    def test(self, X_test, y_test, loss_func="MSE"):
        output = self.forward(X_test)
        if loss_func == "MSE":
            return np.mean(np.square(y_test - output))
        elif loss_func == "MAE":
            return np.mean(np.abs(y_test - output))

    def plot_results(self, X_test, y_test, loss_func="MSE"):
        # Get predictions from the model
        y_pred = []
        for X in X_test:
            y_pred.append(self.forward(X))
        y_pred = np.array(y_pred).flatten()

        # Plot true vs predicted values
        plt.figure(figsize=(10, 6))
        plt.plot(y_test, label='True Function', color='blue')
        plt.plot(y_pred, label='Predicted Function', color='red', linestyle='--')
        plt.title(f'True vs Predicted Function (Loss: {loss_func})')
        plt.xlabel('Sample')
        plt.ylabel('Function Value')
        plt.legend()
        plt.show()


# Example usage
model = MLP(10, 4, 1, 0.1, dropout_rate=0.)
X, y, _ = model.create_dataset(noise_percentage=0.)
train_errors = model.train(X, y, epochs=200, loss_func="MSE")
test_error = model.test(X, y, loss_func="MSE")

# Plot training error over time
plt.plot(train_errors)
plt.title('Training Error (MSE)')
plt.xlabel('Epoch')
plt.ylabel('Error')
plt.show()

# Plot results: true vs predicted values
model.plot_results(X, y, loss_func="MSE")
