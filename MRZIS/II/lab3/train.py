from tqdm import tqdm
import torch.optim as optim
from config import *
from tabulate import tabulate
from data.preparation import generate_data
from models.manual_rnn import ManualRNN
from models.rnn import RNNModel
from models.gru import GRUModel
from models.lstm import LSTMModel
from utils.metrics import calculate_metrics
from utils.visualization import plot_predictions, plot_learning_curves


def train_manual_model(model, train_loader, test_loader):
    metrics = {
        'train_loss': [], 'test_loss': [],
        'train_mae': [], 'test_mae': []
    }

    for epoch in tqdm(range(NUM_EPOCHS)):
        # Training phase
        epoch_loss, epoch_mae, epoch_mape = 0, 0, 0
        for seq, tgt in train_loader:
            # Forward pass
            output = model.forward(seq)

            # Calculate loss
            loss = torch.mean((output - tgt) ** 2)
            epoch_loss += loss.item()

            # Backward pass and update
            dL_dy = 2 * (output - tgt) / seq.size(0)
            model.backward(dL_dy, MANUAL_LEARNING_RATE)
            model.zero_grad()

            # Calculate metrics
            with torch.no_grad():
                mse = loss.item()
                mae = torch.mean(torch.abs(output - tgt)).item()
                mape = torch.mean(torch.abs((tgt - output) / tgt)).item() * 100
                epoch_mae += mae
                epoch_mape += mape

        # Validation phase
        test_loss, test_mae, test_mape = 0, 0, 0
        with torch.no_grad():
            for seq, tgt in test_loader:
                output = model.forward(seq)
                loss = torch.mean((output - tgt) ** 2)

                test_loss += loss.item()
                test_mae += torch.mean(torch.abs(output - tgt)).item()
                test_mape += torch.mean(torch.abs((tgt - output) / tgt)).item() * 100

        # Save metrics
        metrics['train_loss'].append(epoch_loss / len(train_loader))
        metrics['test_loss'].append(test_loss / len(test_loader))
        metrics['train_mae'].append(epoch_mae / len(train_loader))
        metrics['test_mae'].append(test_mae / len(test_loader))

    return metrics


def train_torch_model(model, train_loader, test_loader):
    model.to(DEVICE)
    optimizer = optim.Adam(model.parameters(), lr=LEARNING_RATE)

    metrics = {
        'train_loss': [], 'test_loss': [],
        'train_mae': [], 'test_mae': [],
    }

    for epoch in tqdm(range(NUM_EPOCHS)):
        model.train()
        epoch_loss, epoch_mae, epoch_mape = 0, 0, 0
        for seq, tgt in train_loader:
            seq, tgt = seq.to(DEVICE), tgt.to(DEVICE)
            optimizer.zero_grad()
            outputs = model(seq)
            loss = torch.nn.functional.mse_loss(outputs, tgt)
            loss.backward()
            optimizer.step()

            mse, mae, mape = calculate_metrics(outputs, tgt)
            epoch_loss += mse
            epoch_mae += mae
            epoch_mape += mape
        model.eval()
        test_loss, test_mae, test_mape = 0, 0, 0
        with torch.no_grad():
            for seq, tgt in test_loader:
                seq, tgt = seq.to(DEVICE), tgt.to(DEVICE)
                outputs = model(seq)
                mse, mae, mape = calculate_metrics(outputs, tgt)
                test_loss += mse
                test_mae += mae
                test_mape += mape

        metrics['train_loss'].append(epoch_loss / len(train_loader))
        metrics['test_loss'].append(test_loss / len(test_loader))
        metrics['train_mae'].append(epoch_mae / len(train_loader))
        metrics['test_mae'].append(test_mae / len(test_loader))

    return metrics


def main():
    train_loader, test_loader = generate_data()

    manual_rnn = ManualRNN(
        input_size=INPUT_SIZE,
        hidden_size=HIDDEN_SIZE,
        output_size=OUTPUT_SIZE
    )

    models = {
        "ManualRNN": (manual_rnn, train_manual_model),
        "RNN": (RNNModel(), train_torch_model),
        "GRU": (GRUModel(), train_torch_model),
        "LSTM": (LSTMModel(), train_torch_model)
    }

    results = []

    for name, (model, trainer) in models.items():
        print(f"\nTraining {name}...")
        metrics = trainer(model, train_loader, test_loader)

        # Generate predictions plot
        actuals, predictions = [], []
        with torch.no_grad():
            for seq, tgt in test_loader:
                outputs = model.forward(seq).numpy() if name == "ManualRNN" \
                    else model(seq.to(DEVICE)).cpu().numpy()
                predictions.extend(outputs.squeeze())
                actuals.extend(tgt.numpy().squeeze())
        plot_predictions(name, actuals, predictions)

        # Generate learning curves
        plot_learning_curves(name, metrics)

        # Collect results
        results.append([
            name,
            metrics['test_loss'][-1],
            metrics['test_mae'][-1],
        ])

    # Print results table
    headers = ["Model", "Test MSE", "Test MAE", "Test MAPE (%)"]
    print("\n" + "=" * 60)
    print("Final Results Comparison".center(60))
    print("=" * 60)
    print(tabulate(results,
                   headers=headers,
                   tablefmt="grid",
                   floatfmt=(".4f", ".4f", ".2f")))

    # Save to CSV
    with open("results.csv", "w") as f:
        f.write(tabulate(results, headers=headers, tablefmt="csv"))


if __name__ == "__main__":
    main()