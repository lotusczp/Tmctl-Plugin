#ifndef LTMCTLTOOLWIDGET_H
#define LTMCTLTOOLWIDGET_H

#include <QWidget>

namespace Ui {
class LTmctlToolWidget;
}

class LTmctlToolWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LTmctlToolWidget(QWidget *parent = nullptr);
    ~LTmctlToolWidget();

private:
    Ui::LTmctlToolWidget *ui;
};

#endif // LTMCTLTOOLWIDGET_H
