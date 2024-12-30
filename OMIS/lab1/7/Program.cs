using System;
using System.Drawing;
using System.Windows.Forms;
using System.Collections.Generic;

public class PolygonForm : Form
{
    private List<Point> polygonPoints; // Список точек для многоугольника
    private TextBox xInput;
    private TextBox yInput;
    private Button addPointButton;
    private Button clearButton;
    private Label xLabel;
    private Label yLabel;

    public PolygonForm()
    {
        // Устанавливаем фиксированный размер окна
        this.Size = new Size(400, 400);

        // Инициализация списка точек
        polygonPoints = new List<Point>();

        // Создаем элементы управления
        xLabel = new Label { Location = new Point(10, 10), Text = "X:", AutoSize = true };
        xInput = new TextBox { Location = new Point(30, 10), Width = 50 };
        yLabel = new Label { Location = new Point(90, 10), Text = "Y:", AutoSize = true };
        yInput = new TextBox { Location = new Point(110, 10), Width = 50 };
        addPointButton = new Button { Location = new Point(170, 10), Text = "Add Point" };
        clearButton = new Button { Location = new Point(250, 10), Text = "Clear" };

        // Добавляем обработчики событий
        addPointButton.Click += AddPointButton_Click;
        clearButton.Click += ClearButton_Click;

        // Добавляем элементы управления на форму
        this.Controls.Add(xLabel);
        this.Controls.Add(xInput);
        this.Controls.Add(yLabel);
        this.Controls.Add(yInput);
        this.Controls.Add(addPointButton);
        this.Controls.Add(clearButton);

        // Устанавливаем флаг двойной буферизации для перерисовки
        this.DoubleBuffered = true;
    }

    private void AddPointButton_Click(object sender, EventArgs e)
    {
        if (int.TryParse(xInput.Text, out int x) && int.TryParse(yInput.Text, out int y))
        {
            // Добавляем точку в список
            polygonPoints.Add(new Point(x, y));

            // Очищаем поля ввода
            xInput.Text = string.Empty;
            yInput.Text = string.Empty;

            // Перерисовываем форму
            this.Invalidate();
        }
        else
        {
            MessageBox.Show("Please enter valid integer coordinates.", "Invalid Input", MessageBoxButtons.OK, MessageBoxIcon.Warning);
        }
    }

    private void ClearButton_Click(object sender, EventArgs e)
    {
        // Очищаем список точек
        polygonPoints.Clear();

        // Перерисовываем форму
        this.Invalidate();
    }

    // Переопределяем событие Paint для рисования
    protected override void OnPaint(PaintEventArgs e)
    {
        base.OnPaint(e);

        // Получаем графический контекст
        Graphics g = e.Graphics;

        // Рисуем линии между точками многоугольника, если точек больше 1
        if (polygonPoints.Count > 1)
        {
            for (int i = 0; i < polygonPoints.Count; i++)
            {
                Point start = polygonPoints[i];
                Point end = polygonPoints[(i + 1) % polygonPoints.Count]; // Соединяем с первой точкой
                g.DrawLine(Pens.Black, start, end);
            }
        }
    }

    [STAThread]
    static void Main()
    {
        Application.EnableVisualStyles();
        Application.Run(new PolygonForm());
    }
}
