#pragma once

#include <QDialog>

namespace Ui {
class XimalayaLoginDialog;
}

class XimalayaLoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit XimalayaLoginDialog(QWidget *parent = 0);
    ~XimalayaLoginDialog();

private slots:
    void on_btnLogin_clicked();

private:
    Ui::XimalayaLoginDialog *ui;
};
