import numpy as np
import pandas as pd
import time

def create_dataset():
    data = pd.read_csv('mushrooms.csv')
    encoded_data = pd.get_dummies(data)
    X = encoded_data.drop(columns=['class_e', 'class_p'])
    y = encoded_data['class_p']
    return np.array(X), np.array(y)

def train_test_split(X, y, test_size=0.2, random_state=None):
    if random_state is not None:
        np.random.seed(random_state)

    test_size = int(len(X) * test_size)
    indices = np.arange(len(X))
    np.random.shuffle(indices)

    test_indices = indices[:test_size]
    train_indices = indices[test_size:]

    X_train = X[train_indices]
    X_test = X[test_indices]
    y_train = y[train_indices]
    y_test = y[test_indices]

    return X_train, X_test, y_train, y_test

class MLP:
    def __init__(self, input_size, output_size, hidden_size, learning_rate="adapt"):
        self.input_size = input_size
        self.output_size = output_size
        self.hidden_size = hidden_size
        self.learning_rate = learning_rate

        self.weights_input_hidden = np.random.randn(self.input_size, self.hidden_size)
        self.bias_hidden = np.zeros((1, self.hidden_size))

        self.weights_hidden_output = np.random.randn(self.hidden_size, self.output_size)
        self.bias_output = np.zeros((1, self.output_size))

    def sigmoid(self, x):
        return 1 / (1 + np.exp(-x))

    def sigmoid_derivative(self, x):
        return x * (1 - x)

    def forward(self, inputs):
        self.hidden_input = np.dot(inputs, self.weights_input_hidden) + self.bias_hidden
        self.hidden_output = self.sigmoid(self.hidden_input)
        self.output = np.dot(self.hidden_output, self.weights_hidden_output) + self.bias_output
        return self.sigmoid(self.output)

    def backward(self, inputs, targets, outputs):
        targets = targets.reshape(-1, 1)
        error = targets - outputs
        delta_output = error * self.sigmoid_derivative(outputs)

        if self.learning_rate == "adapt":
            self.learning_rate_output = (4 * np.sum((error ** 2 * outputs * (1 - outputs)), axis=0)) / \
                                         (1 + np.sum(self.hidden_output ** 2) + 1e-10) * \
                                         (np.sum((error * outputs * (1 - outputs)) ** 2, axis=0) + 1e-10)

            h_error = delta_output.dot(self.weights_hidden_output.T)
            self.learning_rate_hidden = (4 * np.sum((h_error ** 2 * self.hidden_output * (1 - self.hidden_output)), axis=0)) / \
                                         (1 + np.sum(self.hidden_input ** 2) + 1e-10) * \
                                         (np.sum((h_error * self.hidden_output * (1 - self.hidden_output)) ** 2, axis=0) + 1e-10)
            self.learning_rate_hidden = self.learning_rate_output
            delta_hidden = h_error * self.sigmoid_derivative(self.hidden_output)

            self.weights_hidden_output += self.hidden_output.T.dot(delta_output) * self.learning_rate_output
            self.bias_output += np.sum(delta_output, axis=0, keepdims=True) * self.learning_rate_output
            self.weights_input_hidden += inputs.T.dot(delta_hidden) * self.learning_rate_hidden
            self.bias_hidden += np.sum(delta_hidden, axis=0, keepdims=True) * self.learning_rate_hidden
        else:
            delta_hidden = delta_output.dot(self.weights_hidden_output.T) * self.sigmoid_derivative(self.hidden_output)

            self.weights_hidden_output += self.hidden_output.T.dot(delta_output) * self.learning_rate
            self.bias_output += np.sum(delta_output, axis=0, keepdims=True) * self.learning_rate
            self.weights_input_hidden += inputs.T.dot(delta_hidden) * self.learning_rate
            self.bias_hidden += np.sum(delta_hidden, axis=0, keepdims=True) * self.learning_rate

    def train(self, inputs, targets, epochs, batch_size):
        training_losses = []
        for epoch in range(epochs):
            loss = 0
            for i in range(0, len(inputs), batch_size):
                input_data = inputs[i:i + batch_size]
                target_data = targets[i:i + batch_size]

                output = self.forward(input_data)
                self.backward(input_data, target_data, output)

                loss += np.mean(np.square(output - target_data))

            avg_loss = loss / (len(inputs) // batch_size)
            training_losses.append(avg_loss)
            print(f'Epoch {epoch + 1}, Loss: {avg_loss}')

        return training_losses

    def predict(self, inputs):
        output = self.forward(inputs)
        return np.round(output)

# Create and prepare dataset
X, y = create_dataset()
X_train, X_test, y_train, y_test = train_test_split(X, y, 0.3, random_state=1)
input_size = X_train.shape[1]

# List of configurations to try
configurations = [
    {'batch_size': 1, 'learning_rate': 0.1},
    {'batch_size': 256, 'learning_rate': 0.1},
    {'batch_size': 1, 'learning_rate': 'adapt'},
    {'batch_size': 256, 'learning_rate': 'adapt'},
]

# List to store results
results = []

for config in configurations:
    model = MLP(input_size, 1, hidden_size=5, learning_rate=config['learning_rate'])

    # Record training time
    start_time = time.time()
    model.train(X_train, y_train, epochs=200, batch_size=config['batch_size'])
    training_time = time.time() - start_time

    # Test the model
    predictions = model.predict(X_test)
    accuracy_test = np.mean(predictions.flatten() == y_test)
    test_error = 1 - accuracy_test

    predictions_train = model.predict(X_train)
    accuracy_train = np.mean(predictions_train.flatten() == y_train)

    # Store results
    results.append({
        'Batch Size': config['batch_size'],
        'Learning Rate': config['learning_rate'],
        'Test Error': test_error,
        'Train Accuracy (%)': accuracy_train * 100,
        'Test Accuracy (%)': accuracy_test * 100,
        'Training Time (s)': training_time,
        'Epochs': 200,
    })

# Create a summary DataFrame
pd.options.display.max_columns = None  # Show all columns
pd.options.display.max_rows = None     # Show all rows
pd.options.display.width = 1000
results_df = pd.DataFrame(results)
results_df.to_csv('mlp_training_results.csv', index=False)
print("Results saved to 'mlp_training_results.csv'")
print(results_df)
