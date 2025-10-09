import numpy as np
from PIL.ImageOps import scale


class MaxPoolLayer:
    def __init__(self, size, scale):
        self.input_width = size[0]
        self.input_height = size[1]
        self.input_depth = size[2]

        self.output_width = self.input_width // scale
        self.output_height = self.input_height // scale
        self.output_depth = self.input_depth

        self.scale = scale
        self.mask = np.zeros(size)

    def forward(self, X):
        output = np.zeros((self.output_width,self.output_height, self.output_depth))
        for r in range(0,self.input_width,self.scale):
            for c in range(0,self.input_height,self.scale):
                for d in range(0,self.input_depth):
                    curr_region = X[r:r+self.scale,c:c+self.scale,d]
                    output[r//self.scale,c//self.scale,d] = np.max(curr_region)
                    x,y = np.unravel_index(np.argmax(curr_region),curr_region.shape)
                    self.mask[r+x,c+y,d] = 1
        return output

    def backward(self, dout):
        dX = np.zeros(self.mask.shape)
        for r in range(0,self.input_width):
            for c in range(0,self.input_height):
                for d in range(0,self.input_depth):
                    dX[r,c,d] = dout[r//self.scale,c//self.scale,d] * self.mask[r,c,d]
        return dX


if __name__ == '__main__':
    model = MaxPoolLayer((4,4,2),2)

    img = np.array([
        [[3,0,-2,2],
        [0,1,-2,-1],
        [-1,2,3,3],
        [1,2,0,2]],
        [[0,3,2,3],
         [-1,-1,-2,0],
         [-1,-2,2,-1],
         [2,2,-1,3]]
         ])
    img = np.transpose(img,(1,2,0))
    print(img.shape)
    a = model.forward(img)
    print(model.backward(a)[:,:,0])
