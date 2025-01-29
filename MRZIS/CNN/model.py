from fullyconnected import FullyConnectedLayer
from pooling import MaxPoolLayer
from convolution import ConvLayer
from activation import ReLUActivationLayer
import numpy as np
from tqdm import tqdm
import time

class Model:
    def __init__(self, train_img, train_label, test_img, test_label, learning_rate = 0.1, num_epochs = 100):
        self.train_img = train_img
        self.train_label = train_label
        self.test_img = test_img
        self.test_label = test_label
        self.learning_rate = learning_rate
        self.num_epochs = num_epochs

        self.conv1 = ConvLayer((32, 32, 3), 6, 5, 0, 1)
        self.activation1 = ReLUActivationLayer((28,28,6))
        self.pool1 = MaxPoolLayer((28, 28, 6), 2)
        self.conv2 = ConvLayer((14, 14, 6), 16, 5, 0, 1)
        self.activation2 = ReLUActivationLayer((10,10,16))
        self.pool2 = MaxPoolLayer((10, 10, 16), 2)
        self.fullconected1 = FullyConnectedLayer((5, 5, 16), 200, "Sigmoid")
        self.fullconected2 = FullyConnectedLayer((1, 200, 1), 10, "Sigmoid")

    def forward(self, X):
        start_time = time.time()
        out = self.conv1.forward(X)
        print(f"Conv1 forward time: {time.time() - start_time:.6f} seconds")

        start_time = time.time()
        out = self.activation1.forward(out)
        print(f"Activation1 forward time: {time.time() - start_time:.6f} seconds")

        start_time = time.time()
        out = self.pool1.forward(out)
        print(f"Pool1 forward time: {time.time() - start_time:.6f} seconds")

        start_time = time.time()
        out = self.conv2.forward(out)
        print(f"Conv2 forward time: {time.time() - start_time:.6f} seconds")

        start_time = time.time()
        out = self.activation2.forward(out)
        print(f"Activation2 forward time: {time.time() - start_time:.6f} seconds")

        start_time = time.time()
        out = self.pool2.forward(out)
        print(f"Pool2 forward time: {time.time() - start_time:.6f} seconds")

        start_time = time.time()
        out = self.fullconected1.forward(out)
        print(f"FullConnected1 forward time: {time.time() - start_time:.6f} seconds")

        start_time = time.time()
        out = self.fullconected2.forward(out)
        print(f"FullConnected2 forward time: {time.time() - start_time:.6f} seconds")

        return out

    def backward(self, out, Y):
        start_time = time.time()
        loss = 0.5 * np.sum((out - Y) ** 2)
        print(f"Loss computation time: {time.time() - start_time:.6f} seconds")

        start_time = time.time()
        dout = out - Y
        print(f"Loss gradient computation time: {time.time() - start_time:.6f} seconds")

        start_time = time.time()
        dout = self.fullconected2.backward(dout)
        print(f"FullConnected2 backward time: {time.time() - start_time:.6f} seconds")

        start_time = time.time()
        dout = self.fullconected1.backward(dout)
        print(f"FullConnected1 backward time: {time.time() - start_time:.6f} seconds")

        start_time = time.time()
        dout = self.pool2.backward(dout)
        print(f"Pool2 backward time: {time.time() - start_time:.6f} seconds")

        start_time = time.time()
        dout = self.activation2.backward(dout)
        print(f"Activation2 backward time: {time.time() - start_time:.6f} seconds")

        start_time = time.time()
        dout = self.conv2.backward(dout)
        print(f"Conv2 backward time: {time.time() - start_time:.6f} seconds")

        start_time = time.time()
        dout = self.pool1.backward(dout)
        print(f"Pool1 backward time: {time.time() - start_time:.6f} seconds")

        start_time = time.time()
        dout = self.activation1.backward(dout)
        print(f"Activation1 backward time: {time.time() - start_time:.6f} seconds")

        start_time = time.time()
        dout = self.conv1.backward(dout)
        print(f"Conv1 backward time: {time.time() - start_time:.6f} seconds")

        return loss

    def update_weights(self):
        self.conv1.update_weights(self.learning_rate)
        self.conv2.update_weights(self.learning_rate)
        self.fullconected1.update_weights(self.learning_rate)
        self.fullconected2.update_weights(self.learning_rate)

    def train(self):
        epoch = 0
        while epoch < self.num_epochs:
            avg_loss = 0
            print(f'Epoch {epoch + 1}/{self.num_epochs}')

            # Используем tqdm для отображения прогресса по обучающим изображениям
            for img, label in tqdm(zip(self.train_img[0:100,:,:,:], self.train_label[0:100,:]), total=len(self.train_img[0:100,:,:,:]), desc="Training",leave=False):
                out = self.forward(img)
                loss = self.backward(out, label)
                self.update_weights()
                avg_loss += loss
            epoch += 1
            print(avg_loss/50000)