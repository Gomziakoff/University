import torch
from config import *


class ManualRNN:
    def __init__(self, input_size, hidden_size, output_size):
        # Инициализация с Xavier и нормальными весами
        self.W_ih = torch.randn(input_size, hidden_size)
        self.W_hh = torch.randn(hidden_size, hidden_size)
        self.W_ho = torch.randn(hidden_size, output_size)
        torch.nn.init.xavier_normal_(self.W_ih)
        torch.nn.init.xavier_normal_(self.W_hh)
        torch.nn.init.xavier_normal_(self.W_ho)

        self.b_h = torch.zeros(hidden_size)
        self.b_o = torch.zeros(output_size)

        self.cache = {
            'inputs': [],
            'hidden_states': [],
            'outputs': []
        }

    def tanh(self, x):
        return torch.clip((torch.exp(x) - torch.exp(-x)) /
                          (torch.exp(x) + torch.exp(-x)), -0.999, 0.999)

    def forward(self, x):
        self.cache = {'inputs': [], 'hidden_states': [torch.zeros(x.size(0), HIDDEN_SIZE)], 'outputs': []}

        for t in range(x.size(1)):
            x_t = x[:, t, :]
            h_prev = self.cache['hidden_states'][-1]

            pre_activation = x_t @ self.W_ih + h_prev @ self.W_hh + self.b_h
            pre_activation = torch.clip(pre_activation, -10, 10)

            h = self.tanh(pre_activation)

            self.cache['inputs'].append(x_t)
            self.cache['hidden_states'].append(h)

        output = self.cache['hidden_states'][-1] @ self.W_ho + self.b_o
        self.cache['outputs'].append(output)

        return output

    def backward(self, dL_dy, learning_rate=0.01):
        batch_size = dL_dy.shape[0]

        dW_ih = torch.zeros_like(self.W_ih)
        dW_hh = torch.zeros_like(self.W_hh)
        db_h = torch.zeros_like(self.b_h)
        dW_ho = torch.zeros_like(self.W_ho)
        db_o = torch.zeros_like(self.b_o)

        h_last = self.cache['hidden_states'][-1]
        dW_ho = h_last.T @ dL_dy / batch_size
        db_o = dL_dy.mean(dim=0)

        dL_dh = dL_dy @ self.W_ho.T

        for t in reversed(range(len(self.cache['inputs']))):
            x_t = self.cache['inputs'][t]
            h_prev = self.cache['hidden_states'][t]
            h_curr = self.cache['hidden_states'][t + 1]

            dtanh = (1 - h_curr ** 2) * dL_dh

            dW_ih += x_t.T @ dtanh / batch_size
            dW_hh += h_prev.T @ dtanh / batch_size
            db_h += dtanh.mean(dim=0)
            dL_dh = dtanh @ self.W_hh.T

            dL_dh = torch.clip(dL_dh, -1, 1)

        self.W_ih -= learning_rate * dW_ih
        self.W_hh -= learning_rate * dW_hh
        self.b_h -= learning_rate * db_h
        self.W_ho -= learning_rate * dW_ho
        self.b_o -= learning_rate * db_o

    def zero_grad(self):
        self.cache = {'inputs': [], 'hidden_states': [torch.zeros_like(self.cache['hidden_states'][0])], 'outputs': []}