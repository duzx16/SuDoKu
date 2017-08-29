#include "block.h"
#include <algorithm>
#include <QtWidgets>

Block::Block(QWidget *parent):QPushButton(parent),marked(false),wrong(false),fixed(false),highlight(false)
{
    setCheckable(true);
    setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    updateStyle();
}

void Block::addNumber(int n)
{
    numbers.insert(n);
    updateText();
}

void Block::deleteNumber(int n)
{
    auto it=numbers.find(n);
    if(it!=numbers.end())
    {
        numbers.erase(it);
    }
    updateText();
}

void Block::deleteNumbers()
{
    numbers.clear();
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

void Block::updateText()
{
    QStringList nums;
    QList<int> num_list=numbers.toList();
    std::sort(num_list.begin(),num_list.end());
    foreach(int num,num_list)
    {
        nums.append(QString::number(num));
    }
    QString text=nums.join(",");
    setText(text);
}

void Block::updateStyle()
{
    //用stylesheet来设置不同情况的方格的格式
    QString style="QPushButton{%1} QPushButton:hover{%2} QPushButton:checked{%3}",normal="border:0px;font-weight:bold;",hover="background-color:#FFFEA0;",press="background-color:#BA874C;";

    if(marked)
    {
        normal.append("background-color:#8F1D78;");
        press="background-color:#7A023C;";
    }
    else if(highlight)
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


