import pygame
import sys
import random

pygame.init()

screen_width = 800
screen_height = 600
screen = pygame.display.set_mode((screen_width, screen_height))
pygame.display.set_caption("Летающие птицы")

bg_r, bg_g, bg_b = 0, 0, 0
color_change_speed = 1

sprite_sheet_main = pygame.image.load('spritesheet.png')
sprite_sheet_bird = pygame.image.load('robin.png')

def load_frames(sheet, rows, cols, scale=2):
    frame_width = sheet.get_width() // cols
    frame_height = sheet.get_height() // rows
    frames = []

    for row in range(rows):
        for col in range(cols):
            x = col * frame_width
            y = row * frame_height
            frame = sheet.subsurface(pygame.Rect(x, y, frame_width, frame_height))
            scaled_frame = pygame.transform.scale(frame, (int(frame_width * scale), int(frame_height * scale)))
            frames.append(scaled_frame)

    return frames, int(frame_width * scale), int(frame_height * scale)

frames_main, main_width, main_height = load_frames(sprite_sheet_main, 6, 5)
frames_bird, bird_width, bird_height = load_frames(sprite_sheet_bird, 5, 5, 0.5)  # Предположим 4 строки и 5 столбцов
frames_bird.pop()
frames_bird.pop()
frames_bird.pop()

class FlyingObject:
    def __init__(self, frames, width, height):
        self.frames = frames
        self.width = width
        self.height = height
        self.x = random.randint(0, max(0, screen_width - width))
        self.y = random.randint(0, max(0, screen_height - height))
        self.dx = random.choice([-3, -2, 2, 3])  # Случайная скорость по X (влево/вправо)
        self.dy = random.choice([-2, -1, 1, 2])  # Случайная скорость по Y
        self.current_frame = 0
        self.frame_counter = 0
        self.frame_rate = 1  # Частота анимации

    def update(self):
        # Обновляем позицию
        self.x += self.dx
        self.y += self.dy

        # Отскакивание от границ
        if self.x <= 0 or self.x >= screen_width - self.width:
            self.dx = -self.dx  # Меняем направление
        if self.y <= 0 or self.y >= screen_height - self.height:
            self.dy = -self.dy  # Меняем направление

        # Анимация (смена кадров)
        self.frame_counter += 1
        if self.frame_counter >= self.frame_rate:
            self.frame_counter = 0
            self.current_frame = (self.current_frame + 1) % len(self.frames)

    def draw(self, screen):
        frame = self.frames[self.current_frame]
        if self.dx > 0:
            frame = pygame.transform.flip(frame, True, False)

        screen.blit(frame, (self.x, self.y))

main_objects = [FlyingObject(frames_main, main_width, main_height) for _ in range(5)]  # Основной объект
birds = [FlyingObject(frames_bird, bird_width, bird_height) for _ in range(5)]  # 5 птиц

num_stars = 500
stars = [(random.randint(0, screen_width), random.randint(0, screen_height),
          random.randint(50, 255), random.randint(50, 255), random.randint(50, 255))
         for _ in range(num_stars)]

clock = pygame.time.Clock()
running = True
while running:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False

    bg_r = (bg_r + color_change_speed) % 256
    bg_g = (bg_g + color_change_speed * 2) % 256
    bg_b = (bg_b + color_change_speed * 3) % 256

    screen.fill((bg_r, bg_g, bg_b))

    for i in range(num_stars):
        if random.random() < 0.1:  # Меняются случайные звезды
            stars[i] = (random.randint(0, screen_width), random.randint(0, screen_height),
                        random.randint(50, 255), random.randint(50, 255), random.randint(50, 255))
        pygame.draw.rect(screen, stars[i][2:], (stars[i][0], stars[i][1], 2, 2))

    for cube in main_objects:
        cube.update()
        cube.draw(screen)

    for bird in birds:
        bird.update()
        bird.draw(screen)

    pygame.display.flip()
    clock.tick(30)

pygame.quit()
sys.exit()
