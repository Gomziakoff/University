#ifndef GRAPH_H
#define GRAPH_H

#include <QMainWindow>
#include <QPushButton>

QT_BEGIN_NAMESPACE
namespace Ui { class Graph; }
QT_END_NAMESPACE

class Graph : public QMainWindow
{
    Q_OBJECT

public:
    Graph(QWidget *parent = nullptr);
    ~Graph();

private slots:
    void on_pushButton_clicked();

private:
    Ui::Graph *ui;
};
#endif // GRAPH_H
