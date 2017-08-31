#include "pages.h"
#include <QtWidgets>

BeginPage::BeginPage(QWidget *parent)
{
    QPushButton *begin_button=new QPushButton("开始游戏",this),*end_button=new QPushButton("结束游戏",this);
    begin_button->setFixedSize(400,100);
    end_button->setFixedSize(400,100);
    connect(begin_button,SIGNAL(clicked(bool)),this,SLOT(emitGameBegin()));
    connect(end_button,SIGNAL(clicked(bool)),this,SLOT(emitQuitGame()));
    QVBoxLayout *layout=new QVBoxLayout(this);
    layout->addWidget(begin_button);
    layout->addWidget(end_button);
    layout->setAlignment(Qt::AlignCenter);
    layout->setSpacing(60);
}


ComplexityDialog::ComplexityDialog(QWidget *parent)
{
    slid=new QSlider(Qt::Horizontal,this);
    slid->setRange(1,10);

    QSpinBox *box=new QSpinBox(this);
    box->setRange(1,10);

    connect(slid,SIGNAL(valueChanged(int)),box,SLOT(setValue(int)));
    connect(box,SIGNAL(valueChanged(int)),slid,SLOT(setValue(int)));

    QDialogButtonBox *buttons=new QDialogButtonBox(QDialogButtonBox::Ok|QDialogButtonBox::Cancel,this);

    connect(buttons,SIGNAL(accepted()),this,SLOT(accept()));
    connect(buttons,SIGNAL(rejected()),this,SLOT(hide()));

    QHBoxLayout *h_layout=new QHBoxLayout;
    h_layout->addWidget(box);
    h_layout->addWidget(slid);

    QVBoxLayout *layout=new QVBoxLayout(this);
    layout->addLayout(h_layout);
    layout->addWidget(buttons);

    setWindowTitle("选择难度");
}

int ComplexityDialog::value()
{
    return slid->value()-1;
}
