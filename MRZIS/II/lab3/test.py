import torch
import torch_directml

# Проверка доступности DirectML
device = torch_directml.device()
print("Используемое устройство:", device)

# Создаем модель и данные
model = torch.nn.Linear(10, 10).to(device)
inputs = torch.randn(100, 10).to(device)
labels = torch.randn(100, 10).to(device)

# Пример вычислений на GPU через DirectML
outputs = model(inputs)
loss = torch.nn.MSELoss()(outputs, labels)
loss.backward()

print("Вычисления выполнены на:", device)
print("Градиенты вычислены:", model.weight.grad is not None)