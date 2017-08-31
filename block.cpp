#include "block.h"
#include <algorithm>
#include <QtWidgets>

Block::Block(QWidget *parent):QPushButton(parent),marked(false),wrong(false),fixed(false),highlight(false),border_type(0)
{
    setCheckable(true);
    setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    updateStyle();
}

void Block::addNumber(int n)
{
    numbers.insert(n);
    emit textChanged();
    updateText();

}

void Block::deleteNumber(int n)
{
    auto it=numbers.find(n);
    if(it!=numbers.end())
    {
        numbers.erase(it);
    }
    emit textChanged();
    updateText();

}

void Block::deleteNumbers()
{
    numbers.clear();
    emit textChanged();
    updateText();
}

bool Block::containNumber(int n)
{
    return numbers.contains(n);
}

QList<int> Block::allNumbers()
{
    return numbers.toList();
}

void Block::setHighLight(bool high_light)
{
    if(high_light!=highlight)
    {
        highlight=high_light;
        updateStyle();
    }
}

void Block::setMarked(bool isMarked)
{
    if(isMarked!=marked)
    {
        marked=isMarked;
        updateStyle();
    }
}

void Block::setWrong(bool isWrong)
{
    if(isWrong!=wrong)
    {
        wrong=isWrong;
        updateStyle();
    }
}

void Block::setFixed(bool isFixed)
{
    if(isFixed!=fixed)
    {
        fixed=isFixed;
        updateStyle();
    }
}

void Block::setBorder(int b)
{
    border_type=b;
    update();
}

void Block::updateText()
{
    QStringList nums;
    QList<int> num_list=numbers.toList();
    QFont f=font();
    if(num_list.size()<=3)
    {
        f.setPointSize(22-2*num_list.size());
    }
    else if(num_list.size()<=6)
    {
        f.setPointSize(14);
    }
    else
    {
        f.setPointSize(11);
    }
    setFont(f);
    std::sort(num_list.begin(),num_list.end());
    foreach(int num,num_list)
    {
        nums.append(QString::number(num));

    }
    QString text=nums.join(" ");
    int i=0;
    for(auto it=text.begin();it!=text.end();++it)
    {
        if(*it==' ')
        {
            i++;
            if(!(i%3))
                *it='\n';
        }

    }
    setText(text);
}

void Block::updateStyle()
{
    //用stylesheet来设置不同情况的方格的格式
    QString style="QPushButton{%1} QPushButton:hover{%2} QPushButton:checked{%3}",normal="border:0px;font-weight:bold;",hover="background-color:#FFFEA0;",press="background-color:#BA874C;";

    /*if(marked)
    {
        normal.append("background-color:#8F1D78;");
        press="background-color:#7A023C;";
    }
    else */if(highlight)
    {
        normal.append("background-color:#E9AE6A;");
    }
    else
    {
        normal.append("background-color:#FEE388;");
    }
    if(fixed)
    {
        normal.append("color:#1DB0B8;");
        //hover.append("color:blue;");
    }
    else if(wrong)
    {
        normal.append("color:#EB3F2F;");
        //hover.append("color:#F01B2D;");
    }
    setStyleSheet(style.arg(normal).arg(hover).arg(press));
}

void Block::paintEvent(QPaintEvent *event)
{

    QPushButton::paintEvent(event);
    QPainter p(this);
    if(marked)
    {
        p.setPen(Qt::red);
        p.setBrush(Qt::red);
        p.drawEllipse(1,1,10,10);
    }
    QPen pen(Qt::blue);
    pen.setWidth(2);
    p.setPen(pen);
    if(border_type%2)
    {
        p.drawLine(0,0,width(),0);
    }
    if(border_type%4/2)
    {
        p.drawLine(width(),0,width(),height());
    }
    if(border_type%8/4)
    {
        p.drawLine(0,height(),width(),height());
    }
    if(border_type/8)
    {
        p.drawLine(0,0,0,height());
    }
    /*QSize size=event->rect().size();
    p.setPen(Qt::blue);
    p.drawLine(0,0,size.width(),0);*/
}


