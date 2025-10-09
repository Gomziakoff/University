import javax.swing.*;
import java.awt.*;

class MovingShapePanel extends JPanel {
    private String currentShape = "Circle";  // Изначально круг
    private int x = 150;  // Начальная позиция по X
    private int y = 150;  // Начальная позиция по Y
    private final int size = 100;  // Размер фигуры

    @Override
    protected void paintComponent(Graphics g) {
        super.paintComponent(g);

        // Рисуем фигуру в зависимости от текущей формы
        switch (currentShape) {
            case "Circle":
                g.setColor(Color.BLUE);
                g.fillOval(x, y, size, size);
                break;
            case "Square":
                g.setColor(Color.RED);
                g.fillRect(x, y, size, size);
                break;
            case "Triangle":
                g.setColor(Color.GREEN);
                Polygon triangle = new Polygon();
                triangle.addPoint(x + size / 2, y);
                triangle.addPoint(x, y + size);
                triangle.addPoint(x + size, y + size);
                g.fillPolygon(triangle);
                break;
        }
    }

    // Метод для изменения формы фигуры
    public void setShape(String shape) {
        this.currentShape = shape;
        repaint();  // Перерисовать фигуру
    }

    // Методы для перемещения фигуры
    public void moveUp() {
        y -= 10;
        repaint();
    }

    public void moveDown() {
        y += 10;
        repaint();
    }

    public void moveLeft() {
        x -= 10;
        repaint();
    }

    public void moveRight() {
        x += 10;
        repaint();
    }
}

public class MovingShapeApp extends JFrame {

    public MovingShapeApp() {
        setTitle("Движение фигуры");
        setSize(400, 400);
        setDefaultCloseOperation(EXIT_ON_CLOSE);
        setLayout(new BorderLayout());

        // Создаём панель для рисования фигуры
        MovingShapePanel shapePanel = new MovingShapePanel();
        add(shapePanel, BorderLayout.CENTER);

        // Панель с кнопками для управления движением
        JPanel movementButtonPanel = new JPanel();
        movementButtonPanel.setLayout(new GridLayout(3, 3));

        // Пустые кнопки для сетки
        movementButtonPanel.add(new JLabel(""));
        movementButtonPanel.add(new JLabel(""));

        // Кнопка "Вверх"
        JButton upButton = new JButton("Вверх");
        upButton.addActionListener(e -> shapePanel.moveUp());
        movementButtonPanel.add(upButton);

        movementButtonPanel.add(new JLabel(""));

        // Кнопка "Влево"
        JButton leftButton = new JButton("Влево");
        leftButton.addActionListener(e -> shapePanel.moveLeft());
        movementButtonPanel.add(leftButton);

        // Кнопка "Вправо"
        JButton rightButton = new JButton("Вправо");
        rightButton.addActionListener(e -> shapePanel.moveRight());
        movementButtonPanel.add(rightButton);

        movementButtonPanel.add(new JLabel(""));

        // Кнопка "Вниз"
        JButton downButton = new JButton("Вниз");
        downButton.addActionListener(e -> shapePanel.moveDown());
        movementButtonPanel.add(downButton);

        add(movementButtonPanel, BorderLayout.SOUTH);

        // Панель с кнопками для выбора формы фигуры
        JPanel shapeButtonPanel = new JPanel();
        shapeButtonPanel.setLayout(new GridLayout(1, 3));

        // Кнопка для круга
        JButton circleButton = new JButton("Круг");
        circleButton.addActionListener(e -> shapePanel.setShape("Circle"));
        shapeButtonPanel.add(circleButton);

        // Кнопка для квадрата
        JButton squareButton = new JButton("Квадрат");
        squareButton.addActionListener(e -> shapePanel.setShape("Square"));
        shapeButtonPanel.add(squareButton);

        // Кнопка для треугольника
        JButton triangleButton = new JButton("Треугольник");
        triangleButton.addActionListener(e -> shapePanel.setShape("Triangle"));
        shapeButtonPanel.add(triangleButton);

        add(shapeButtonPanel, BorderLayout.NORTH);
    }

    public static void main(String[] args) {
        SwingUtilities.invokeLater(() -> {
            MovingShapeApp app = new MovingShapeApp();
            app.setVisible(true);
        });
    }
}
