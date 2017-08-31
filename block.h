#ifndef BLOCK_H
#define BLOCK_H

#include <QPushButton>
#include <QSet>

class Block:public QPushButton
{
public:
    enum Border{UpBorder=1,RightBorder=2,DownBorder=4,LeftBorder=8,};
    Block(QWidget *parent=0);

    //增删数字的接口
    void addNumber(int n);
    void deleteNumber(int n);
    void deleteNumbers();

    //得到关于当前数字的信息的接口
    bool containNumber(int n);
    QList<int> allNumbers();

    bool isFixed(){return fixed;}
    bool isMarked(){return marked;}
    bool single_num(){return numbers.size()==1;}


    void setHighLight(bool);//用背景颜色表示
    void setMarked(bool);//用背景颜色表示
    void setWrong(bool);//用红色字体表示
    void setFixed(bool);//用蓝色字体表示
    void setBorder(int b);

    //更新按钮的文本，对数字更改后应该调用这个函数
    void updateText();
    //更新按钮的样式，对显示格式更改后应该调用这个函数
    void updateStyle();
protected:
    void paintEvent(QPaintEvent *event);
private:
    QSet<int> numbers;

    bool marked;
    bool wrong;
    bool fixed;
    bool highlight;
    int border_type;
};

#endif // BLOCK_H
