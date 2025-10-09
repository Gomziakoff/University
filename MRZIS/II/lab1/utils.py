import pandas as pd
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import StandardScaler
from torch.utils.data import TensorDataset, DataLoader
import torch

def load_data(filepath):
    data = pd.read_csv(filepath)
    X = data.drop(['quality','Id'], axis=1)
    y = data['quality']
    return X, y

def preprocess_data(X, y, test_size=0.2, random_state=42):
    scaler = StandardScaler()
    X_scaled = scaler.fit_transform(X)
    X_train, X_test, y_train, y_test = train_test_split(X_scaled, y, test_size=test_size, random_state=random_state)
    return X_train, X_test, y_train, y_test

def create_dataloader(X_train, batch_size=32):
    X_train_tensor = torch.tensor(X_train, dtype=torch.float32)
    train_dataset = TensorDataset(X_train_tensor)
    train_loader = DataLoader(train_dataset, batch_size=batch_size, shuffle=True)
    return train_loader