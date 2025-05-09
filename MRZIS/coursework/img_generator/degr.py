import os
import torch
import numpy as np
from torchmetrics.image.fid import FrechetInceptionDistance
from torchmetrics.image.inception import InceptionScore
from torchvision.transforms import Compose, Resize, ToTensor, Normalize

import WassersteinGAN.WGAN
import dcgan.model
import LSGAN_28.model
import TinyGAN.model
import vanila_gan.model


class ModelEvaluator:
    def __init__(self, real_images_dir, image_size=128, num_images=1000, device='cuda'):
        self.device = device if torch.cuda.is_available() else 'cpu'
        self.num_images = num_images
        self.image_size = image_size
        
        # Загрузка реальных изображений один раз
        self.real_images = self.load_real_images(real_images_dir)
        self.transform = Compose([
            Resize((image_size, image_size)),
            ToTensor(),
            Normalize(mean=[0.5, 0.5, 0.5], std=[0.5, 0.5, 0.5])
        ])

    def load_real_images(self, directory):
        images = []
        for img_file in os.listdir(directory)[:self.num_images]:
            img_path = os.path.join(directory, img_file)
            img = Image.open(img_path).convert('RGB')
            img = self.transform(img).unsqueeze(0)
            images.append(img)
        return torch.cat(images).to(self.device)

    def evaluate_models(self, models_dict, z_dim=100, batch_size=50):
        results = {}
        
        for model_name, model_data in models_dict.items():
            print(f"\n=== Evaluating {model_name} ===")
            
            # Инициализация модели
            model = model_data['class']().to(self.device)
            model.load_state_dict(torch.load(model_data['path'], map_location=self.device))
            model.eval()

            # Генерация изображений батчами
            all_fake = []
            with torch.no_grad():
                for _ in range(0, self.num_images, batch_size):
                    z = torch.randn(batch_size, z_dim).to(self.device)
                    fake = model(z)
                    all_fake.append(fake)
                    
            fake_images = torch.cat(all_fake)[:self.num_images]
            
            # Расчет метрик
            fid_score = self.calculate_fid(fake_images)
            is_score, is_std = self.calculate_inception_score(fake_images)
            
            # Очистка памяти
            del model, fake_images, all_fake
            torch.cuda.empty_cache()
            
            results[model_name] = {
                'FID': fid_score,
                'Inception_Score': is_score,
                'Inception_Std': is_std
            }
            
        return results

    def calculate_fid(self, fake_images):
        fid = FrechetInceptionDistance(feature=2048, normalize=True).to(self.device)
        fid.update(self.real_images, real=True)
        fid.update(fake_images, real=False)
        return fid.compute().item()

    def calculate_inception_score(self, fake_images):
        inception = InceptionScore(normalize=True).to(self.device)
        inception.update(fake_images)
        return inception.compute()

# Конфигурация
models_config = {
    'Model1': {
        'class': dcgan.model.Generator(),  # Ваш класс генератора 1
        'path': '/home/oleksiiko/github/University/MRZIS/coursework/dcgan/model/test_batch_128/anime/gen.pt'
    },
    'Model2': {
        'class': LSGAN_28.model.Generator(),  # Ваш класс генератора 2
        'path': ''
    },
    'Model 3' : {
        'class': TinyGAN.model.Generator(),  # Ваш класс генератора 2
        'path': ''
    },
    'Model 4' : {
        'class': vanila_gan.model.Generator(),  # Ваш класс генератора 2
        'path': '/home/oleksiiko/github/University/MRZIS/coursework/vanila_gan/model/test_batch_128/anime/gen.pt'
    },
    'Model 5' : {
        'class': WassersteinGAN.WGAN.Generator(),  # Ваш класс генератора 2
        'path': '/home/oleksiiko/github/University/MRZIS/coursework/WassersteinGAN/model/anime/generator.pkl'
    }
}

# Инициализация и запуск
evaluator = ModelEvaluator(
    real_images_dir='/home/oleksiiko/github/University/MRZIS/coursework/dcgan/data/anime',
    image_size=64,
    num_images=1000,
    device='cuda'
)

results = evaluator.evaluate_models(models_config)

# Сохранение результатов
import json
with open('evaluation_results.json', 'w') as f:
    json.dump(results, f, indent=2)

print("Evaluation completed. Results saved to evaluation_results.json")