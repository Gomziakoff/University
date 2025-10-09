import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

public class LightColumn extends JFrame {
    // Текущие состояния лампочек (по умолчанию все выключены)
    private Color redLight = Color.GRAY;
    private Color yellowLight = Color.GRAY;
    private Color greenLight = Color.GRAY;

    public LightColumn() {
        setTitle("Светофор");
        setSize(200, 500);
        setDefaultCloseOperation(EXIT_ON_CLOSE);
        setLayout(new BorderLayout());

        // Панель для отображения световой колонны (светофора)
        JPanel lightPanel = new JPanel() {
            @Override
            protected void paintComponent(Graphics g) {
                super.paintComponent(g);
                drawTrafficLight(g);
            }
        };
        lightPanel.setPreferredSize(new Dimension(200, 400));
        add(lightPanel, BorderLayout.CENTER);

        // Панель с кнопками для управления светофором
        JPanel buttonPanel = new JPanel();
        buttonPanel.setLayout(new GridLayout(1, 3));

        JButton redButton = new JButton("Красный");
        redButton.addActionListener(new LightButtonListener(Color.RED));
        buttonPanel.add(redButton);

        JButton yellowButton = new JButton("Желтый");
        yellowButton.addActionListener(new LightButtonListener(Color.YELLOW));
        buttonPanel.add(yellowButton);

        JButton greenButton = new JButton("Зеленый");
        greenButton.addActionListener(new LightButtonListener(Color.GREEN));
        buttonPanel.add(greenButton);

        add(buttonPanel, BorderLayout.SOUTH);
    }

    // Внутренний класс для обработки нажатия кнопок
    private class LightButtonListener implements ActionListener {
        private Color selectedColor;

        public LightButtonListener(Color color) {
            this.selectedColor = color;
        }

        @Override
        public void actionPerformed(ActionEvent e) {
            // Меняем состояние лампочек
            if (selectedColor == Color.RED) {
                redLight = Color.RED;
                yellowLight = Color.GRAY;
                greenLight = Color.GRAY;
            } else if (selectedColor == Color.YELLOW) {
                redLight = Color.GRAY;
                yellowLight = Color.YELLOW;
                greenLight = Color.GRAY;
            } else if (selectedColor == Color.GREEN) {
                redLight = Color.GRAY;
                yellowLight = Color.GRAY;
                greenLight = Color.GREEN;
            }
            repaint(); // Обновляем отображение светофора
        }
    }

    // Метод для отрисовки светофора
    private void drawTrafficLight(Graphics g) {
        // Отрисовываем красный свет
        g.setColor(redLight);
        g.fillOval(50, 50, 100, 100);

        // Отрисовываем желтый свет
        g.setColor(yellowLight);
        g.fillOval(50, 175, 100, 100);

        // Отрисовываем зеленый свет
        g.setColor(greenLight);
        g.fillOval(50, 300, 100, 100);
    }

    

    public static void main(String[] args) {
        SwingUtilities.invokeLater(() -> {
            LightColumn app = new LightColumn();
            app.setVisible(true);
        });
    }
}
