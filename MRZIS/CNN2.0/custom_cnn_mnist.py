import torch
import pickle
import matplotlib.pyplot as plt
import time
import struct
from tqdm import tqdm
from utils.custom_cnn_model import CustomCNN
import numpy as np


def evaluate(model, X, y, batch_size=256):
    model_copy = model.extract_model()
    total_errors = 0
    num_samples = X.size(0)  # Общее количество примеров
    num_batches = (num_samples + batch_size - 1) // batch_size

    with torch.no_grad():
        for i in range(num_batches):
            start_idx = i * batch_size
            end_idx = min((i + 1) * batch_size, num_samples)  # Защита от выхода за пределы
            X_batch = X[start_idx:end_idx]
            y_batch = y[start_idx:end_idx]

            errors, _ = model_copy.forward_propagation(X_batch, y_batch, "test")
            total_errors += errors

    # Возвращаем среднюю ошибку (процент)
    return total_errors / num_samples

def read_dataset(paths):
    images_path, labels_path = paths
    with open(labels_path, "rb") as f:
        magic, n = struct.unpack(">II", f.read(8))
        labels = torch.tensor(np.frombuffer(f.read(), dtype=np.uint8), dtype=torch.long)
    with open(images_path, "rb") as f:
        magic, n, h, w = struct.unpack(">IIII", f.read(16))
        images = torch.tensor(np.frombuffer(f.read(), dtype=np.uint8), dtype=torch.float32).view(-1, h, w)
    return images, labels


def normalize(images):
    images = (images - images.min()) / (images.max() - images.min())
    return (images - images.mean()) / images.std()


def load_data():
    train = read_dataset(("dataset/MNIST/train-images-idx3-ubyte", "dataset/MNIST/train-labels-idx1-ubyte"))
    test = read_dataset(("dataset/MNIST/t10k-images-idx3-ubyte", "dataset/MNIST/t10k-labels-idx1-ubyte"))
    return (normalize(train[0]).unsqueeze(3), train[1]), (normalize(test[0]).unsqueeze(3), test[1])


def train_model(model, train_data, test_data, epochs, lrs, bs):
    X_train, y_train = train_data
    X_test, y_test = test_data
    history = []

    for epoch in range(epochs):
        lr = lrs[epoch]
        print(f"Epoch {epoch + 1}/{epochs}, lr={lr:.4f}")

        permutation = torch.randperm(X_train.size(0))
        X_train = X_train[permutation]
        y_train = y_train[permutation]

        total_loss = 0.0
        for i in tqdm(range(0, X_train.size(0), bs)):
            X_batch = X_train[i:i + bs]
            y_batch = y_train[i:i + bs]

            loss = model.forward_propagation(X_batch, y_batch, "train")
            total_loss += loss
            model.back_propagation(lr)

        train_err = evaluate(model, X_train, y_train)
        test_err = evaluate(model, X_test, y_test)
        history.append((train_err, test_err))

        print(f"Loss: {total_loss:.2f}, Train Err: {train_err}, Test Err: {test_err}")

        with open(f"model_data_cnn/model_{epoch + 1}.pkl", "wb") as f:
            pickle.dump(model.extract_model(), f)

    return torch.tensor(history).T


def test_model(model_path, test_data, batch_size=256, show_examples=True):
    """
    Тестирование модели из файла с визуализацией примеров
    """
    # Загрузка модели
    with open(model_path, "rb") as f:
        model = pickle.load(f)

    X_test, y_test = test_data
    total_samples = len(X_test)
    total_errors = 0

    # Прогон через модель
    with torch.no_grad():
        # Вычисление ошибок
        num_batches = (total_samples + batch_size - 1) // batch_size
        for i in tqdm(range(num_batches), desc="Testing"):
            start = i * batch_size
            end = min(start + batch_size, total_samples)

            batch_images = X_test[start:end]
            batch_labels = y_test[start:end]

            errors, _ = model.forward_propagation(batch_images, batch_labels, "test")
            total_errors += errors


        # Визуализация примеров
        if show_examples:
            visualize_predictions(model, test_data)

    # Вывод результатов
    accuracy = (1 - total_errors / total_samples) * 100
    print(f"\nResults for model: {model_path}")
    print(f"Test samples: {total_samples}")
    print(f"Errors: {total_errors}")
    print(f"Accuracy: {accuracy:.2f}%")

    return accuracy


def visualize_predictions(model, test_data, num_examples=10):
    """
    Визуализация предсказаний модели
    """
    indices = torch.randperm(len(test_data[0]))[:num_examples]
    images = test_data[0][indices]
    labels = test_data[1][indices]

    with torch.no_grad():
        _, preds = model.forward_propagation(images, labels, "test")

    plt.figure(figsize=(15, int(num_examples / 2) * 3))
    for i in range(num_examples):
        plt.subplot(int(num_examples / 5) + 1, 5, i + 1)
        img = images[i].squeeze().numpy()
        plt.imshow(img, cmap='gray')
        plt.title(f"True: {labels[i]} | Pred: {preds[i]}")
        plt.axis('off')
    plt.tight_layout()
    plt.show()


def main():
    (X_train, y_train), (X_test, y_test) = load_data()
    model = CustomCNN()
    test_model("model_8.pkl", (X_test,y_test))
    epochs = 20
    lrs = torch.cat([
        torch.full((2,), 5e-2),
        torch.full((3,), 2e-2),
        torch.full((3,), 1e-2),
        torch.full((4,), 5e-3),
        torch.full((4,), 1e-3),
        torch.full((4,), 5e-4),
    ])

    history = train_model(model, (X_train, y_train), (X_test, y_test), epochs, lrs, bs=8)

    plt.plot(history[0], label="Train Error")
    plt.plot(history[1], label="Test Error")
    plt.legend()
    plt.show()


if __name__ == "__main__":
    main()