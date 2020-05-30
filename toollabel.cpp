#include "toollabel.h"

#include <QPainter>

ToolLabel::ToolLabel(QWidget *parent) : QLabel(parent)
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::Tool | Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_TranslucentBackground,true);
}

ToolLabel::~ToolLabel()
{

}

void ToolLabel::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    painter.setPen(QPen(Qt::black,1));
    painter.drawRect(0,0,this->width() - 1,this->height() -1);
    QLabel::paintEvent(e);
}

