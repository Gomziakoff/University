import matplotlib.pyplot as plt
from sklearn.metrics import mean_squared_error
from utils import load_data, preprocess_data
from autoencoder import Autoencoder
from tabulate import tabulate

if __name__ == "__main__":
    data = load_data('abalone.csv')
    x_train, x_test = preprocess_data(data)
    optimizers = ["oja", "cumulative", "sgd", "adam"]
    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(14, 6))
    results = []

    for opt in optimizers:
        print(f"Training with {opt} optimizer")
        autoencoder = Autoencoder(input_dim=x_train.shape[1], encoding_dim=5, learning_rate=0.03, alpha=0.9, optimizer_type=opt)
        autoencoder.train(x_train, x_test, epochs=1000)

        x_train_encoded = autoencoder.encode_data(x_train)
        x_train_decoded = autoencoder.decode_data(x_train_encoded)

        x_test_encoded = autoencoder.encode_data(x_test)
        x_test_decoded = autoencoder.decode_data(x_test_encoded)

        train_mse = mean_squared_error(x_train, x_train_decoded)
        test_mse = mean_squared_error(x_test, x_test_decoded)

        results.append([opt, train_mse, test_mse])

        print(f"Train Mean Squared Error ({opt}): {train_mse:.4f}")
        print(f"Test Mean Squared Error ({opt}): {test_mse:.4f}")

        ax1.plot(autoencoder.train_errors, label=f"Train ({opt})")
        ax2.plot(autoencoder.test_errors, label=f"Test ({opt})")

    headers = ["Optimizer", "Train MSE", "Test MSE"]
    print("\nResults Summary:")
    print(tabulate(results, headers=headers, tablefmt="pretty", floatfmt=".4f"))

    ax1.set_xlabel("Epochs")
    ax1.set_ylabel("Mean Squared Error")
    ax1.set_title("Training Error Comparison")
    ax1.legend()

    ax2.set_xlabel("Epochs")
    ax2.set_ylabel("Mean Squared Error")
    ax2.set_title("Testing Error Comparison")
    ax2.legend()

    plt.tight_layout()
    plt.show()