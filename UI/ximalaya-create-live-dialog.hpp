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
    void on_btnContinue_clicked();
    void on_btnWaiting_clicked();
    void on_btnCreateLive_clicked();
    void SelectLive(QJsonObject model);

private:
    Ui::XimalayaCreateLiveDialog *ui;

    XimalayaApi ximalayaApi;

    QJsonArray livingRecords;
    QJsonArray waitingRecords;
};
