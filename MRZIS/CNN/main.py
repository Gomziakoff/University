import sys
import deeplake
import numpy as np
from convolution import ConvLayer
import matplotlib.pyplot as plt
import convolution, pooling, activation,fullyconnected
from fullyconnected import FullyConnectedLayer
from pooling import MaxPoolLayer

train_dataset = deeplake.load("hub://activeloop/cifar10-train")
print(train_dataset.tensors)
print(train_dataset["images"].shape, train_dataset["labels"].shape)
test_dataset = deeplake.load("hub://activeloop/cifar10-test")
print(test_dataset.tensors)
print(test_dataset["images"].shape, test_dataset["labels"].shape)
train_img = train_dataset["images"].numpy()
print(train_img.shape)

img = train_img[0,:,:,:]

def train(img):
    conv1 = ConvLayer((32,32,3),6,5,0,1)
    pool1 = MaxPoolLayer((28,28,6),2)
    conv2 = ConvLayer((14,14,6),16,5,0,1)
    pool2 = MaxPoolLayer((10,10,16),2)
    fullconected1 = FullyConnectedLayer((5,5,16),10,"ReLU")
    a = conv1.forward(img)
    print(a.shape)
    b = pool1.forward(a)
    print(b.shape)
    c = conv2.forward(b)
    print(c.shape)
    d = pool2.forward(c)
    print(d.shape)
    e = fullconected1.forward(d)
    print(e)
    de = fullconected1.backward(e,d)
    print(de.shape)
    dd = pool2.backward(de)
    print(dd.shape)
    dc = conv2.backward(dd,b)
    print(dc.shape)
    db = pool1.backward(dc)
    print(db.shape)
    da = conv1.backward(db,img)


train(img)

