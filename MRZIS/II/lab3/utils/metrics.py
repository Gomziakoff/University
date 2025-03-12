import torch

def calculate_metrics(output, target):
    mse = torch.nn.functional.mse_loss(output, target)
    mae = torch.mean(torch.abs(output - target))
    mape = torch.mean(torch.abs((target - output) / target)) * 100
    return mse.item(), mae.item(), mape.item()