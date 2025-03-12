from config import *
import numpy as np
import torch
from torch.utils.data import DataLoader, TensorDataset


def generate_data():
    x = np.arange(0, 20 * np.pi, 0.1)
    y = np.cos(4 * x) + 2 * np.sin(3 * x)
    y_tensor = torch.FloatTensor(y).view(-1, 1)

    # Create sequences
    sequences = torch.stack([
        y_tensor[i:i + SEQUENCE_LENGTH]
        for i in range(len(y) - SEQUENCE_LENGTH)
    ])
    targets = torch.stack([
        y_tensor[i + SEQUENCE_LENGTH]
        for i in range(len(y) - SEQUENCE_LENGTH)
    ])

    # Split data
    train_size = int(0.8 * len(sequences))
    train_dataset = TensorDataset(sequences[:train_size], targets[:train_size])
    test_dataset = TensorDataset(sequences[train_size:], targets[train_size:])

    return (
        DataLoader(train_dataset, batch_size=BATCH_SIZE, shuffle=True),
        DataLoader(test_dataset, batch_size=BATCH_SIZE, shuffle=False)
    )