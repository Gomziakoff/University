import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import java.util.Random;

public class ShootingGame extends JPanel implements KeyListener {
    private int cannonX = 50, cannonY = 400;
    private int targetX, targetY;
    private int bulletX = -1, bulletY = -1;
    private boolean targetVisible = false;
    private boolean explosion = false;

    public ShootingGame() {
        setPreferredSize(new Dimension(800, 500));
        setBackground(Color.WHITE);
        addKeyListener(this);
        setFocusable(true);
        new TargetThread().start();
    }

    @Override
    protected void paintComponent(Graphics g) {
        super.paintComponent(g);
        g.setColor(Color.BLACK);
        g.fillRect(cannonX, cannonY, 50, 20); // Рисуем пушку

        if (targetVisible) {
            g.setColor(Color.RED);
            g.fillOval(targetX, targetY, 30, 30); // Рисуем мишень
        }

        if (bulletX != -1) {
            g.setColor(Color.BLUE);
            g.fillOval(bulletX, bulletY, 10, 10); // Рисуем снаряд
        }

        if (explosion) {
            g.setColor(Color.ORANGE);
            g.fillOval(targetX - 10, targetY - 10, 50, 50); // Взрыв
            explosion = false;
        }
    }

    private class TargetThread extends Thread {
        public void run() {
            Random rand = new Random();
            while (true) {
                try {
                    Thread.sleep(rand.nextInt(3000) + 1000);
                    targetX = rand.nextInt(300) + 400;
                    targetY = rand.nextInt(300) + 100;
                    targetVisible = true;
                    repaint();
                    Thread.sleep(5000);
                    targetVisible = false;
                    repaint();
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
        }
    }

    private class BulletThread extends Thread {
        public BulletThread() {
            bulletX = cannonX + 50;
            bulletY = cannonY + 5;
        }

        public void run() {
            while (bulletX < 800) {
                bulletX += 5;
                repaint();

                if (targetVisible && bulletX >= targetX && bulletX <= targetX + 30 && bulletY >= targetY && bulletY <= targetY + 30) {
                    explosion = true;
                    targetVisible = false;
                    repaint();
                    break;
                }

                try {
                    Thread.sleep(10);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
            bulletX = -1;
        }
    }

    @Override
    public void keyPressed(KeyEvent e) {
        if (e.getKeyCode() == KeyEvent.VK_SPACE) {
            new BulletThread().start();
        } else if (e.getKeyCode() == KeyEvent.VK_LEFT) {
            cannonX = Math.max(0, cannonX - 10);
        } else if (e.getKeyCode() == KeyEvent.VK_RIGHT) {
            cannonX = Math.min(750, cannonX + 10);
        } else if (e.getKeyCode() == KeyEvent.VK_UP) {
            cannonY = Math.max(0, cannonY - 10);
        } else if (e.getKeyCode() == KeyEvent.VK_DOWN) {
            cannonY = Math.min(480, cannonY + 10);
        }
        repaint();
    }

    @Override
    public void keyReleased(KeyEvent e) {}
    @Override
    public void keyTyped(KeyEvent e) {}

    public static void main(String[] args) {
        JFrame frame = new JFrame("Shooting Game");
        ShootingGame game = new ShootingGame();
        frame.add(game);
        frame.pack();
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.setVisible(true);
    }
}
