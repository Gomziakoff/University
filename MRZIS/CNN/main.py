import deeplake
import numpy as np
from model import Model

train_dataset = deeplake.load("hub://activeloop/cifar10-train")
print(train_dataset["images"].shape, train_dataset["labels"].shape)
test_dataset = deeplake.load("hub://activeloop/cifar10-test")
print(test_dataset["images"].shape, test_dataset["labels"].shape)
train_img = train_dataset["images"].numpy()
train_labels = train_dataset["labels"].numpy()
train_labels = np.eye(10)[train_labels.reshape(-1)]
test_img = test_dataset["images"].numpy()
test_labels = test_dataset["labels"].numpy()
test_labels = np.eye(10)[test_labels.reshape(-1)]

CNN = Model(train_img/255.0, train_labels, test_img/255.0, test_labels,0.1,100)
CNN.train()


