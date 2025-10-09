import torch
import torch.nn as nn
import torch.optim as optim
from torch.optim import Optimizer
from collections import defaultdict

class CumulativeSGDOptimizer(Optimizer):
    def __init__(self, params, lr=0.01, alpha=0.0):
        defaults = dict(lr=lr, alpha=alpha)
        super().__init__(params, defaults)
        self.state = defaultdict(dict)

    def step(self, closure=None):
        for group in self.param_groups:
            lr = group['lr']
            alpha = group['alpha']
            for p in group['params']:
                if p.grad is None:
                    continue
                state = self.state[p]

                if 'prev_delta' not in state:
                    state['prev_delta'] = torch.zeros_like(p.data)

                delta = lr * p.grad.data + alpha * state['prev_delta']
                p.data.add_(-delta)
                state['prev_delta'] = delta.clone()

class OjaOptimizerV2(Optimizer):
    def __init__(self, encoder_params, decoder_params, lr=0.01, weight_decay=0.0):
        defaults = dict(lr=lr, weight_decay=weight_decay)
        params = [
            {'params': encoder_params, 'is_encoder': True},
            {'params': decoder_params, 'is_encoder': False}
        ]
        super().__init__(params, defaults)

    def step(self, closure=None):
        for group in self.param_groups:
            lr = group['lr']
            weight_decay = group['weight_decay']
            is_encoder = group.get('is_encoder', False)

            for p in group['params']:
                if p.grad is None:
                    continue

                if is_encoder:
                    if hasattr(p, 'oja_x') and hasattr(p, 'oja_y'):
                        x = p.oja_x.detach()  # [batch_size, input_dim]
                        y = p.oja_y.detach()  # [batch_size, encoding_dim]

                        for i in range(p.data.size(0)):  # For each neuron
                            wy = y[:, i].unsqueeze(1) * p.data[i]
                            delta = (y[:, i].unsqueeze(1) * (x - wy)).mean(dim=0)
                            p.data[i] += lr * delta

                            if weight_decay > 0:
                                p.data[i].mul_(1 - lr * weight_decay)
                            norm = p.data[i].norm()
                            p.data[i].div_(max(norm, 1e-8))
                else:
                    if p.grad is not None:
                        p.data.add_(-lr * p.grad.data)
                        if weight_decay > 0:
                            p.data.add_(-lr * weight_decay, p.data)


class AutoencoderV2(nn.Module):
    def __init__(self, input_dim, encoding_dim, learning_rate, alpha=0.0, optimizer_type="cumulative"):
        super().__init__()
        self.encoder = nn.Linear(input_dim, encoding_dim)
        self.decoder = nn.Linear(encoding_dim, input_dim)
        self.sigmoid = nn.Sigmoid()
        self.optimizer_type = optimizer_type
        self.use_linear_encoder = (optimizer_type == "oja")

        if optimizer_type == "oja":
            self.optimizer = OjaOptimizerV2(
                encoder_params=[self.encoder.weight, self.encoder.bias],
                decoder_params=[self.decoder.weight, self.decoder.bias],
                lr=learning_rate
            )
        elif optimizer_type == "cumulative":
            self.optimizer = CumulativeSGDOptimizer(
                self.parameters(), lr=learning_rate, alpha=alpha
            )
        elif optimizer_type == "sgd":
            self.optimizer = optim.SGD(self.parameters(), lr=learning_rate)
        elif optimizer_type == "adam":
            self.optimizer = optim.Adam(self.parameters(), lr=learning_rate)

        self.criterion = nn.MSELoss()
        self.train_errors = []
        self.test_errors = []

    def forward(self, x):
        if self.optimizer_type == "oja":
            # Store linear activations for Oja's rule
            self.encoder.weight.oja_x = x
            encoded_linear = self.encoder(x)
            self.encoder.weight.oja_y = encoded_linear
            encoded = self.sigmoid(encoded_linear)
        else:
            encoded = self.sigmoid(self.encoder(x))

        decoded = self.sigmoid(self.decoder(encoded))
        return decoded

    def train_model(self, train_loader, test_loader, epochs=10):
        for epoch in range(epochs):
            # Training
            self.train()
            train_loss = 0.0
            for data in train_loader:
                inputs = data[0]
                self.optimizer.zero_grad()
                outputs = self(inputs)
                loss = self.criterion(outputs, inputs)
                loss.backward()
                self.optimizer.step()
                train_loss += loss.item()

            self.eval()
            test_loss = 0.0
            with torch.no_grad():
                for data in test_loader:
                    inputs = data[0]
                    outputs = self(inputs)
                    test_loss += self.criterion(outputs, inputs).item()

            self.train_errors.append(train_loss / len(train_loader))
            self.test_errors.append(test_loss / len(test_loader))

            if (epoch + 1) % 100 == 0:
                print(
                    f"Epoch {epoch + 1}, Train Loss: {self.train_errors[-1]:.4f}, Test Loss: {self.test_errors[-1]:.4f}")

    def encode_data(self, inputs):
        with torch.no_grad():
            return self.sigmoid(self.encoder(inputs)).numpy()

    def decode_data(self, encoded):
        with torch.no_grad():
            return self.sigmoid(self.decoder(encoded)).numpy()
