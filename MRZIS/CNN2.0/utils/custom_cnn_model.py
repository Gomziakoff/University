from utils.cnn_layers import Conv_Layer, ReLU_Layer, MaxPool_Layer, FC_Layer, FC_Output_Layer

class CustomCNN:
    def __init__(self):
        self.C1 = Conv_Layer((5,5,1,32), pad=2, sigma=0.05, bias_factor=0.005)
        self.ReLU1 = ReLU_Layer()
        self.S2 = MaxPool_Layer()
        self.C3 = Conv_Layer((5,5,32,64), sigma=0.05, bias_factor=0.005)
        self.ReLU2 = ReLU_Layer()
        self.S4 = MaxPool_Layer()
        self.C5 = Conv_Layer((5,5,64,128), sigma=0.05, bias_factor=0.005)
        self.ReLU3 = ReLU_Layer()
        self.F6 = FC_Layer((128,256), sigma=0.05, bias_factor=0.005)
        self.ReLU4 = ReLU_Layer()
        self.F7 = FC_Output_Layer((256,10), sigma=0.05, bias_factor=0.005)

    def forward_propagation(self, X, labels, mode):
        x = self.C1.forward_propagation(X)
        x = self.ReLU1.forward_propagation(x)
        x = self.S2.forward_propagation(x)
        x = self.C3.forward_propagation(x)
        x = self.ReLU2.forward_propagation(x)
        x = self.S4.forward_propagation(x)
        x = self.C5.forward_propagation(x)
        x = self.ReLU3.forward_propagation(x)
        x = x[:,0,0,:]
        x = self.F6.forward_propagation(x)
        x = self.ReLU4.forward_propagation(x)
        return self.F7.forward_propagation(x, labels, mode)

    def back_propagation(self, lr):
        dout = self.F7.back_propagation(lr)
        dout = self.ReLU4.back_propagation(dout)
        dout = self.F6.back_propagation(dout, lr)
        dout = dout.unsqueeze(1).unsqueeze(1)
        dout = self.ReLU3.back_propagation(dout)
        dout = self.C5.back_propagation(dout, lr)
        dout = self.S4.back_propagation(dout)
        dout = self.ReLU2.back_propagation(dout)
        dout = self.C3.back_propagation(dout, lr)
        dout = self.S2.back_propagation(dout)
        dout = self.ReLU1.back_propagation(dout)
        self.C1.back_propagation(dout, lr)

    def extract_model(self):
        model = CustomCNN()
        model.C1.weight.data.copy_(self.C1.weight)
        model.C1.bias.data.copy_(self.C1.bias)
        model.C3.weight.data.copy_(self.C3.weight)
        model.C3.bias.data.copy_(self.C3.bias)
        model.C5.weight.data.copy_(self.C5.weight)
        model.C5.bias.data.copy_(self.C5.bias)
        model.F6.weight.data.copy_(self.F6.weight)
        model.F6.bias.data.copy_(self.F6.bias)
        model.F7.weight.data.copy_(self.F7.weight)
        model.F7.bias.data.copy_(self.F7.bias)
        return model