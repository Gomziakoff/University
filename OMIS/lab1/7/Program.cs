using System.Drawing;
using System.Windows.Forms;
using System;

public class PolygonForm : Form
{
    // Массив точек для многоугольника
    private Point[] polygonPoints;

    public PolygonForm()
    {
        // Устанавливаем фиксированный размер окна
        this.Size = new Size(400, 400);

        // Инициализация точек многоугольника
        polygonPoints = new Point[]
        {
            new Point(50, 100),
            new Point(150, 50),
            new Point(250, 100),
            new Point(300, 200),
            new Point(200, 300),
            new Point(100, 250)
        };

        // Устанавливаем флаг двойной буферизации для перерисовки
        this.DoubleBuffered = true;
    }

    // Переопределяем событие Paint для рисования
    protected override void OnPaint(PaintEventArgs e)
    {
        base.OnPaint(e);

        // Получаем графический контекст
        Graphics g = e.Graphics;

        // Рисуем линии между точками многоугольника
        for (int i = 0; i < polygonPoints.Length; i++)
        {
            Point start = polygonPoints[i];
            Point end = polygonPoints[(i + 1) % polygonPoints.Length]; // Соединяем с первой точкой
            g.DrawLine(Pens.Black, start, end);
        }
    }

    [STAThread]
    static void Main()
    {
        Application.EnableVisualStyles();
        Application.Run(new PolygonForm());
    }
}