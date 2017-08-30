#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "gamewidget.h"
#include "solver.h"
#include <QMainWindow>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
private:
    GameWidget *game_widget;
    Generator generator;

};

#endif // MAINWINDOW_H
