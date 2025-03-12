from config import *
import torch

class GRUModel(torch.nn.Module):
    def __init__(self):
        super().__init__()
        self.gru = torch.nn.GRU(1, HIDDEN_SIZE, batch_first=True)
        self.fc = torch.nn.Linear(HIDDEN_SIZE, 1)

    def forward(self, x):
        out, _ = self.gru(x)
        return self.fc(out[:, -1])