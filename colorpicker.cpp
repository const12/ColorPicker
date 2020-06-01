#include "colorpicker.h"
#include "ui_colorpicker.h"

#include <QScreen>
#include <QMessageBox>
#include <QDebug>

#define PICSIZE ui->label->width()
#define PicSelectSize 4
#define ButtonStye "border-image: url(:/resource/select.png);"
#define MAX_SCALE 500
#define EVERY_SCALE 25

ColorPicker::ColorPicker(QWidget *parent) :
    QWidget(parent),
    m_pixmap(QPixmap()),
    m_cursorPixmap(QPixmap()),
    m_scale(100),
    m_picLabelMove(false),
    ui(new Ui::ColorPicker)
{
    ui->setupUi(this);

    initWidgets();

    m_timer = new QTimer(this);
    m_timer->setInterval(300);
    connect(m_timer,SIGNAL(timeout()),this,SLOT(slotTimeOut()));
}

ColorPicker::~ColorPicker()
{
    delete ui;
}

bool ColorPicker::eventFilter(QObject *obj, QEvent *env)
{
    if(obj == ui->pushButton)
    {
        if(env->type() == QEvent::MouseButtonPress)
        {
            m_scale = 100;
            ui->label_ratio->setText(QString("%1%").arg(m_scale));
            ui->pushButton->setStyleSheet("");
            QApplication::setOverrideCursor(QCursor(m_cursorPixmap));
            startTimer();
        }
        else if(env->type() == QEvent::MouseButtonRelease)
        {
            QApplication::restoreOverrideCursor();
            ui->pushButton->setStyleSheet(ButtonStye);
            stopTimer();
        }
        else{}
    }

    if(obj == m_picSelect)
    {
        if(env->type() == QEvent::MouseButtonPress)
        {
            m_picLabelMove = true;
        }
        else if(env->type() == QEvent::MouseMove)
        {
            if(m_picLabelMove && !m_pixmap.isNull())
            {
                m_picSelect->setCursor(QCursor(Qt::CrossCursor));

                QRect rect = QRect(mapFromGlobal(QCursor::pos()),QSize(PicSelectSize,PicSelectSize));
                QRect labelRect = ui->label->geometry();
                if(rect.left() < labelRect.left())
                {
                    rect.setX(labelRect.x());
                }
                else if(rect.right() > labelRect.right())
                {
                    rect.setX(labelRect.right() - PicSelectSize);
                }
                else{}

                if(rect.top() < labelRect.top())
                {
                    rect.setY(labelRect.y());
                }
                else if(rect.bottom() > labelRect.bottom())
                {
                    rect.setY(labelRect.bottom() - PicSelectSize);
                }
                else{}
                m_picSelect->move(mapToGlobal(rect.topLeft()));
                showPointColor();
            }
        }
        else if(env->type() == QEvent::MouseButtonRelease)
        {
            m_picLabelMove = false;
            m_picSelect->setCursor(QCursor(Qt::PointingHandCursor));
        }
        else{}
    }
    return QWidget::eventFilter(obj,env);
}

void ColorPicker::slotTimeOut()
{
    QPoint p = QCursor::pos();
    ui->label_pos->setText(QString("X: %1  Y: %2")
                              .arg(QString::number(p.x()))
                              .arg(QString::number(p.y())));

    QRect rect = QRect(p.x() - PICSIZE/2, p.y() - PICSIZE/2, PICSIZE, PICSIZE);
    QRect screenRect = QApplication::primaryScreen()->geometry();
    if(rect.left() < screenRect.left())
    {
        rect.setX(screenRect.x());
    }
    else if(rect.right() > screenRect.right())
    {
        rect.setX(screenRect.right() - PICSIZE);
    }
    else{}

    if(rect.top() < screenRect.top())
    {
        rect.setY(screenRect.y());
    }
    else if(rect.bottom() > screenRect.bottom())
    {
        rect.setY(screenRect.bottom() - PICSIZE);
    }
    else{}

    m_pixmap = QApplication::primaryScreen()->grabWindow(0,rect.x(),rect.y(),PICSIZE,PICSIZE);
    ui->label->setPixmap(m_pixmap);

    showPointColor();
}

void ColorPicker::on_toolButton_big_clicked()
{
    if(m_scale >= MAX_SCALE)
        return;
    m_scale += EVERY_SCALE;
    ui->label_ratio->setText(QString("%1%").arg(m_scale));
    ui->label->setPixmap(m_pixmap.scaled(PICSIZE*m_scale/100,PICSIZE*m_scale/100,Qt::KeepAspectRatio));
    showPointColor();
}

void ColorPicker::on_toolButton_small_clicked()
{
    if(m_scale < 101)
        return;
    m_scale -= EVERY_SCALE;
    ui->label_ratio->setText(QString("%1%").arg(m_scale));
    ui->label->setPixmap(m_pixmap.scaled(PICSIZE*m_scale/100,PICSIZE*m_scale/100,Qt::KeepAspectRatio));
    showPointColor();
}

void ColorPicker::initWidgets()
{
    setWindowFlags(Qt::Window | Qt::WindowStaysOnTopHint |
                   Qt::WindowSystemMenuHint | Qt::WindowCloseButtonHint | Qt::WindowMinimizeButtonHint);
    setWindowModality(Qt::ApplicationModal);
    setWindowTitle(QString::fromLocal8Bit("颜色提取器"));
    setWindowIcon(QIcon(":/resource/icon.png"));

    //鼠标图标
    m_cursorPixmap = QPixmap(":/resource/select.png");

    m_picSelect = new ToolLabel(this);
    m_picSelect->setFixedSize(PicSelectSize,PicSelectSize);
    m_picSelect->show();
    setPicSelectCenterPos();
    m_picSelect->setCursor(QCursor(Qt::PointingHandCursor));

    installEventFilter(this);
    ui->pushButton->installEventFilter(this);
    m_picSelect->installEventFilter(this);
}

void ColorPicker::startTimer()
{
    if(m_timer->isActive())
    {
        m_timer->stop();
    }
    m_timer->start();
}

void ColorPicker::stopTimer()
{
    if(m_timer->isActive())
    {
        m_timer->stop();
    }
}

void ColorPicker::setPicSelectCenterPos()
{
    QPoint p = mapToGlobal(ui->label->pos());
    int margin = ui->verticalLayout_picture->margin();
    int dif = margin + PICSIZE /2 - PicSelectSize/2;

    p.setX(p.x() + dif);
    p.setY(p.y() + dif);
    m_picSelect->move(p);
}

void ColorPicker::showPointColor()
{
    QImage image = ui->label->pixmap()->toImage();

    QPoint p = mapFromGlobal(m_picSelect->pos());
    int diff = PicSelectSize/2 -  ui->verticalLayout_picture->margin();
    p.setX(p.x() + diff);
    p.setY(p.y() + diff);

    if(!image.isNull())
    {
        //图片缩放后尺寸和label不符
        if(image.width() > PICSIZE)
        {
            int diff = (image.width() - PICSIZE) / 2;
            image = image.copy(diff, diff, PICSIZE, PICSIZE);
        }

        QColor color = QColor(image.pixel(p));
        int red=color.red();
        int green=color.green();
        int blue=color.blue();
        QString strRed=QString("%1").arg(red&0xFF,2,16,QLatin1Char('0'));
        QString strGreen=QString("%1").arg(green&0xFF,2,16,QLatin1Char('0'));
        QString strBlue=QString("%1").arg(blue&0xFF,2,16,QLatin1Char('0'));

        ui->label_redColor->setText(QString::number(red));
        ui->label_greenColor->setText(QString::number(green));
        ui->label_blueColor->setText(QString::number(blue));

        ui->label_hexColor->setText(QString("#%1%2%3")
                                  .arg(strRed.toUpper())
                                  .arg(strGreen.toUpper())
                                  .arg(strBlue.toUpper()));
        ui->label_color->setStyleSheet(QString("background-color:%1;").arg(ui->label_hexColor->text()));
    }
    else
    {
        qDebug()<<__FUNCTION__<<"label pixmap is not image";
        QMessageBox::about(this,QString::fromLocal8Bit("提示"),QString::fromLocal8Bit("该位置超出图片范围!"));
    }
}
