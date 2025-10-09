import javax.swing.*;
import java.awt.*;

class BalloonPanel extends JPanel {
    private int balloonHeight = 50;  // Начальная высота шарика
    private int balloonWidth = 50;  // Ширина шарика остаётся неизменной
    private final int baseY = 300;  // Положение нижней точки шарика

    @Override
    protected void paintComponent(Graphics g) {
        super.paintComponent(g);
        int centerX = getWidth() / 2;

        // Рисуем шарик
        g.setColor(Color.RED);
        g.fillOval(centerX - balloonWidth / 2, baseY - balloonHeight, balloonWidth, balloonHeight);
    }

    // Метод для надувания шарика (увеличения высоты)
    public void inflateBalloon() {
        balloonHeight += 10;  // Увеличиваем высоту
        balloonWidth += 5;
        repaint();  // Перерисовываем шарик с новой высотой
    }
}

public class BalloonApp extends JFrame {

    public BalloonApp() {
        setTitle("Надувание шарика");
        setSize(400, 400);
        setDefaultCloseOperation(EXIT_ON_CLOSE);
        setLayout(new BorderLayout());

        // Панель с шариком
        BalloonPanel balloonPanel = new BalloonPanel();
        add(balloonPanel, BorderLayout.CENTER);

        // Кнопка для надувания шарика
        JButton inflateButton = new JButton("Надуть");
        inflateButton.addActionListener(e -> balloonPanel.inflateBalloon());  // Увеличиваем шарик при нажатии
        add(inflateButton, BorderLayout.SOUTH);
    }

    public static void main(String[] args) {
        SwingUtilities.invokeLater(() -> {
            BalloonApp app = new BalloonApp();
            app.setVisible(true);
        });
    }
}
