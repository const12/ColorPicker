#ifndef COLORPICKER_H
#define COLORPICKER_H

#include <QWidget>
#include <QTimer>
#include "toollabel.h"

namespace Ui {
class ColorPicker;
}

class ColorPicker : public QWidget
{
    Q_OBJECT

public:
    explicit ColorPicker(QWidget *parent = 0);
    ~ColorPicker();

protected:
    bool    eventFilter(QObject *, QEvent *);

private slots:
    void    slotTimeOut();
    void    on_toolButton_big_clicked();
    void    on_toolButton_small_clicked();

private:
    Ui::ColorPicker *ui;
    ToolLabel*      m_picSelect;
    bool            m_picLabelMove;
    QPixmap         m_cursorPixmap;
    QPixmap         m_pixmap;
    int             m_scale;
    QTimer*         m_timer;

private:
    void    initWidgets();
    void    startTimer();
    void    stopTimer();
    //初始化时m_picSelect位置
    void    setPicSelectCenterPos();
    //显示颜色
    void    showPointColor();
};

#endif // COLORPICKER_H
