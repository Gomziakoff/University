import numpy as np

class ConvLayer:
    def __init__(self, input_size, fc, fs, padding, stride):
        self.width = input_size[0]
        self.height = input_size[1]
        self.depth = input_size[2]

        self.fc = fc
        self.fs = fs
        self.fd = self.depth

        self.outwidth = (self.width - self.fs + 2 * padding) // stride + 1
        self.outheight = (self.height - self.fs + 2 * padding) // stride + 1
        self.outdepth = fc

        self.padding = padding
        self.stride = stride

        self.W = np.random.rand(fc,fs,fs,self.fd)
        self.dW = np.zeros((fc, fs, fs, self.fd))

        self.b = np.random.rand(fc)
        self.db = np.zeros(fc)


    def forward(self, X):
        feature_maps = np.zeros((self.outwidth,self.outheight,self.outdepth))
        for W_num in range(self.fc):
            print("Filter ", W_num + 1)
            curr_filter = self.W[W_num,:]
            conv_map = self.conv_(X[:, :, 0], curr_filter[:, :, 0])
            for ch_num in range(1, curr_filter.shape[-1]):
                conv_map = conv_map + self.conv_(X[:, :, ch_num], curr_filter[:, :, ch_num])
            feature_maps[:,:,W_num] = conv_map + self.b[W_num]
        return feature_maps

    def conv_(self, img, conv_filter):
        filter_size = conv_filter.shape[0]

        # Добавляем padding к изображению
        padded_img = np.pad(
            img,
            pad_width=((self.padding, self.padding), (self.padding, self.padding)),
            mode='constant',
            constant_values=0  # Заполняем нулями
        )

        # Создаём результат с нужной размерностью
        result = np.zeros((self.outwidth, self.outheight))

        # Применяем свёртку
        for r in range(0, padded_img.shape[0] - filter_size + 1, self.stride):
            for c in range(0, padded_img.shape[1] - filter_size + 1, self.stride):
                curr_region = padded_img[r:r + filter_size, c:c + filter_size]
                curr_result = curr_region * conv_filter
                conv_sum = np.sum(curr_result)
                result[r // self.stride, c // self.stride] = conv_sum  # Привязка к результату
        return result

    def backward(self, dout, X):
        dX = np.zeros((self.height, self.width, self.depth))

        # Calculate gradients for weights and biases
        for f in range(self.fc):
            for i in range(self.outheight):
                for j in range(self.outwidth):
                    delta = dout[i, j, f]

                    for y in range(self.fs):
                        for x in range(self.fs):
                            y_pos = i * self.stride + y - self.padding
                            x_pos = j * self.stride + x - self.padding

                            if 0 <= y_pos < self.height and 0 <= x_pos < self.width:
                                for c in range(self.fd):
                                    self.dW[f, y, x, c] += delta * X[y_pos, x_pos, c]

                    self.db[f] += delta

        # Calculate gradient with respect to input
        for y in range(self.height):
            for x in range(self.width):
                for c in range(self.fd):
                    for f in range(self.fc):
                        for i in range(self.fs):
                            for j in range(self.fs):
                                y_pos = y + self.padding - i
                                x_pos = x + self.padding - j

                                if 0 <= y_pos < self.outheight and 0 <= x_pos < self.outwidth:
                                    dX[y, x, c] += dout[y_pos, x_pos, f] * self.W[f, i, j, c]

        return dX

    def update_weights(self, learning_rate):
        self.W -= learning_rate * self.dW
        self.b -= learning_rate * self.db
        self.dW = np.zeros_like(self.dW)
        self.db = np.zeros_like(self.db)

    def set_weights(self, weights, num):
        self.W[:,:,:,num] = weights
        print(self.W[:,:,:,num])

    def set_bias(self, bias, num):
        self.b[num] = bias

if __name__ == '__main__':
    conv1 = ConvLayer((4,4,2),2,3,0,1)
    conv1.set_weights([[[1,4,1],[1,4,3],[3,3,1]]],0)
    conv1.set_bias(0,0)
    img = np.array([[[4,5,8,7],[1,8,8,8],[3,6,6,4],[6,5,7,8]],[[4,5,8,7],[1,8,8,8],[3,6,6,4],[6,5,7,8]]])
    img = np.transpose(img,(1,2,0))
    print(conv1.forward(img)[:,:,0])
    dout = np.array([[[2,1],[4,4]]])
    dout = np.transpose(dout,(1,2,0))
    print(dout)
    print(conv1.backward(dout,img)[:,:,0])


