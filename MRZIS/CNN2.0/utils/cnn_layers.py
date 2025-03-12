import torch


def zero_pad(X, pad):
    if pad == 0:
        return X
    padding = (0, 0, pad, pad, pad, pad)
    return torch.nn.functional.pad(X, padding, mode='constant', value=0)


def initialise(kernel_shape, sigma=0.1, bias_factor=0.01):
    if len(kernel_shape) == 4:
        bias_shape = (1, 1, 1, kernel_shape[-1])
    else:
        bias_shape = (kernel_shape[-1],)
    weight = torch.randn(kernel_shape) * sigma
    bias = torch.ones(bias_shape) * bias_factor
    weight.requires_grad_(False)
    bias.requires_grad_(False)
    return weight, bias


def softmax(X):
    X_exp = torch.exp(X)
    return X_exp / X_exp.sum(dim=1, keepdim=True)


class Conv_Layer:
    def __init__(self, kernel_shape, stride=1, pad=0, sigma=0.1, bias_factor=0.01):
        self.weight, self.bias = initialise(kernel_shape, sigma, bias_factor)
        self.stride = stride
        self.pad = pad

    def forward_propagation(self, input_map):
        self.input_map = input_map
        batch_size, h_in, w_in, _ = input_map.shape
        f, _, _, c_out = self.weight.shape
        h_out = (h_in + 2 * self.pad - f) // self.stride + 1
        w_out = (w_in + 2 * self.pad - f) // self.stride + 1

        output = torch.zeros((batch_size, h_out, w_out, c_out))
        input_pad = zero_pad(input_map, self.pad)

        for h in range(h_out):
            for w in range(w_out):
                h_start = h * self.stride
                h_end = h_start + f
                w_start = w * self.stride
                w_end = w_start + f

                input_slice = input_pad[:, h_start:h_end, w_start:w_end, :]
                output[:, h, w, :] = torch.tensordot(
                    input_slice, self.weight, dims=([1, 2, 3], [0, 1, 2])) + self.bias
        return output

    def back_propagation(self, dout, lr):
        f = self.weight.shape[0]
        batch, h_out, w_out, c_out = dout.shape
        d_input = torch.zeros_like(self.input_map)
        input_pad = zero_pad(self.input_map, self.pad)
        d_input_pad = zero_pad(d_input, self.pad)
        d_weight = torch.zeros_like(self.weight)
        d_bias = torch.zeros_like(self.bias)

        for h in range(h_out):
            for w in range(w_out):
                h_start = h * self.stride
                h_end = h_start + f
                w_start = w * self.stride
                w_end = w_start + f

                input_slice = input_pad[:, h_start:h_end, w_start:w_end, :]
                dout_slice = dout[:, h, w, :]

                d_input_pad[:, h_start:h_end, w_start:w_end, :] += torch.tensordot(
                    dout_slice, self.weight, dims=([1], [3]))

                d_weight += torch.tensordot(
                    input_slice, dout_slice, dims=([0], [0]))

                d_bias += dout_slice.sum(dim=0)

        if self.pad != 0:
            d_input = d_input_pad[:, self.pad:-self.pad, self.pad:-self.pad, :]
        else:
            d_input = d_input_pad

        self.weight -= lr * d_weight
        self.bias -= lr * d_bias.squeeze()[None, None, None, :]
        return d_input


class ReLU_Layer:
    def forward_propagation(self, X):
        self.input = X
        return torch.relu(X)

    def back_propagation(self, dout):
        return dout * (self.input > 0).float()


class MaxPool_Layer:
    def __init__(self, stride=2, f=2):
        self.stride = stride
        self.f = f

    def forward_propagation(self, X):
        self.input = X
        batch, h_in, w_in, c = X.shape
        h_out = (h_in - self.f) // self.stride + 1
        w_out = (w_in - self.f) // self.stride + 1

        output = torch.zeros((batch, h_out, w_out, c))
        for h in range(h_out):
            for w in range(w_out):
                h_start = h * self.stride
                h_end = h_start + self.f
                w_start = w * self.stride
                w_end = w_start + self.f

                input_slice = X[:, h_start:h_end, w_start:w_end, :]
                output[:, h, w, :] = torch.amax(input_slice, dim=(1, 2))
        return output

    def back_propagation(self, dout):
        batch, h_out, w_out, c = dout.shape
        d_input = torch.zeros_like(self.input)

        for h in range(h_out):
            for w in range(w_out):
                h_start = h * self.stride
                h_end = h_start + self.f
                w_start = w * self.stride
                w_end = w_start + self.f

                input_slice = self.input[:, h_start:h_end, w_start:w_end, :]
                max_vals = torch.amax(input_slice, dim=(1, 2), keepdim=True)
                mask = (input_slice == max_vals).float()
                d_input[:, h_start:h_end, w_start:w_end, :] += (
                        dout[:, h, w, :][:, None, None, :] * mask)
        return d_input


class FC_Layer:
    def __init__(self, weight_shape, sigma=0.1, bias_factor=0.01):
        self.weight, self.bias = initialise(weight_shape, sigma, bias_factor)

    def forward_propagation(self, X):
        self.input = X
        return X @ self.weight + self.bias

    def back_propagation(self, dout, lr):
        d_input = dout @ self.weight.T
        d_weight = self.input.T @ dout
        d_bias = dout.sum(dim=0)

        self.weight -= lr * d_weight
        self.bias -= lr * d_bias
        return d_input


class FC_Output_Layer:
    def __init__(self, weight_shape, sigma=0.1, bias_factor=0.01):
        self.weight, self.bias = initialise(weight_shape, sigma, bias_factor)

    def forward_propagation(self, X, labels, mode):
        self.input = X
        self.labels = labels
        logits = X @ self.weight + self.bias
        probs = softmax(logits)

        if mode == "train":
            loss = -torch.log(probs[torch.arange(X.shape[0]), labels]).sum()
            return loss.item()
        elif mode == "test":
            preds = torch.argmax(probs, dim=1)
            errors = (labels != preds).sum().item()
            return errors, preds

    def back_propagation(self, lr):
        probs = softmax(self.input @ self.weight + self.bias)
        probs[torch.arange(probs.shape[0]), self.labels] -= 1
        dout = probs / probs.shape[0]

        d_input = dout @ self.weight.T
        d_weight = self.input.T @ dout
        d_bias = dout.sum(dim=0)

        self.weight -= lr * d_weight
        self.bias -= lr * d_bias
        return d_input