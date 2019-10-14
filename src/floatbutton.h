#ifndef FLOATBUTTON_H
#define FLOATBUTTON_H

#include <QWidget>
#include "picker.h"
namespace Ui {
class FloatButton;
}

class FloatButton : public QWidget
{
    Q_OBJECT

public:
    explicit FloatButton(QWidget *parent = nullptr);

    ~FloatButton();

protected:
    void mousePressEvent(QMouseEvent *);

signals:
    void pressed(QPoint mousePressPosition, QPoint mouseReleasedPosition);
private:
    Ui::FloatButton *ui;
    Picker *picker;
    QPoint mousePressPosition;
    QPoint mouseReleasedPosition;
    void onMouseButtonPressed(int x, int y); // global listen
    void onMouseButtonReleased(int x, int y); // global listen


};

#endif // FLOATBUTTON_H
