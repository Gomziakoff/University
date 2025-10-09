import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

// Отдельный класс для панели светодиода
class LEDPanel extends JPanel {
    private boolean ledOn = false; // Статус светодиода

    @Override
    protected void paintComponent(Graphics g) {
        super.paintComponent(g);
        // Отрисовка светодиода: включённый (зелёный) или выключенный (серый)
        if (ledOn) {
            g.setColor(Color.BLUE);
        } else {
            g.setColor(Color.GRAY);
        }
        g.fillOval(100, 100, 100, 100); // Светодиод как круг
    }

    // Метод для переключения состояния светодиода
    public void toggleLED() {
        ledOn = !ledOn;
        repaint(); // Перерисовка панели с новым состоянием
    }
}

public class LEDControlApp extends JFrame {

    public LEDControlApp() {
        setTitle("LED Управление");
        setSize(300, 300);
        setDefaultCloseOperation(EXIT_ON_CLOSE);
        setLayout(new BorderLayout());

        // Создаём экземпляр панели со светодиодом
        LEDPanel ledPanel = new LEDPanel();
        ledPanel.setPreferredSize(new Dimension(300, 300));
        add(ledPanel, BorderLayout.CENTER);

        // Кнопка для включения/выключения светодиода
        JButton toggleButton = new JButton("Вкл/Выкл");
        toggleButton.addActionListener(e -> ledPanel.toggleLED()); // Вызываем метод toggleLED при нажатии
        add(toggleButton, BorderLayout.SOUTH);
    }

    public static void main(String[] args) {
        SwingUtilities.invokeLater(() -> {
            LEDControlApp app = new LEDControlApp();
            app.setVisible(true);
        });
    }
}
