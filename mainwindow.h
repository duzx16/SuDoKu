#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "gamewidget.h"
#include "solver.h"
#include "pages.h"
#include <QMainWindow>
#include <QMenu>
#include <QStackedWidget>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
public slots:
    void beginGame();
    void returnBegin();
private:

    void createActions();

    GameWidget *game_widget;
    BeginPage *begin_page;
    ComplexityDialog *complex_dialog;
    QStackedWidget *pages;
    Generator generator;

    QAction *askHelpAction;
    QAction *returnBeginAction;

    QMenu *controlMenu;

};

#endif // MAINWINDOW_H
