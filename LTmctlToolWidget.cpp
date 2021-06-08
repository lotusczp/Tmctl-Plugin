#include "LTmctlToolWidget.h"
#include "ui_LTmctlToolWidget.h"

LTmctlToolWidget::LTmctlToolWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LTmctlToolWidget)
{
    ui->setupUi(this);
}

LTmctlToolWidget::~LTmctlToolWidget()
{
    delete ui;
}
