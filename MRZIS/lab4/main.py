import numpy as np

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

    def sigmoid(self, x):
        return 1 / (1 + np.exp(-x))

    def sigmoid_derivative(self, x):
        return x * (1 - x)

    def train(self, x, e, batch_size):
        num_samples = x.shape[0]

        for start in range(0, num_samples, batch_size):
            end = start + batch_size
            x_batch = x[start:end]
            e_batch = e[start:end]

            yh = self.sigmoid(np.dot(x_batch, self.Wh) + self.Th)
            yo = self.sigmoid(np.dot(yh, self.Wo) + self.To)

            error = e_batch - yo
            deltao = error * self.sigmoid_derivative(yo)
            hidden_error = deltao.dot(self.Wo.T)
            deltah = hidden_error * self.sigmoid_derivative(yh)

            self.Wo += yh.T.dot(deltao) * self.alpha / batch_size
            self.To += np.sum(deltao, axis=0, keepdims=True) * self.alpha / batch_size
            self.Wh += x_batch.T.dot(deltah) * self.alpha / batch_size
            self.Th += np.sum(deltah, axis=0, keepdims=True) * self.alpha / batch_size

    def test(self, x, e):
        yh = self.sigmoid(np.dot(x, self.Wh) + self.Th)
        yo = self.sigmoid(np.dot(yh, self.Wo) + self.To)
        print(yo)

x = np.array([[0, 0], [0, 1], [1, 0], [1, 1]])
y = np.array([[0], [1], [1], [0]])

model = MLP(input_size=2, hide_size=2, output_size=1, alpha=0.2)
batch_size = 2 

for epoch in range(10000):
    model.train(x, y, batch_size)

model.test(x, y)
