 #ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QDialog>
#include<QPainter>
namespace Ui {
class AboutDialog;
}

class AboutDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AboutDialog(QWidget *parent = nullptr);
    ~AboutDialog();

public:
    void paintEvent(QPaintEvent * event);
private slots:
    void on_pushButton_2_clicked();

private:
    Ui::AboutDialog *ui;
};

#endif // ABOUTDIALOG_H
