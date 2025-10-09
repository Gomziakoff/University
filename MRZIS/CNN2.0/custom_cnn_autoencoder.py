# custom_cnn_autoencoder_train.py
import torch
import pickle
import matplotlib.pyplot as plt
import numpy as np
import struct
from tqdm import tqdm
from utils.custom_cnn_autoencoder_model import ConvAutoencoder


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


def train_autoencoder(model, train_data, test_data, epochs, lrs, bs, alpha=0.5):
    X_train, y_train = train_data
    X_test, y_test = test_data
    history = []

    for epoch in range(epochs):
        lr = lrs[epoch] if isinstance(lrs, (list, torch.Tensor)) else lrs
        print(f"Epoch {epoch + 1}/{epochs}, lr={lr:.4f}")

        permutation = torch.randperm(X_train.size(0))
        X_train = X_train[permutation]
        y_train = y_train[permutation]

        total_recon_loss = 0.0
        total_class_loss = 0.0
        for i in tqdm(range(0, X_train.size(0), bs), desc="Training"):
            X_batch = X_train[i:i + bs]
            y_batch = y_train[i:i + bs]

            recon_loss = model.forward_propagation(X_batch, mode="train")
            class_loss = torch.nn.functional.cross_entropy(model.logits, y_batch)

            model.back_propagation(lr, y=y_batch, alpha=alpha)

            total_recon_loss += recon_loss
            total_class_loss += class_loss.item()

        # Evaluation
        train_recon, train_class = evaluate_autoencoder(model, (X_train, y_train))
        test_recon, test_class = evaluate_autoencoder(model, (X_test, y_test))
        history.append((train_recon, train_class, test_recon, test_class))

        print(f"Train Recon: {train_recon:.4f}, Train Class: {train_class:.4f}")
        print(f"Test Recon: {test_recon:.4f}, Test Class: {test_class:.4f}\n")

        # Save model
        with open(f"model_ae_class_{epoch + 1}.pkl", "wb") as f:
            pickle.dump(model.extract_model(), f)

    return torch.tensor(history).T


def evaluate_autoencoder(model, data, batch_size=256):
    X, y = data
    model_copy = model.extract_model()
    total_recon = 0.0
    total_class = 0.0
    num_batches = (len(X) + batch_size - 1) // batch_size

    with torch.no_grad():
        for i in tqdm(range(num_batches), desc="Evaluating"):
            X_batch = X[i * batch_size: (i + 1) * batch_size]
            y_batch = y[i * batch_size: (i + 1) * batch_size]

            output, logits = model_copy.forward_propagation(X_batch, mode="test")

            recon_loss = torch.mean((X_batch - output) ** 2).item()
            class_loss = torch.nn.functional.cross_entropy(logits, y_batch).item()

            total_recon += recon_loss * X_batch.size(0)
            total_class += class_loss * X_batch.size(0)

    avg_recon = total_recon / len(X)
    avg_class = total_class / len(X)
    return avg_recon, avg_class


def evaluate_classifier(model, X, y, batch_size=256):
    model_copy = model.extract_model()
    correct = 0
    total = 0
    num_batches = (len(X) + batch_size - 1) // batch_size

    with torch.no_grad():
        for i in tqdm(range(num_batches), desc="Classifying"):
            X_batch = X[i * batch_size: (i + 1) * batch_size]
            y_batch = y[i * batch_size: (i + 1) * batch_size]

            _, logits = model_copy.forward_propagation(X_batch, mode="test")
            preds = torch.argmax(logits, dim=1)

            correct += (preds == y_batch).sum().item()
            total += y_batch.size(0)

    return correct / total * 100


def visualize_reconstructions(model, data):
    X, _ = data
    with torch.no_grad():
        output, _ = model.forward_propagation(X[:10], mode="test")

    plt.figure(figsize=(10, 4))
    for i in range(10):
        plt.subplot(2, 10, i + 1)
        plt.imshow(X[i].squeeze(), cmap='gray')
        plt.axis('off')

        plt.subplot(2, 10, i + 11)
        plt.imshow(output[i].squeeze(), cmap='gray')
        plt.axis('off')
    plt.tight_layout()
    plt.show()


def load_and_test_model(model_path, test_data, batch_size=256):
    """Загрузка и тестирование сохраненной модели"""
    # Загрузка модели
    with open(model_path, "rb") as f:
        model = pickle.load(f)

    X_test, y_test = test_data

    # Проверка реконструкции
    test_recon, test_class = evaluate_autoencoder(model, (X_test, y_test))
    print(f"\n[Model: {model_path}]")
    print(f"Reconstruction Loss: {test_recon:.4f}")
    print(f"Classification Loss: {test_class:.4f}")

    # Проверка точности классификации
    accuracy = evaluate_classifier(model, X_test, y_test)
    print(f"Classification Accuracy: {accuracy:.2f}%")

    # Визуализация реконструкций
    visualize_reconstructions(model, (X_test, y_test))

    return test_recon, test_class, accuracy

def main():
    (X_train, y_train), (X_test, y_test) = load_data()
    model = ConvAutoencoder()
    load_and_test_model("model_ae_class_1.pkl", (X_test, y_test))

    epochs = 1
    lrs = torch.linspace(1e-2, 1e-4, steps=epochs)
    history = train_autoencoder(model, (X_train, y_train), (X_test, y_test), epochs, lrs, bs=128, alpha=0.5)

    # Plot losses
    plt.figure(figsize=(12, 5))
    plt.subplot(1, 2, 1)
    plt.plot(history[0], label="Train Recon")
    plt.plot(history[2], label="Test Recon")
    plt.legend()
    plt.title("Reconstruction Loss")

    plt.subplot(1, 2, 2)
    plt.plot(history[1], label="Train Class")
    plt.plot(history[3], label="Test Class")
    plt.legend()
    plt.title("Classification Loss")
    plt.show()

    # Visualize reconstructions
    visualize_reconstructions(model.extract_model(), (X_test, y_test))

    # Evaluate classifier
    accuracy = evaluate_classifier(model, X_test, y_test)
    print(f"Test Accuracy: {accuracy:.2f}%")


if __name__ == "__main__":
    main()