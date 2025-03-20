# custom_cnn_autoencoder.py
from utils.cnn_autoencoder_layers import *


class ConvAutoencoder:
    def __init__(self):
        # Encoder
        self.enc1 = Conv_Layer((3, 3, 1, 16), pad=1)
        self.relu1 = ReLU_Layer()
        self.pool1 = MaxPool_Layer(stride=2, f=2)

        self.enc2 = Conv_Layer((3, 3, 16, 32), pad=1)
        self.relu2 = ReLU_Layer()
        self.pool2 = MaxPool_Layer(stride=2, f=2)

        # Classifier
        self.flatten = Flatten_Layer()
        self.linear1 = Linear_Layer(32 * 7 * 7, 128)
        self.relu3 = ReLU_Layer()
        self.linear2 = Linear_Layer(128, 10)

        # Decoder
        self.up1 = Upsample_Layer(scale_factor=2)
        self.dec1 = Conv_Layer((3, 3, 32, 16), pad=1)
        self.relu4 = ReLU_Layer()

        self.up2 = Upsample_Layer(scale_factor=2)
        self.dec2 = Conv_Layer((3, 3, 16, 1), pad=1)
        self.sigmoid = Sigmoid_Layer()

    def forward_propagation(self, X, mode="train"):
        self.input = X.clone()

        # Encoder
        x = self.enc1.forward_propagation(X)
        x = self.relu1.forward_propagation(x)
        x = self.pool1.forward_propagation(x)

        x = self.enc2.forward_propagation(x)
        x = self.relu2.forward_propagation(x)
        encoded = self.pool2.forward_propagation(x)

        # Classifier
        flattened = self.flatten.forward_propagation(encoded)
        fc1 = self.linear1.forward_propagation(flattened)
        fc1_relu = self.relu3.forward_propagation(fc1)
        self.logits = self.linear2.forward_propagation(fc1_relu)

        # Decoder
        x = self.up1.forward_propagation(encoded)
        x = self.dec1.forward_propagation(x)
        x = self.relu4.forward_propagation(x)

        x = self.up2.forward_propagation(x)
        x = self.dec2.forward_propagation(x)
        self.output = self.sigmoid.forward_propagation(x)

        if mode == "train":
            self.reconstruction_loss = torch.mean((self.input - self.output) ** 2)
            return self.reconstruction_loss.item()
        else:
            return self.output, self.logits

    def back_propagation(self, lr, y=None, alpha=0.5):
        # Backprop decoder
        dout_recon = (self.output - self.input) / self.input.numel()

        dout_recon = self.sigmoid.back_propagation(dout_recon)
        dout_recon = self.dec2.back_propagation(dout_recon, lr)
        dout_recon = self.up2.back_propagation(dout_recon)
        dout_recon = self.relu4.back_propagation(dout_recon)
        dout_recon = self.dec1.back_propagation(dout_recon, lr)
        dout_recon = self.up1.back_propagation(dout_recon)

        # Backprop classifier
        if y is not None:
            m = y.shape[0]
            probs = torch.softmax(self.logits, dim=1)
            y_onehot = torch.zeros_like(probs)
            y_onehot.scatter_(1, y.unsqueeze(1), 1.0)
            d_class = (probs - y_onehot) / m

            d_class = self.linear2.back_propagation(d_class, lr)
            d_class = self.relu3.back_propagation(d_class)
            d_class = self.linear1.back_propagation(d_class, lr)
            d_class = self.flatten.back_propagation(d_class)

            dout_encoder = dout_recon + d_class * alpha
        else:
            dout_encoder = dout_recon

        # Backprop encoder
        dout_encoder = self.pool2.back_propagation(dout_encoder)
        dout_encoder = self.relu2.back_propagation(dout_encoder)
        dout_encoder = self.enc2.back_propagation(dout_encoder, lr)

        dout_encoder = self.pool1.back_propagation(dout_encoder)
        dout_encoder = self.relu1.back_propagation(dout_encoder)
        self.enc1.back_propagation(dout_encoder, lr)

    def extract_model(self):
        model = ConvAutoencoder()
        # Copy encoder
        model.enc1.weight.data.copy_(self.enc1.weight)
        model.enc1.bias.data.copy_(self.enc1.bias)
        model.enc2.weight.data.copy_(self.enc2.weight)
        model.enc2.bias.data.copy_(self.enc2.bias)
        # Copy decoder
        model.dec1.weight.data.copy_(self.dec1.weight)
        model.dec1.bias.data.copy_(self.dec1.bias)
        model.dec2.weight.data.copy_(self.dec2.weight)
        model.dec2.bias.data.copy_(self.dec2.bias)
        # Copy classifier
        model.linear1.weight.data.copy_(self.linear1.weight)
        model.linear1.bias.data.copy_(self.linear1.bias)
        model.linear2.weight.data.copy_(self.linear2.weight)
        model.linear2.bias.data.copy_(self.linear2.bias)
        return model