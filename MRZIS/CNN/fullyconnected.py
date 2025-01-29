import numpy as np


class FullyConnectedLayer:
    def __init__(self, size, outputs, activation_function):
        self.size = size
        self.input_width = size[0]
        self.input_height = size[1]
        self.input_depth = size[2]

        self.inputs = size[0] * size[1] * size[2]
        self.outputs = outputs

        self.activation_function = activation_function
        self.df = None

        self.b = np.random.rand(self.outputs)
        self.db = np.zeros(self.outputs)
        #
        self.W = np.random.uniform(-0.1, 0.1, size=(self.outputs, self.inputs))
        #

    def activate(self, output):
        if self.activation_function == 'ReLU':
            self.df = (output > 0).astype(int)
            output = np.maximum(0, output)
        if self.activation_function == 'Sigmoid':
            sigmoid = lambda x: 1 / (1 + np.exp(-x))
            self.df = sigmoid(output)*(1-sigmoid(output))
            output = sigmoid(output)
        return output

    def forward(self, X):
        self.X = X.flatten()
        output = self.W @ self.X + self.b
        output = self.activate(output)
        return output

    def backward(self, dout):
        dout = dout.flatten()
        delta = dout * self.df

        self.dW = np.outer(delta, self.X)
        self.db = delta

        dX = self.W.T @ delta
        dX = dX.reshape(self.size)
        return dX

    def update_weights(self, learning_rate):
        self.W -= learning_rate * self.dW
        self.b -= learning_rate * self.db



if __name__ == '__main__':
    model = FullyConnectedLayer((1,8,1), 4, "Sigmoid")
    X = np.array([1,2,-3,4,0,-7,2,-4])
    print(model.forward(X))
    dout = np.array([-0.5,0.1,-0.25,0.7])
    print(model.backward(dout))
    a = np.array([1,2,3,4,6,7,8,9])
    a = a.reshape(4,2)
    a = a.flatten()
    print(a)