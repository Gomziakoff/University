import pandas as pd
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import StandardScaler

def load_data(filepath):
    data = pd.read_csv(filepath)
    data['Sex'] = data['Sex'].map({'I':0, 'M':1, 'F':2})
    return data

def preprocess_data(X, test_size=0.2, random_state=42):
    scaler = StandardScaler()
    X_scaled = scaler.fit_transform(X)
    X_train, X_test = train_test_split(X_scaled, test_size=test_size, random_state=random_state)
    return X_train, X_test
