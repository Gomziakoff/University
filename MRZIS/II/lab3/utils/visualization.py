from config import *
import matplotlib.pyplot as plt
import os


os.makedirs('plots', exist_ok=True)


def plot_predictions(model_name, actuals, predictions):
    plt.figure(figsize=(10, 5))
    plt.plot(actuals, label='Actual', color=COLORS[0])
    plt.plot(predictions, label='Predicted', alpha=0.7, color=COLORS[1])
    plt.title(f'{model_name} Predictions')
    plt.legend()
    plt.savefig(f'plots/{model_name}_predictions.png')
    plt.close()


def plot_learning_curves(model_name, metrics):
    plt.figure(figsize=(10, 5))

    plt.subplot(1, 2, 1)
    plt.plot(metrics['train_loss'], label='Train')
    plt.plot(metrics['test_loss'], label='Test')
    plt.title(f'{model_name} MSE Loss')
    plt.legend()

    plt.tight_layout()
    plt.savefig(f'plots/{model_name}_learning_curves.png')
    plt.close()