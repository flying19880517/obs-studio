#pragma once

#include <QDialog>

namespace Ui {
class XimalayaCreateLiveDialog;
}

class XimalayaCreateLiveDialog : public QDialog
{
    Q_OBJECT

public:
    explicit XimalayaCreateLiveDialog(QWidget *parent = 0);
    ~XimalayaCreateLiveDialog();

private slots:
    void on_btnLogin_clicked();

private:
    Ui::XimalayaCreateLiveDialog *ui;
};
