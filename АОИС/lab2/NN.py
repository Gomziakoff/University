import matplotlib.pyplot as plt
import numpy as np


class NN:
    def __init__(self, alpha=0.01, neurons=5):
        self.alpha = alpha
        self.neurons = neurons
        self.W = np.random.rand(neurons)
        self.T = np.zeros(1)
        self.errors = []

    def create_dataset(self, y):
        return np.array([y[i - self.neurons : i] for i in range(self.neurons, len(y))])

    def MSE(self, x, e):
        return (1 / 2) * ((self.predict(x) - e) ** 2)

    def predict(self, x):
        return self.W @ x - self.T

    def train(self, X, E):
        for x, e in zip(X[: 31 - self.neurons], E[self.neurons : 31]):
            y = self.predict(x)
            self.W += -self.alpha * (y - e) * x
            self.T += self.alpha * (y - e)

    def test(self, epoch: int, X, E):
        values = []
        error = np.mean([self.MSE(x, e) for x, e in zip(X[31 - self.neurons :], E[31:])])
        for x, e in zip(X[0 : len(E) - self.neurons], E[self.neurons : len(E)]):
            values.append(self.predict(x))
        print(f"Epoch: {epoch}: e={error}")
        return values

    def set_MSE(self, X, E):
        self.errors.append(np.mean([self.MSE(x, e) for x, e in zip(X[31 - self.neurons :], E[31:])]))


if __name__ == "__main__":
    # best alpha=0.2 neurons=5
    neurons = 5
    model = NN(alpha=0.02, neurons=neurons)

    arrange = np.arange(0, 5, 0.1)

    func = lambda a, b, d, x: a * np.sin(b * x) + d
    y = func(1, 8, 0.4, np.arange(0, 5, 0.1))
    x = model.create_dataset(y)
    last_epoch = -1
    for epoch in range(1, 100):
        model.train(x, y)
        model.set_MSE(x, y)
        model.test(epoch, x, y)
        if model.errors[-1] <= 1e-5:
            last_epoch = epoch
            break
    if last_epoch == -1:
        last_epoch = 99
    print("Training results")
    values = model.test(last_epoch, x, y)

    plt.plot(range(1, len(model.errors) + 1), model.errors)
    plt.xlabel("Epoch")
    plt.ylabel("Error")
    plt.savefig("NN.png")

    plt.clf()

    plt.plot(np.arange(0, 5, 0.1), func(1, 8, 0.4, np.arange(0, 5, 0.1)), label="function")
    plt.plot(np.arange(neurons / 10, 5, 0.1), values, label="predict")
    plt.xlabel("X")
    plt.ylabel("Y")
    plt.legend(loc="upper center")
    plt.savefig("graph.png")
