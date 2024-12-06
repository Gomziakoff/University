import torch
import torch.nn as nn
import torch.optim as optim
import torch.utils.data as DataLoader
import torchvision
import torchvision.transforms as transforms
import mlflow
from tqdm import tqdm
import time
import matplotlib.pyplot as plt

device = torch.device("cuda" if torch.cuda.is_available() else "cpu")

transform = transforms.Compose([
    transforms.ToTensor(),
    transforms.Normalize((0.5,), (0.5,))
])

train_dataset = torchvision.datasets.MNIST(root='./data', train=True, download=True, transform=transform)
test_dataset = torchvision.datasets.MNIST(root='./data', train=False, download=True, transform=transform)
print(len(train_dataset), len(test_dataset))
train_loader = DataLoader.DataLoader(dataset=train_dataset, batch_size=64, shuffle=True)
test_loader = DataLoader.DataLoader(dataset=test_dataset, batch_size=64, shuffle=False)


class CustomMNISTNet(nn.Module):
    def __init__(self):
        super(CustomMNISTNet, self).__init__()
        self.conv1 = nn.Conv2d(1, 16, kernel_size=3, stride=1, padding=1)
        self.conv2 = nn.Conv2d(16, 32, kernel_size=3, stride=1, padding=1)
        self.fc1 = nn.Linear(32 * 7 * 7, 128)
        self.fc2 = nn.Linear(128, 10)
        self.pool = nn.MaxPool2d(kernel_size=2, stride=2)
        self.dropout = nn.Dropout(0.25)

    def forward(self, x):
        x = self.pool(torch.relu(self.conv1(x)))
        x = self.pool(torch.relu(self.conv2(x)))
        x = x.view(-1, 32 * 7 * 7)
        x = torch.relu(self.fc1(x))
        x = self.dropout(x)
        x = self.fc2(x)
        return x


model = CustomMNISTNet().to(device)
criterion = nn.CrossEntropyLoss()
optimizer = optim.RMSprop(model.parameters(), lr=0.001)

train_losses = []
train_accuracies = []
test_accuracies = []


def train_model(model, train_loader, criterion, optimizer, num_epochs=10):
    model.train()
    for epoch in range(num_epochs):
        running_loss = 0.0
        start_time = time.time()

        with tqdm(total=len(train_loader), desc=f"Epoch {epoch + 1}/{num_epochs}") as pbar:
            for i, (images, labels) in enumerate(train_loader):
                images, labels = images.to(device), labels.to(device)

                optimizer.zero_grad()
                outputs = model(images)
                loss = criterion(outputs, labels)
                loss.backward()
                optimizer.step()

                running_loss += loss.item()

                elapsed_time = time.time() - start_time
                time_per_batch = elapsed_time / (i + 1)
                remaining_time = time_per_batch * (len(train_loader) - (i + 1))
                pbar.set_postfix(loss=f"{loss.item():.4f}", remaining_time=f"{remaining_time:.2f} sec")
                pbar.update(1)

        avg_loss = running_loss / len(train_loader)
        train_losses.append(avg_loss)

        train_accuracy = evaluate_model(model, train_loader)
        test_accuracy = evaluate_model(model, test_loader)
        train_accuracies.append(train_accuracy)
        test_accuracies.append(test_accuracy)
        time.sleep(0.1)
        tqdm.write(f"Epoch [{epoch + 1}/{num_epochs}], Loss: {avg_loss:.4f}, Train Accuracy: {train_accuracy:.2f}%, Test Accuracy: {test_accuracy:.2f}%")
        time.sleep(0.1)

def evaluate_model(model, loader):
    model.eval()
    correct = 0
    total = 0
    with torch.no_grad():
        for images, labels in loader:
            images, labels = images.to(device), labels.to(device)
            outputs = model(images)
            _, predicted = torch.max(outputs.data, 1)
            total += labels.size(0)
            correct += (predicted == labels).sum().item()
    accuracy = 100 * correct / total
    return accuracy


def plot_metrics():
    plt.figure(figsize=(12, 5))
    plt.subplot(1, 2, 1)
    plt.plot(train_losses, label='Training Loss')
    plt.xlabel('Epochs')
    plt.ylabel('Loss')
    plt.title('Training Loss over Epochs')
    plt.legend()

    plt.subplot(1, 2, 2)
    plt.plot(train_accuracies, label='Train Accuracy', color='blue')
    plt.plot(test_accuracies, label='Test Accuracy', color='orange')
    plt.xlabel('Epochs')
    plt.ylabel('Accuracy (%)')
    plt.title('Train and Test Accuracy over Epochs')
    plt.legend()

    plt.tight_layout()
    plt.show()


mlflow.start_run()
mlflow.log_param("optimizer", "RMSprop")
mlflow.log_param("loss_function", "CrossEntropyLoss")
mlflow.log_param("batch_size", 64)
mlflow.log_param("epochs", 10)

train_model(model, train_loader, criterion, optimizer, num_epochs=3)

accuracy = test_accuracies[-1] if test_accuracies else 0.0
mlflow.log_metric("accuracy", accuracy)
mlflow.end_run()


def show_predictions(model, loader):
    model.eval()
    correct_image, correct_label = None, None
    incorrect_image, incorrect_label, predicted_label = None, None, None

    with torch.no_grad():
        for images, labels in loader:
            images, labels = images.to(device), labels.to(device)
            outputs = model(images)
            _, predicted = torch.max(outputs.data, 1)

            for img, lbl, pred in zip(images, labels, predicted):
                if lbl == pred and correct_image is None:
                    correct_image = img.cpu()
                    correct_label = lbl.item()
                elif lbl != pred and incorrect_image is None:
                    incorrect_image = img.cpu()
                    incorrect_label = lbl.item()
                    predicted_label = pred.item()
                if correct_image is not None and incorrect_image is not None:
                    break
            if correct_image is not None and incorrect_image is not None:
                break

    fig, axes = plt.subplots(1, 2, figsize=(10, 5))

    # Correct prediction
    if correct_image is not None:
        axes[0].imshow(correct_image.squeeze(), cmap="gray")
        axes[0].set_title(f"Correctly classified: {correct_label}")
        axes[0].axis("off")
    else:
        axes[0].set_title("No correct predictions found")
        axes[0].axis("off")

    # Incorrect prediction
    if incorrect_image is not None:
        axes[1].imshow(incorrect_image.squeeze(), cmap="gray")
        axes[1].set_title(f"Incorrect: {incorrect_label} → {predicted_label}")
        axes[1].axis("off")
    else:
        axes[1].set_title("No incorrect predictions found")
        axes[1].axis("off")

    plt.tight_layout()
    plt.show()

plot_metrics()
show_predictions(model, test_loader)

