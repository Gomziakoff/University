import numpy as np

class ReLUActivationLayer:
    def __init__(self,size):
        self.X = None
        self.size = size

    def ReLU(self,X):
        out = np.maximum(X,0)
        return out

    def ReLU_derivative(self,X):
        out = (X>0).astype(int)
        return out

    def forward(self,X):
        self.X=X
        out = self.ReLU(X)
        return out

    def backward(self,dout):
        out = dout*self.ReLU_derivative(self.X)
        return out

if __name__ == '__main__':
    model = ReLUActivationLayer((4,4,2))

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
    print(a[:,:,0])
    print(model.backward(a*10)[:,:,0])