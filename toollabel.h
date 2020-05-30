#ifndef TOOLLABEL_H
#define TOOLLABEL_H

#include <QLabel>

class ToolLabel : public QLabel
{
    Q_OBJECT
public:
    explicit ToolLabel(QWidget *parent = 0);
    ~ToolLabel();

protected:
    void    paintEvent(QPaintEvent* e);

signals:

public slots:
};

#endif // TOOLLABEL_H
