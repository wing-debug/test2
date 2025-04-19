#include "aboutdialog.h"
#include "ui_aboutdialog.h"

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog)
{
    ui->setupUi(this);
    this->setWindowFlag(Qt::FramelessWindowHint);
    //紧张窗口大小的变化
    this->setFixedSize(this->width(),this->height());
}

AboutDialog::~AboutDialog()
{
    delete ui;
}
//重写窗口背景
void AboutDialog::paintEvent(QPaintEvent * event)
{
    QPainter qPainter(this);
    qPainter.drawPixmap(0,0,width(),height(),QPixmap(":/new/prefix1/images/background.jpg"));
}

void AboutDialog::on_pushButton_2_clicked()
{
    this->close();
}
