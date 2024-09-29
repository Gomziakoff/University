import numpy as np
import matplotlib.pyplot as plt
import random
import pandas as pd
import time
class NN:
    def __init__(self,alpha,n_samples,noise_ratio):
        self.noise_ratio = noise_ratio
        self.n_samples = n_samples//2
        self.alpha = alpha
        #self.W = np.random.randn(2)
        self.W = np.array([0.1,0.1])
        self.T = np.zeros(1)
        self.errors = []
        self.acuracy = 0
    def decision_boundary(self,x):
        return -2*x + 12
    def create_dataset(self):
        X = []
        y = []
        for i in range(self.n_samples):
            x = random.randrange(-100, 100)
            y_coord = random.randrange(-300, self.decision_boundary(x))
            X.append([x, y_coord])
            y.append(-1)
        for i in range(self.n_samples):
            x = random.randrange(-100, 100)
            y_coord = random.randrange(self.decision_boundary(x), 300)
            X.append([x, y_coord])
            y.append(1)
        n_noise_class0 = int(self.noise_ratio * self.n_samples)
        n_noise_class1 = int(self.noise_ratio * self.n_samples)
        for i in range(n_noise_class0):
            x = random.randrange(-100, 100)
            y_coord = self.decision_boundary(x) + random.randrange(0, 100)
            X[i] = [x, y_coord]
            y[i] = 0
        for i in range(n_noise_class1):
            x = random.randrange(-100, 100)
            y_coord = self.decision_boundary(x) - random.randrange(0, 100)
            X[self.n_samples + i] = [x, y_coord]
            y[self.n_samples + i] = 1
        return np.array(X), np.array(y)
    def plot_dataset(self, X, y):
        x_line = np.linspace(-100, 100, 400)
        y_line = self.decision_boundary(x_line)
        plt.figure(figsize=(10, 6))
        plt.scatter(X[y == 0][:, 0], X[y == 0][:, 1], color='blue', label='Class 0')
        plt.scatter(X[y == 1][:, 0], X[y == 1][:, 1], color='red', label='Class 1')
        plt.plot(x_line, y_line, color='black', linestyle='--', label='y = -2x + 12')
        plt.title('Генерация датасета с шумами и разделением по линии y = -2x + 12')
        plt.xlabel('X1')
        plt.ylabel('X2')
        plt.legend()
        plt.grid(True)
        plt.show()
    def MSE(self, x, e):
        return (1 / 2) * ((self.predict(x) - e) ** 2)
    def set_MSE(self, X, E):
        self.errors.append(np.mean([self.MSE(x, e) for x, e in zip(X, E)]))
    def activate(self, x):
        return 1 if x > 0 else -1
    def predict(self, X):
        linear_output =  self.W @ X + self.T
        return self.activate(linear_output)
    def train(self, X, E):
        for x,e in zip(X,E):
            y = self.predict(x)
            self.W += -self.alpha * (y - e) * x
            self.T += self.alpha * (y - e)
    def adaptive_alpha(self, x):
        return 1/(1+sum(x**2))
    def adapt_train(self, X, E):
        for x, e in zip(X, E):
            y = self.predict(x)
            self.W += -self.adaptive_alpha(x) * (y - e) * x
            self.T += self.adaptive_alpha(x) * (y - e)
    def batch_train(self, X, E, batch_size):
        num_batches = len(X) // batch_size + (
            1 if len(X) % batch_size != 0 else 0)
        for batch_index in range(num_batches):
            W_update = np.zeros_like(self.W)
            T_update = np.zeros(1)
            for x, e in zip(X, E):
                y = self.predict(x)
                W_update += (y - e) * x
                T_update += (y - e)
            self.W += -self.alpha * W_update
            self.T += self.alpha * T_update
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
    def adapt_batch_train(self, X, E, batch_size):
        num_batches = len(X) // batch_size + (
            1 if len(X) % batch_size != 0 else 0)
        for batch_index in range(num_batches):
            W_update = np.zeros_like(self.W)
            T_update = np.zeros(1)
            for x, e in zip(X, E):
                y = self.predict(x)
                W_update += (y - e) * x
                T_update += (y - e)
            self.W += -self.adaptive_batch_alpha(X, E) * W_update
            self.T += self.adaptive_batch_alpha(X, E) * T_update
    def test(self,epoch, X, E):
        values = []
        right = 0
        error = np.mean([self.MSE(x, e) for x, e in zip(X, E)])
        for x, e in zip(X, E):
            if e == self.predict(x): right += 1
            values.append(self.predict(x))
        self.acuracy = right/(self.n_samples*2)*100
        print(f"Epoch: {epoch}: e={error}")
        return values
def collect_results(model_name, model, epochs, start_time):
    results = {
        "Model": model_name,
        "Test Error": model.errors[-1],  # Last error value
        "Epochs": epochs,
        "Training Time (s)": time.time() - start_time,  # Time in seconds
        "Right Answers": model.acuracy
    }
    return results
results_table = []
lin_model = NN(alpha=0.05,n_samples = 100,noise_ratio = 0.1)
lin_model_adapt = NN(alpha=0.05,n_samples = 100,noise_ratio = 0.1)
batch_model = NN(alpha=0.05,n_samples = 100,noise_ratio = 0.1)
batch_model_adapt = NN(alpha=0.05,n_samples = 100,noise_ratio = 0.1)
x,y = lin_model.create_dataset()
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
results_table.append(collect_results("Batch Model Adaptive", batch_model_adapt, last_epoch, start_time))
df = pd.DataFrame(results_table)
print("\nTraining Results Table:")
print(df)