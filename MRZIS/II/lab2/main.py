import matplotlib.pyplot as plt
from sklearn.metrics import mean_squared_error
from utils import load_data, preprocess_data
from autoencoder import AutoencoderV2
from tabulate import tabulate
import torch
from torch.utils.data import TensorDataset, DataLoader

if __name__ == "__main__":
    # Загрузка и предобработка данных
    data = load_data('abalone.csv')
    x_train, x_test = preprocess_data(data)

    # Конвертация в тензоры и создание DataLoader
    x_train_tensor = torch.tensor(x_train, dtype=torch.float32)
    x_test_tensor = torch.tensor(x_test, dtype=torch.float32)

    train_loader = DataLoader(TensorDataset(x_train_tensor), batch_size=len(x_train_tensor), shuffle=True)
    test_loader = DataLoader(TensorDataset(x_test_tensor), batch_size=len(x_test_tensor), shuffle=False)

    optimizers = ["oja", "cumulative", "sgd", "adam"]
    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(14, 6))
    results = []

    for opt in optimizers:
        print(f"\nTraining with {opt} optimizer")
        autoencoder = AutoencoderV2(
            input_dim=x_train.shape[1],
            encoding_dim=5,
            learning_rate=0.1,
            alpha=0.9,
            optimizer_type=opt
        )
        # Обучение модели
        autoencoder.train_model(train_loader, test_loader, epochs=1000)

        # Предсказания
        with torch.no_grad():
            decoded_train = autoencoder(x_train_tensor).numpy()
            decoded_test = autoencoder(x_test_tensor).numpy()

        # Расчет ошибок
        train_mse = mean_squared_error(x_train, decoded_train)
        test_mse = mean_squared_error(x_test, decoded_test)
        results.append([opt, train_mse, test_mse])

        # Визуализация ошибок
        ax1.plot(autoencoder.train_errors, label=f"Train ({opt})")
        ax2.plot(autoencoder.test_errors, label=f"Test ({opt})")

    # Вывод результатов
    headers = ["Optimizer", "Train MSE", "Test MSE"]
    print("\nResults Summary:")
    print(tabulate(results, headers=headers, tablefmt="pretty", floatfmt=".4f"))

    # Настройка графиков
    ax1.set_xlabel("Epochs")
    ax1.set_ylabel("MSE")
    ax1.set_title("Training MSE")
    ax1.legend()

    ax2.set_xlabel("Epochs")
    ax2.set_ylabel("MSE")
    ax2.set_title("Testing MSE")
    ax2.legend()

    plt.tight_layout()
    plt.show()