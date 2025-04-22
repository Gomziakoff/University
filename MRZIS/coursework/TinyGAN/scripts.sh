python main.py --dataset mnist
python main.py --dataset fashionmnist
python main.py --dataset usps --image_size 16
python main.py --dataset anime --n_channels 3 --image_size 64 --experiment_name "tinygan_test_batch_128" --model_path "./model/test_batch_128" --batch_size 128 --output_path "./outputs/test_batch_128" --epochs 100