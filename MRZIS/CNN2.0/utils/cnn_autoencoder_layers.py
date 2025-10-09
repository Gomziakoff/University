# cnn_autoencoder_layers.py
import torch


def zero_pad(X, pad):
    if pad == 0:
        return X
    padding = (0, 0, pad, pad, pad, pad)
    return torch.nn.functional.pad(X, padding, mode='constant', value=0)


def initialise(kernel_shape, sigma=0.1, bias_factor=0.01):
    if len(kernel_shape) == 4:
        bias_shape = (kernel_shape[-1],)
    else:
        bias_shape = (kernel_shape[-1],)

    weight = torch.randn(kernel_shape) * sigma
    bias = torch.ones(bias_shape) * bias_factor

    weight.requires_grad_(False)
    bias.requires_grad_(False)
    return weight, bias


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
        self.bias -= lr * d_bias.view(*self.bias.shape)
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


class Upsample_Layer:
    def __init__(self, scale_factor=2):
        self.scale_factor = scale_factor

    def forward_propagation(self, X):
        X_permuted = X.permute(0, 3, 1, 2)
        upsampled = torch.nn.functional.interpolate(
            X_permuted, scale_factor=self.scale_factor, mode='nearest')
        return upsampled.permute(0, 2, 3, 1)

    def back_propagation(self, dout):
        batch, h_out, w_out, c = dout.shape
        h_in = h_out // self.scale_factor
        w_in = w_out // self.scale_factor

        d_input = torch.zeros((batch, h_in, w_in, c), device=dout.device)

        for i in range(h_in):
            for j in range(w_in):
                h_start = i * self.scale_factor
                w_start = j * self.scale_factor
                d_input[:, i, j, :] = dout[:, h_start:h_start + self.scale_factor,
                                      w_start:w_start + self.scale_factor, :].sum(dim=(1, 2))
        return d_input


class Sigmoid_Layer:
    def forward_propagation(self, X):
        self.input = X
        return torch.sigmoid(X)

    def back_propagation(self, dout):
        sig = torch.sigmoid(self.input)
        return dout * sig * (1 - sig)


class Flatten_Layer:
    def forward_propagation(self, X):
        self.input_shape = X.shape
        return X.view(X.size(0), -1)

    def back_propagation(self, dout):
        return dout.view(self.input_shape)


class Linear_Layer:
    def __init__(self, input_size, output_size, sigma=0.1, bias_factor=0.01):
        self.weight = torch.randn(input_size, output_size) * sigma
        self.bias = torch.ones(output_size) * bias_factor
        self.weight.requires_grad_(False)
        self.bias.requires_grad_(False)

    def forward_propagation(self, X):
        self.input = X
        return torch.mm(X, self.weight) + self.bias

    def back_propagation(self, dout, lr):
        d_weight = torch.mm(self.input.t(), dout)
        d_bias = dout.sum(dim=0)
        d_input = torch.mm(dout, self.weight.t())

        self.weight -= lr * d_weight
        self.bias -= lr * d_bias
        return d_input