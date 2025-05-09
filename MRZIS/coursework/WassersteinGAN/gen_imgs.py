import model
import torch
import os
import math
from torchmetrics.image.fid import FrechetInceptionDistance
from torchmetrics.image.inception import InceptionScore
import json
from torchvision.datasets import ImageFolder
from torchvision import transforms
from torch.utils.data import DataLoader, Subset
from tqdm import tqdm

import torch.nn as nn
import torch.nn.functional as F

def conv_block(c_in, c_out, k_size=4, stride=2, pad=1, use_bn=True, transpose=False):
	module = []
	if transpose:
		module.append(nn.ConvTranspose2d(c_in, c_out, k_size, stride, pad, bias=not use_bn))
	else:
		module.append(nn.Conv2d(c_in, c_out, k_size, stride, pad, bias=not use_bn))
	if use_bn:
		module.append(nn.BatchNorm2d(c_out))
	return nn.Sequential(*module)


class Generator(nn.Module):
	def __init__(self, z_dim=100, channels=3, conv_dim=64):
		super(Generator, self).__init__()
		self.tconv1 = conv_block(z_dim, conv_dim * 8, pad=0, transpose=True)
		self.tconv2 = conv_block(conv_dim * 8, conv_dim * 4, transpose=True)
		self.tconv3 = conv_block(conv_dim * 4, conv_dim * 2, transpose=True)
		self.tconv4 = conv_block(conv_dim * 2, conv_dim, transpose=True)
		self.tconv5 = conv_block(conv_dim, channels, transpose=True, use_bn=False)

		# Initialization
		for m in self.modules():
			if isinstance(m, nn.Conv2d) or isinstance(m, nn.ConvTranspose2d):
				nn.init.normal_(m.weight, 0.0, 0.02)
			elif isinstance(m, nn.BatchNorm2d):
				nn.init.normal_(m.weight.data, 1.0, 0.02)
				nn.init.constant_(m.bias.data, 0)

	def forward(self, x):
		x = x.reshape([x.shape[0], -1, 1, 1])
		x = F.relu(self.tconv1(x))
		x = F.relu(self.tconv2(x))
		x = F.relu(self.tconv3(x))
		x = F.relu(self.tconv4(x))
		x = torch.tanh(self.tconv5(x))
		return x

# Конфигурация
print("\n=== Инициализация конфигурации ===")
config = {
    "real_images_path": "./data/anime",
    "model_path": "./model/anime/generator.pkl",
    "metrics_dir": "./metrics",
    "target_num_images": 10000,
    "z_dim": 100,
    "image_size": 64,
    "batch_size": 32
}
print("Конфигурация загружена:")
for k, v in config.items():
    print(f"{k:>20}: {v}")

# Подготовка трансформаций
print("\n=== Подготовка трансформаций изображений ===")
transform = transforms.Compose([
    transforms.Resize((config['image_size'], config['image_size'])),
    transforms.ToTensor(),
    transforms.Normalize(mean=[0.5, 0.5, 0.5], std=[0.5, 0.5, 0.5])
])
print("Трансформации созданы:", transform)

# Инициализация метрик
print("\n=== Инициализация метрик ===")
fid = FrechetInceptionDistance(feature=2048, normalize=True)
inception = InceptionScore(normalize=True)
print("FID и Inception Score инициализированы")

# Загрузка реальных изображений батчами
print("\n=== Загрузка реальных изображений ===")
real_dataset = ImageFolder(root=config['real_images_path'], transform=transform)
num_real_available = len(real_dataset)
num_real_images = min(config['target_num_images'], num_real_available)
print(f"Доступно реальных изображений: {num_real_available}")
print(f"Будет использовано: {num_real_images}")

# Создание DataLoader для реальных изображений
subset = Subset(real_dataset, indices=range(num_real_images))
real_loader = DataLoader(
    subset,
    batch_size=config['batch_size'],
    shuffle=False,
    num_workers=4
)

print("\nОбработка реальных изображений...")
for batch in tqdm(real_loader, desc="Реальные изображения"):
    imgs, _ = batch
    imgs = (imgs * 127.5 + 127.5).to(torch.uint8)
    fid.update(imgs, real=True)

# Инициализация генератора
print("\n=== Инициализация генератора ===")
generator = Generator(z_dim=config['z_dim'])
generator.load_state_dict(torch.load(config['model_path'], map_location='cpu'))
generator.eval()
print(f"Модель загружена из {config['model_path']}")

# Генерация фейковых изображений батчами
print("\n=== Генерация фейковых изображений ===")
total_generate = num_real_images
generated = 0

with torch.no_grad():
    progress_bar = tqdm(total=total_generate, desc="Фейковые изображения")
    while generated < total_generate:
        batch_size = min(config['batch_size'], total_generate - generated)
        z = torch.randn(batch_size, config['z_dim'])
        fake = generator(z)
        fake = (fake * 127.5 + 127.5).to(torch.uint8)
        
        # Обновление метрик
        fid.update(fake, real=False)
        inception.update(fake)
        
        generated += batch_size
        progress_bar.update(batch_size)
    progress_bar.close()

# Расчет и сохранение метрик
print("\n=== Расчет метрик ===")
metrics = {
    "FID": fid.compute().item(),
    "Inception_Score": {
        "mean": inception.compute()[0].item(),
        "std": inception.compute()[1].item()
    },
    "num_images_used": num_real_images
}

os.makedirs(config['metrics_dir'], exist_ok=True)
with open(os.path.join(config['metrics_dir'], 'metrics.json'), 'w') as f:
    json.dump(metrics, f, indent=2)

# Вывод результатов
print("\n=== Итоговые результаты ===")
print(f"Количество изображений: {metrics['num_images_used']}")
print(f"FID: {metrics['FID']:.2f}")
print(f"Inception Score: {metrics['Inception_Score']['mean']:.2f} ± {metrics['Inception_Score']['std']:.2f}")

# Очистка памяти
print("\n=== Очистка памяти ===")
del generator
torch.cuda.empty_cache()
print("\nВСЕ ОПЕРАЦИИ ЗАВЕРШЕНЫ!")