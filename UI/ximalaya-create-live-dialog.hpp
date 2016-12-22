#pragma once

#include <QDialog>
#include "ximalayaapi.hpp"

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
    void on_btnCreateLive_clicked();

private:
    Ui::XimalayaCreateLiveDialog *ui;

	XimalayaApi ximalayaApi;
};
