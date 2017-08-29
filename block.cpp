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
}

void Block::deleteNumbers()
{
    numbers.clear();
    updateText();
}

bool Block::existNumber(int n)
{
    return numbers.contains(n);
}

QList<int> Block::allNumbers()
{
    return numbers.toList();
}

void Block::setHighLight(bool high_light)
{
    highlight=high_light;
    updateStyle();
}

void Block::setMarked(bool isMarked)
{
    marked=isMarked;
    updateStyle();
}

void Block::setWrong(bool isWrong)
{
    wrong=isWrong;
    updateStyle();
}

void Block::setFixed(bool isFixed)
{
    fixed=isFixed;
    updateStyle();
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
    QString style="QPushButton{%1} QPushButton:hover{%2} QPushButton:press{%3}",normal="",hover="background-color:#FFFEA0;",press="background-color:#BA874C;";

    if(marked)
    {
        normal.append("background-color:#8F1D78;");
        press="background-color:7A023C;";
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
        normal.append("color:blue;");
        hover.append("color:blue;");
    }
    else if(wrong)
    {
        normal.append("color:red;");
        hover.append("color:red;");
    }
    setStyleSheet(style.arg(normal).arg(hover).arg(press));
}


