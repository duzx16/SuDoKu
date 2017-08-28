#include "block.h"

Block::Block(QWidget *parent):QPushButton(parent),marked(false),wrong(false),fixed(false)
{
    setCheckable(true);
    setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    setStyleSheet("QPushButton:hover{background-color:yellow}");
}

void Block::addNumber(int n)
{
    numbers.insert(n);
    updateText();
}
