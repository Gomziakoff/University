import numpy as np
import torch
import torch.nn as nn
import torch.optim as optim

class Autoencoder:
    def __init__(self, input_dim, encoding_dim, learning_rate, alpha=0.0, optimizer_type="cumulative"):
        self.input_dim = input_dim
        self.encoding_dim = encoding_dim
        self.learning_rate = learning_rate
        self.alpha = alpha
        self.optimizer_type = optimizer_type

        self.weights_input_encoding = np.random.randn(input_dim, encoding_dim)
        self.weights_encoding_input = np.random.randn(encoding_dim, input_dim)
        self.bias_encoding = np.random.randn(1, encoding_dim)
        self.bias_input = np.random.randn(1, input_dim)

        self.prev_delta_weights_input_encoding = np.zeros((input_dim, encoding_dim))
        self.prev_delta_weights_encoding_input = np.zeros((encoding_dim, input_dim))
        self.prev_delta_bias_encoding = np.zeros((1, encoding_dim))
        self.prev_delta_bias_input = np.zeros((1, input_dim))

        self.train_errors = []
        self.test_errors = []

        if optimizer_type == "adam":
            self.model = nn.Sequential(
                nn.Linear(input_dim, encoding_dim),
                nn.Sigmoid(),
                nn.Linear(encoding_dim, input_dim),
                nn.Sigmoid()
            )
            self.criterion = nn.MSELoss()
            self.optimizer = optim.Adam(self.model.parameters(), lr=learning_rate)

    @staticmethod
    def sigmoid(x):
        return 1 / (1 + np.exp(-x))

    @staticmethod
    def sigmoid_derivative(x):
        return x * (1 - x)

    def encode(self, inputs):
        self.encoding_layer_input = np.dot(inputs, self.weights_input_encoding) + self.bias_encoding
        self.encoding_layer_output = self.sigmoid(self.encoding_layer_input)

    def decode(self, encoding_output):
        self.input_layer_input = np.dot(encoding_output, self.weights_encoding_input) + self.bias_input
        self.input_layer_output = self.sigmoid(self.input_layer_input)

    def backward_pass(self, inputs):
        if self.optimizer_type == "adam":
            inputs_tensor = torch.tensor(inputs, dtype=torch.float32)
            outputs = self.model(inputs_tensor)
            loss = self.criterion(outputs, inputs_tensor)

            self.optimizer.zero_grad()
            loss.backward()
            self.optimizer.step()

            self.train_errors.append(loss.item())
            return

        encoding_error = inputs - self.input_layer_output
        encoding_delta = encoding_error * self.sigmoid_derivative(self.input_layer_output)

        if self.optimizer_type == "oja":
            self.weights_encoding_input += self.learning_rate * np.dot(self.encoding_layer_output.T, encoding_delta) / inputs.shape[0]
            self.bias_input += self.learning_rate * np.mean(encoding_delta, axis=0, keepdims=True)

            decoding_error = encoding_delta.dot(self.weights_encoding_input.T)
            decoding_delta = decoding_error * self.sigmoid_derivative(self.encoding_layer_output)

            for i in range(self.encoding_dim):
                delta_w = self.learning_rate * (self.encoding_layer_output[:, i][:, np.newaxis] * (
                        inputs - self.encoding_layer_output[:, i][:, np.newaxis] * self.weights_input_encoding[:, i])).mean(axis=0)
                self.weights_input_encoding[:, i] += delta_w

            self.bias_encoding += self.learning_rate * np.mean(decoding_delta, axis=0, keepdims=True)

        elif self.optimizer_type == "cumulative":
            delta_weights_encoding_input = (
                self.learning_rate * np.dot(self.encoding_layer_output.T, encoding_delta) / inputs.shape[0]
                + self.alpha * self.prev_delta_weights_encoding_input
            )
            delta_bias_input = (
                self.learning_rate * np.mean(encoding_delta, axis=0, keepdims=True)
                + self.alpha * self.prev_delta_bias_input
            )

            self.weights_encoding_input += delta_weights_encoding_input
            self.bias_input += delta_bias_input

            decoding_error = encoding_delta.dot(self.weights_encoding_input.T)
            decoding_delta = decoding_error * self.sigmoid_derivative(self.encoding_layer_output)

            delta_weights_input_encoding = (
                self.learning_rate * np.dot(inputs.T, decoding_delta) / inputs.shape[0]
                + self.alpha * self.prev_delta_weights_input_encoding
            )
            delta_bias_encoding = (
                self.learning_rate * np.mean(decoding_delta, axis=0, keepdims=True)
                + self.alpha * self.prev_delta_bias_encoding
            )

            self.weights_input_encoding += delta_weights_input_encoding
            self.bias_encoding += delta_bias_encoding

            # Store current deltas for next iteration
            self.prev_delta_weights_input_encoding = delta_weights_input_encoding
            self.prev_delta_weights_encoding_input = delta_weights_encoding_input
            self.prev_delta_bias_encoding = delta_bias_encoding
            self.prev_delta_bias_input = delta_bias_input

        elif self.optimizer_type == "sgd":
            self.weights_encoding_input += self.learning_rate * np.dot(self.encoding_layer_output.T, encoding_delta) / inputs.shape[0]
            self.bias_input += self.learning_rate * np.mean(encoding_delta, axis=0, keepdims=True)

            decoding_error = encoding_delta.dot(self.weights_encoding_input.T)
            decoding_delta = decoding_error * self.sigmoid_derivative(self.encoding_layer_output)

            self.weights_input_encoding += self.learning_rate * np.dot(inputs.T, decoding_delta) / inputs.shape[0]
            self.bias_encoding += self.learning_rate * np.mean(decoding_delta, axis=0, keepdims=True)

        mse = np.mean((inputs - self.input_layer_output) ** 2)
        self.train_errors.append(mse)

    def train(self, train_data, test_data, epochs=10):
        for epoch in range(epochs):
            if self.optimizer_type == "adam":
                self.backward_pass(train_data)
            else:
                self.encode(train_data)
                self.decode(self.encoding_layer_output)
                self.backward_pass(train_data)

            if self.optimizer_type == "adam":
                test_inputs_tensor = torch.tensor(test_data, dtype=torch.float32)
                test_outputs = self.model(test_inputs_tensor)
                test_loss = self.criterion(test_outputs, test_inputs_tensor)
                self.test_errors.append(test_loss.item())
            else:
                self.encode(test_data)
                self.decode(self.encoding_layer_output)
                test_mse = np.mean((test_data - self.input_layer_output) ** 2)
                self.test_errors.append(test_mse)

            if (epoch + 1) % 100 == 0:
                print(f"Epoch {epoch + 1}/{epochs}, Train MSE: {self.train_errors[-1]:.4f}, Test MSE: {self.test_errors[-1]:.4f}")

    def encode_data(self, inputs_data):
        if self.optimizer_type == "adam":
            inputs_tensor = torch.tensor(inputs_data, dtype=torch.float32)
            return self.model[:2](inputs_tensor).detach().numpy()
        self.encode(inputs_data)
        return self.encoding_layer_output

    def decode_data(self, encoded_data):
        if self.optimizer_type == "adam":
            encoded_tensor = torch.tensor(encoded_data, dtype=torch.float32)
            return self.model[2:](encoded_tensor).detach().numpy()
        self.decode(encoded_data)
        return self.input_layer_output

