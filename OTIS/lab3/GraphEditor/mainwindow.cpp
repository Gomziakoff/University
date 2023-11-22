#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "graphwindow.h"
#include <QMdiSubWindow>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_action_triggered()
{
    loadSubWindow(new GraphWindow(this));
}

void MainWindow::loadSubWindow(QWidget *widget)
{
    auto window = ui->mdiArea->addSubWindow(widget);
    window -> setWindowTitle(widget->windowTitle());
    window -> setWindowIcon(widget->windowIcon());
    window->resize(500,350);
    window -> show();
}

