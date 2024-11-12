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

# Определение устройства
device = torch.device("cuda" if torch.cuda.is_available() else "cpu")

# Загрузка и преобразование данных MNIST
transform = transforms.Compose([
    transforms.ToTensor(),
    transforms.Normalize((0.5,), (0.5,))
])

train_dataset = torchvision.datasets.MNIST(root='./data', train=True, download=True, transform=transform)
test_dataset = torchvision.datasets.MNIST(root='./data', train=False, download=True, transform=transform)

train_loader = DataLoader.DataLoader(dataset=train_dataset, batch_size=64, shuffle=True)
test_loader = DataLoader.DataLoader(dataset=test_dataset, batch_size=64, shuffle=False)


# Определение нейронной сети
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


# Инициализация сети, функции потерь и оптимизатора
model = CustomMNISTNet().to(device)
criterion = nn.CrossEntropyLoss()
optimizer = optim.RMSprop(model.parameters(), lr=0.001)

# Переменные для отслеживания метрик
train_losses = []
test_accuracies = []


# Функция для обучения модели
def train_model(model, train_loader, criterion, optimizer, num_epochs=10):
    model.train()
    for epoch in range(num_epochs):
        running_loss = 0.0
        start_time = time.time()

        # Прогресс-бар с логами времени
        with tqdm(total=len(train_loader), desc=f"Epoch {epoch + 1}/{num_epochs}") as pbar:
            for i, (images, labels) in enumerate(train_loader):
                images, labels = images.to(device), labels.to(device)

                # Обнуление градиентов
                optimizer.zero_grad()
                outputs = model(images)
                loss = criterion(outputs, labels)
                loss.backward()
                optimizer.step()

                running_loss += loss.item()

                # Обновление прогресс-бара и расчет времени
                elapsed_time = time.time() - start_time
                time_per_batch = elapsed_time / (i + 1)
                remaining_time = time_per_batch * (len(train_loader) - (i + 1))
                pbar.set_postfix(loss=f"{loss.item():.4f}", remaining_time=f"{remaining_time:.2f} sec")
                pbar.update(1)

        avg_loss = running_loss / len(train_loader)
        train_losses.append(avg_loss)
        print(f"Epoch [{epoch + 1}/{num_epochs}], Loss: {avg_loss:.4f}")


# Функция для тестирования модели
def evaluate_model(model, test_loader):
    model.eval()
    correct = 0
    total = 0
    with torch.no_grad():
        for images, labels in test_loader:
            images, labels = images.to(device), labels.to(device)
            outputs = model(images)
            _, predicted = torch.max(outputs.data, 1)
            total += labels.size(0)
            correct += (predicted == labels).sum().item()
    accuracy = 100 * correct / total
    test_accuracies.append(accuracy)
    print(f'Accuracy of the network on the test images: {accuracy:.2f}%')
    return accuracy


# Функция для построения графиков
def plot_metrics():
    # График функции потерь
    plt.figure(figsize=(10, 5))
    plt.subplot(1, 2, 1)
    plt.plot(train_losses, label='Training Loss')
    plt.xlabel('Epochs')
    plt.ylabel('Loss')
    plt.title('Training Loss over Epochs')
    plt.legend()

    # График точности
    plt.subplot(1, 2, 2)
    plt.plot(test_accuracies, label='Test Accuracy', color='orange')
    plt.xlabel('Epochs')
    plt.ylabel('Accuracy (%)')
    plt.title('Test Accuracy over Epochs')
    plt.legend()

    # Показ графиков
    plt.tight_layout()
    plt.show()


# Использование MLFlow для трекинга
mlflow.start_run()
mlflow.log_param("optimizer", "RMSprop")
mlflow.log_param("loss_function", "CrossEntropyLoss")
mlflow.log_param("batch_size", 64)
mlflow.log_param("epochs", 10)

train_model(model, train_loader, criterion, optimizer, num_epochs=10)
accuracy = evaluate_model(model, test_loader)

mlflow.log_metric("accuracy", accuracy)
mlflow.end_run()

# Построение графиков
plot_metrics()
