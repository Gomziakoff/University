import os
import torch
import torch.nn as nn
from torch import optim
from data_loader import get_loader
import torchvision.utils as vutils
from model import Generator, Discriminator
from tqdm import tqdm
import mlflow
import matplotlib.pyplot as plt

class Solver(object):
    def __init__(self, args):
        self.args = args
        
        # Initialize MLflow
        mlflow.set_experiment(self.args.experiment_name)
        mlflow.start_run()
        mlflow.log_params(vars(self.args))
        mlflow.set_tag("model_type", "GAN")

        # Get data loaders
        self.train_loader = get_loader(args)

        # Define Generator and Discriminator
        self.gen = Generator(z_dim=self.args.z_dim, n_channels=self.args.n_channels, image_size=self.args.image_size)
        self.dis = Discriminator(n_channels=self.args.n_channels, image_size=self.args.image_size)

        # Display models
        print('--------Generator--------')
        print(self.gen)
        print('--------Discriminator--------')
        print(self.dis)

        # Option to load pretrained model
        if self.args.load_model:
            print("Using pretrained model")
            self.gen.load_state_dict(torch.load(os.path.join(self.args.model_path, 'gen.pt')))
            self.dis.load_state_dict(torch.load(os.path.join(self.args.model_path, 'dis.pt')))

        # Training loss function
        self.loss_fn = nn.BCEWithLogitsLoss()
        
        # Fixed noise for visualization
        self.fixed_z = torch.randn(self.args.batch_size, self.args.z_dim)

        # GPU support
        if self.args.is_cuda:
            self.gen = self.gen.cuda()
            self.dis = self.dis.cuda()
            self.fixed_z = self.fixed_z.cuda()

    def generate_images(self, name='final.png'):
        self.gen.eval()
        with torch.no_grad():
            x_fake = self.gen(self.fixed_z)
            x_fake = (x_fake + 1) / 2
            grid = vutils.make_grid(x_fake, normalize=False, nrow=int(x_fake.shape[0]**0.5))
            vutils.save_image(grid, os.path.join(self.args.output_path, name))
        return grid

    def generate_sample_images(self):
        x = next(iter(self.train_loader))[0]
        x = (x + 1) / 2
        grid = vutils.make_grid(x, normalize=False, nrow=int(x.shape[0]**0.5))
        vutils.save_image(grid, os.path.join(self.args.output_path, 'x_original.png'))

    def train(self):
        iters_per_epoch = len(self.train_loader)
        
        # Define optimizers
        g_opt = optim.Adam(self.gen.parameters(), lr=self.args.lr, betas=(0.5, 0.999), weight_decay=2e-5)
        d_opt = optim.Adam(self.dis.parameters(), lr=self.args.lr*2, betas=(0.5, 0.999), weight_decay=2e-5)

        # Log optimizers
        mlflow.log_dict({
            "generator_optimizer": str(g_opt),
            "discriminator_optimizer": str(d_opt)
        }, "optimizers.json")

        # Initial samples
        self.generate_sample_images()
        self.generate_images(name='sample_0.png')
        mlflow.log_artifact(os.path.join(self.args.output_path, 'x_original.png'), "samples")
        mlflow.log_artifact(os.path.join(self.args.output_path, 'sample_0.png'), "progress_samples")

        # Training loop
        epoch_bar = tqdm(
            total=self.args.epochs,
            desc='[Epoch Progress]',
            position=0,
            bar_format='{l_bar}{bar:20}{r_bar}{bar:-20b}'
        )

        # Loss tracking
        g_losses = []
        d_losses = []

        for epoch in range(self.args.epochs):
            self.gen.train()
            self.dis.train()
            
            total_d_loss = 0.0
            total_g_loss = 0.0

            iter_bar = tqdm(
                total=iters_per_epoch,
                desc=f'[Epoch {epoch+1}/{self.args.epochs}]',
                position=1,
                leave=False,
                bar_format='{l_bar}{bar:20}{r_bar}{bar:-20b}'
            )

            for i, (x, _) in enumerate(self.train_loader):
                z = torch.randn(self.args.batch_size, self.args.z_dim)
                if self.args.is_cuda:
                    x, z = x.cuda(), z.cuda()

                # Original training code
                x_fake = self.gen(z)
                
                # Train discriminator
                fake_out = self.dis(x_fake.detach())
                real_out = self.dis(x)
                d_loss = (self.loss_fn(fake_out, torch.zeros_like(fake_out)) 
                        + self.loss_fn(real_out, torch.ones_like(real_out)))

                d_opt.zero_grad()
                d_loss.backward()
                d_opt.step()

                # Train generator
                fake_out = self.dis(x_fake)
                g_loss = self.loss_fn(fake_out, torch.ones_like(fake_out))

                g_opt.zero_grad()
                g_loss.backward()
                g_opt.step()

                # Original progress tracking
                iter_bar.set_postfix({
                    'D_loss': f'{d_loss.item():.3f}',
                    'G_loss': f'{g_loss.item():.3f}'
                })
                iter_bar.update(1)

                total_d_loss += d_loss.item()
                total_g_loss += g_loss.item()

            iter_bar.close()

            # Calculate and log epoch metrics
            avg_d_loss = total_d_loss / iters_per_epoch
            avg_g_loss = total_g_loss / iters_per_epoch
            mlflow.log_metrics({
                "discriminator_loss": avg_d_loss,
                "generator_loss": avg_g_loss
            }, step=epoch)

            # Store for loss curve
            d_losses.append(avg_d_loss)
            g_losses.append(avg_g_loss)

            # Original saving logic
            sample_name = f'sample_{epoch+1}.png'
            self.generate_images(name=sample_name)
            torch.save(self.gen.state_dict(), os.path.join(self.args.model_path, "gen.pt"))
            torch.save(self.dis.state_dict(), os.path.join(self.args.model_path, "dis.pt"))

            # Log artifacts
            mlflow.log_artifact(os.path.join(self.args.output_path, sample_name), "progress_samples")
            mlflow.log_artifacts(self.args.model_path, "models")

            epoch_bar.set_postfix({
                'Avg D Loss': f'{avg_d_loss:.3f}',
                'Avg G Loss': f'{avg_g_loss:.3f}'
            })
            epoch_bar.update(1)

        epoch_bar.close()

        # Final logging
        self.generate_images(name='final.png')
        mlflow.log_artifact(os.path.join(self.args.output_path, 'final.png'), "final_results")
        
        # Log loss curve
        plt.figure(figsize=(10, 5))
        plt.plot(g_losses, label="Generator Loss")
        plt.plot(d_losses, label="Discriminator Loss")
        plt.title("Training Loss History")
        plt.xlabel("Epoch")
        plt.ylabel("Loss")
        plt.legend()
        plt.savefig(os.path.join(self.args.output_path, "loss_curve.png"))
        mlflow.log_artifact(os.path.join(self.args.output_path, "loss_curve.png"), "metrics")

        # Log final models
        mlflow.pytorch.log_model(self.gen, "generator")
        mlflow.pytorch.log_model(self.dis, "discriminator")
        
        mlflow.end_run()