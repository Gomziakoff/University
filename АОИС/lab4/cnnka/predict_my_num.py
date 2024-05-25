from CNN.utils import *

from tqdm import tqdm
import argparse
import matplotlib.pyplot as plt
import pickle
import random

def get_random_instance_of_digit(digit, data, labels):
    indices = np.where(labels == digit)[0]
    if len(indices) == 0:
        raise ValueError(f"No instances of digit {digit} found in the dataset")
    random_index = random.choice(indices)
    return data[random_index], labels[random_index]


def predict(image, f1, f2, w3, w4, b1, b2, b3, b4, conv_s=1, pool_f=2, pool_s=2):
    conv1 = convolution(image, f1, b1, conv_s)  # convolution operation
    conv1[conv1 <= 0] = 0  # relu activation

    conv2 = convolution(conv1, f2, b2, conv_s)  # second convolution operation
    conv2[conv2 <= 0] = 0  # pass through ReLU non-linearity

    pooled = maxpool(conv2, pool_f, pool_s)  # maxpooling operation
    (nf2, dim2, _) = pooled.shape
    fc = pooled.reshape((nf2 * dim2 * dim2, 1))  # flatten pooled layer

    z = w3.dot(fc) + b3  # first dense layer
    z[z <= 0] = 0  # pass through ReLU non-linearity

    out = w4.dot(z) + b4  # second dense layer
    probs = softmax(out)  # predict class probabilities with the softmax activation function

    return np.argmax(probs), probs

# Main function
if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Predict the network accuracy.')
    parser.add_argument('parameters', metavar='parameters', help='name of file parameters were saved in. These parameters will be used to measure the accuracy.')
    args = parser.parse_args()
    save_path = args.parameters

    # Load parameters
    params, cost = pickle.load(open(save_path, 'rb'))
    [f1, f2, w3, w4, b1, b2, b3, b4] = params

    # Get test data
    m = 10000
    X = extract_data('t10k-images-idx3-ubyte.gz', m, 28)
    y_dash = extract_labels('t10k-labels-idx1-ubyte.gz', m).reshape(m, 1)

    # Normalize the data
    X -= int(np.mean(X))  # subtract mean
    X /= int(np.std(X))  # divide by standard deviation
    test_data = np.hstack((X.reshape(m, -1), y_dash))

    X = test_data[:, 0:-1]
    X = X.reshape(len(test_data), 1, 28, 28)
    y = test_data[:, -1]

    # User input
    user_digit = int(input("Enter a digit (0-9): "))
    selected_image, selected_label = get_random_instance_of_digit(user_digit, X, y)

    # Predict the digit
    pred, prob = predict(selected_image, f1, f2, w3, w4, b1, b2, b3, b4)

    # Display the image
    plt.imshow(selected_image.reshape(28, 28), cmap='gray')
    plt.title(f"Original Label: {selected_label}, Predicted Label: {pred}")
    plt.show()

    print(f"Selected label: {selected_label}")
    prob = prob.flatten()
    formatted_prob = ', '.join([f"{p * 100:.2f}%" for p in prob])
    print(f"Probs : {formatted_prob}")
    print(f"Predicted label: {pred}")