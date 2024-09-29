import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
import time

class NN:
    def __init__(self, alpha=0.01, neurons=5):
        self.alpha = alpha
        self.neurons = neurons
        self.W = np.random.rand(neurons)
        self.T = np.zeros(1)
        self.errors = []

    def create_dataset(self, y):
        return np.array([y[i - self.neurons : i] for i in range(self.neurons, len(y))])

    def adaptive_alpha(self, x):
        return 1/(1+sum(x**2))

    def a(self,X,E,xk):
        a = 0
        for x, e in zip(X,E):
            sum = 0
            for i in range(len(x)):
                sum+=xk[i]*x[i]
            a+=(self.predict(x)-e)*(1+sum)
        return a

    def adaptive_batch_alpha(self, X, E):
        sum_a = 0
        sum_b = 0
        for x, e in zip(X,E):
            sum_a+=(self.predict(x)-e)*self.a(X,E,x)
            sum_b+=(self.a(X,E,x))**2
        return sum_a/sum_b

    def MSE(self, x, e):
        return (1 / 2) * ((self.predict(x) - e) ** 2)

    def predict(self, x):
        return self.W @ x - self.T

    def train(self, X, E):
        for x, e in zip(X[: 31 - self.neurons], E[self.neurons : 31]):
            y = self.predict(x)
            self.W += -self.alpha * (y - e) * x
            self.T += self.alpha * (y - e)

    def adapt_train(self, X, E):
        for x, e in zip(X[: 31 - self.neurons], E[self.neurons: 31]):
            y = self.predict(x)
            self.W += -self.adaptive_alpha(x) * (y - e) * x
            self.T += self.adaptive_alpha(x) * (y - e)


    def batch_train(self, X, E, batch_size):
        num_batches = len(X[: 31 - self.neurons]) // batch_size + (
            1 if len(X[: 31 - self.neurons]) % batch_size != 0 else 0)
        for batch_index in range(num_batches):
            start = batch_index * batch_size
            end = min((batch_index + 1) * batch_size, len(X[: 31 - self.neurons]))

            W_update = np.zeros_like(self.W)
            T_update = np.zeros(1)

            for x, e in zip(X[start:end], E[start+self.neurons:end+self.neurons]):
                y = self.predict(x)
                W_update += (y - e) * x
                T_update += (y - e)

            self.W += -self.alpha * W_update
            self.T += self.alpha * T_update

    def adapt_batch_train(self, X, E, batch_size):
        num_batches = len(X[: 31 - self.neurons]) // batch_size + (
            1 if len(X[: 31 - self.neurons]) % batch_size != 0 else 0)
        for batch_index in range(num_batches):
            start = batch_index * batch_size
            end = min((batch_index + 1) * batch_size, len(X[: 31 - self.neurons]))

            W_update = np.zeros_like(self.W)
            T_update = np.zeros(1)

            for x, e in zip(X[start:end], E[start+self.neurons:end+self.neurons]):
                y = self.predict(x)
                W_update += (y - e) * x
                T_update += (y - e)

            self.W += -self.adaptive_batch_alpha(X[start:end], E[start+self.neurons:end+self.neurons]) * W_update
            self.T += self.adaptive_batch_alpha(X[start:end], E[start+self.neurons:end+self.neurons]) * T_update

    def test(self, epoch: int, X, E):
        values = []
        error = np.mean([self.MSE(x, e) for x, e in zip(X[31 - self.neurons :], E[31:])])
        for x, e in zip(X[0 : len(E) - self.neurons], E[self.neurons : len(E)]):
            values.append(self.predict(x))
        print(f"Epoch: {epoch}: e={error}")
        return values

    def set_MSE(self, X, E):
        self.errors.append(np.mean([self.MSE(x, e) for x, e in zip(X[31 - self.neurons :], E[31:])]))

def collect_results(model_name, model, epochs, start_time):
    results = {
        "Model": model_name,
        "Test Error": model.errors[-1],  # Last error value
        "Epochs": epochs,
        "Training Time (s)": time.time() - start_time  # Time in seconds
    }
    return results

# Create an empty list to store results
results_table = []

if __name__ == "__main__":
    # best alpha=0.2 neurons=5
    neurons = 5
    lin_model = NN(alpha=0.05, neurons=neurons)
    lin_model_adapt = NN(alpha=0.05, neurons=neurons)
    batch_model = NN(alpha=0.05, neurons=neurons)
    batch_model_adapt = NN(alpha=0.05, neurons=neurons)

    arrange = np.arange(0, 5, 0.1)

    func = lambda a, b, d, x: a * np.sin(b * x) + d
    y = func(1, 8, 0.3, np.arange(0, 5, 0.1))
    x = lin_model.create_dataset(y)

    # Linear model
    print("\nLinear model\n")
    start_time = time.time()
    last_epoch = -1
    for epoch in range(1, 100):
        lin_model.train(x, y)
        lin_model.set_MSE(x, y)
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
    print("\nLinear model adapt\n")
    start_time = time.time()
    last_epoch = -1
    for epoch in range(1, 100):
        lin_model_adapt.adapt_train(x, y)
        lin_model_adapt.set_MSE(x, y)
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
        batch_model.batch_train(x, y, 10)
        batch_model.set_MSE(x, y)
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
        batch_model_adapt.adapt_batch_train(x, y, 10)
        batch_model_adapt.set_MSE(x, y)
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

    # Convert results to a pandas DataFrame
    df = pd.DataFrame(results_table)

    # Print the table
    print("\nTraining Results Table:")
    print(df)

    # Optionally, save the table to a CSV file
    df.to_csv("training_results.csv", index=False)

    plt.plot(range(1, len(lin_model.errors) + 1), lin_model.errors, label="Lin Model", linestyle='-', marker='o')
    plt.plot(range(1, len(lin_model_adapt.errors) + 1), lin_model_adapt.errors, label="Lin Model Adapt", linestyle='--',
             marker='s')
    plt.plot(range(1, len(batch_model.errors) + 1), batch_model.errors, label="Batch Model", linestyle='-.', marker='^')
    plt.plot(range(1, len(batch_model_adapt.errors) + 1), batch_model_adapt.errors, label="Batch Model Adapt",
             linestyle=':', marker='d')
    plt.xlabel("Epoch")
    plt.ylabel("Error")
    plt.legend()  # Optional, to add labels for each line
    plt.savefig("NN.png")

    plt.clf()

    plt.plot(np.arange(0, 5, 0.1), func(1, 8, 0.3, np.arange(0, 5, 0.1)), label="Original Function")
    plt.plot(np.arange(neurons / 10, 5, 0.1), lin_values, label="Prediction (Linear)")
    plt.plot(np.arange(neurons / 10, 5, 0.1), lin_values_adapt, label="Prediction (Linear, Adaptive)")
    plt.plot(np.arange(neurons / 10, 5, 0.1), batch_values, label="Prediction (Batch)")
    plt.plot(np.arange(neurons / 10, 5, 0.1), batch_values_adapt, label="Prediction (Batch, Adaptive)")

    plt.xlabel("X (Input Value)")
    plt.ylabel("Y (Output Value)")
    plt.title("Comparison of Original Function and Neural Network Predictions")
    plt.legend(loc="upper center")
    plt.savefig("graph.png")
