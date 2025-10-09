import torch

SEQUENCE_LENGTH = 10
HIDDEN_SIZE = 64
BATCH_SIZE = 32
NUM_EPOCHS = 100
MANUAL_LEARNING_RATE = 0.1
LEARNING_RATE = 0.001
INPUT_SIZE = 1
OUTPUT_SIZE = 1
DEVICE = torch.device("cuda" if torch.cuda.is_available() else "cpu")
COLORS = ['#1f77b4', '#ff7f0e', '#2ca02c', '#d62728']

