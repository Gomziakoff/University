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


if __name__ == "__main__":
    # best alpha=0.2 neurons=5
    neurons = 5
    model = NN(alpha=0.05, neurons=neurons)

    arrange = np.arange(0, 5, 0.1)

    func = lambda a, b, d, x: a * np.sin(b * x) + d
    y = func(1, 8, 0.3, np.arange(0, 5, 0.1))
    x = model.create_dataset(y)
    last_epoch = -1
    for epoch in range(1, 100):
        model.batch_train(x, y,10)
        #model.train(x,y)
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

    plt.plot(np.arange(0, 5, 0.1), func(1, 8, 0.3, np.arange(0, 5, 0.1)), label="function")
    plt.plot(np.arange(neurons / 10, 5, 0.1), values, label="predict")
    plt.xlabel("X")
    plt.ylabel("Y")
    plt.legend(loc="upper center")
    plt.savefig("graph.png")
