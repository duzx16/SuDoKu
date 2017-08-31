#ifndef PAGES_H
#define PAGES_H
#include <QWidget>
#include <QDialog>

QT_BEGIN_NAMESPACE
class QSlider;
QT_END_NAMESPACE

class ComplexityDialog:public QDialog
{
public:
    ComplexityDialog(QWidget *parent);
    int value();
private:
    QSlider *slid;


};

class BeginPage:public QWidget
{
    Q_OBJECT
public:
    BeginPage(QWidget *parent=0);
signals:
    void GameBegin();
    void QuitGame();
public slots:
    void emitGameBegin(){emit GameBegin();}
    void emitQuitGame(){emit QuitGame();}
private:


};


#endif // PAGES_H
