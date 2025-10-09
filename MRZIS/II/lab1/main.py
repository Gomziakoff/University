import time
import torch
import matplotlib.pyplot as plt
from sklearn.metrics import mean_squared_error
from pca import PCA
from autoencoder import Autoencoder, train_autoencoder
from utils import load_data, preprocess_data, create_dataloader

X, y = load_data('WineQT.csv')
X_train, X_test, y_train, y_test = preprocess_data(X, y)
print(X_train.shape, X_test.shape)

# PCA
start_time = time.time()
pca = PCA(n_components=2)
X_train_pca = pca.fit_transform(X_train)
X_test_pca = pca.transform(X_test)
X_train_reconstructed_pca = pca.inverse_transform(X_train_pca)
X_test_reconstructed_pca = pca.inverse_transform(X_test_pca)
pca_time = time.time() - start_time

pca_mse_train = mean_squared_error(X_train, X_train_reconstructed_pca)
pca_mse_test = mean_squared_error(X_test, X_test_reconstructed_pca)

plt.scatter(X_train_pca[:, 0], X_train_pca[:, 1], c=y_train, cmap='viridis')
plt.title('PCA: 2 компоненты')
plt.colorbar(label='Качество вина')
plt.show()

# автоэнкодер
train_loader = create_dataloader(X_train)
input_dim = X_train.shape[1]
encoding_dim = 2
model = Autoencoder(input_dim, encoding_dim)

start_time = time.time()
train_autoencoder(model, train_loader)
autoencoder_time = time.time() - start_time

with torch.no_grad():
    X_train_tensor = torch.tensor(X_train, dtype=torch.float32)
    X_test_tensor = torch.tensor(X_test, dtype=torch.float32)
    X_train_encoded = model.encoder(X_train_tensor).numpy()
    X_test_encoded = model.encoder(X_test_tensor).numpy()
    X_train_reconstructed_ae = model(X_train_tensor).numpy()
    X_test_reconstructed_ae = model(X_test_tensor).numpy()

ae_mse_train = mean_squared_error(X_train, X_train_reconstructed_ae)
ae_mse_test = mean_squared_error(X_test, X_test_reconstructed_ae)

plt.scatter(X_train_encoded[:, 0], X_train_encoded[:, 1], c=y_train, cmap='viridis')
plt.title('Автоэнкодер: 2 компоненты')
plt.colorbar(label='Качество вина')
plt.show()

# Вывод результатов
print(f'PCA Time: {pca_time:.4f}s, PCA MSE (Train): {pca_mse_train:.4f}, PCA MSE (Test): {pca_mse_test:.4f}')
print(f'Autoencoder Time: {autoencoder_time:.4f}s, Autoencoder MSE (Train): {ae_mse_train:.4f}, Autoencoder MSE (Test): {ae_mse_test:.4f}')