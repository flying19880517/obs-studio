#include "obs-app.hpp"
#include "ximalaya-create-live-dialog.hpp"
#include "ui_XimalayaCreateLiveDialog.h"
#include "ximalayaapi.hpp"
#include <QMessageBox>
#include <QDateTime>

XimalayaCreateLiveDialog::XimalayaCreateLiveDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::XimalayaCreateLiveDialog)
{
    ui->setupUi(this);

    QString msg;
    QJsonObject result;

    if (!ximalayaApi.liveMine(&result, &msg))
    {
        reject();
        QMessageBox::critical(this, QTStr("Ximalaya.Api.GetMineLiveRecordsFailed"), msg);
        return;
    }
    QJsonObject resultData = result["data"].toObject();

    if (resultData.contains("waitingRecords") && resultData["waitingRecords"].toArray().count() > 0)
    {
        waitingRecords = resultData["waitingRecords"].toArray();
        foreach(QJsonValue var, waitingRecords)
        {
            QJsonObject item = var.toObject();
            QString name = item["name"].toString();
            QString start = item["actualStartAt"].toVariant().toString();
            QString title = QString("%1 (%2)").arg(name, QDateTime::fromTime_t(start.toUInt()).toString("yyyy-MM-dd HH:mm"));
            ui->listWaiting->addItem(title);
        }
    }
    else
    {
        ui->tabWidget->removeTab(1);
    }

    if (resultData.contains("livingRecords") && resultData["livingRecords"].toArray().count() > 0)
    {
        livingRecords = resultData["livingRecords"].toArray();
        foreach(QJsonValue var, livingRecords)
        {
            QJsonObject item = var.toObject();
            QString name = item["name"].toString();
            QString start = item["actualStartAt"].toVariant().toString();
            QString title = QString("%1 (%2)").arg(name, QDateTime::fromTime_t(start.toUInt()).toString("yyyy-MM-dd HH:mm"));
            ui->listLiving->addItem(title);
        }

    }
    else
    {
        ui->tabWidget->removeTab(0);
    }


    if (!ximalayaApi.liveCategory(&result, &msg))
    {
        reject();
        QMessageBox::critical(this, QTStr("Ximalaya.Api.GetLiveCategoryFailed"), msg);
        return;
    }
    int index = 0;
    foreach(QJsonValue var, result["data"].toArray())
    {
        QJsonObject item = var.toObject();
        ui->ddlCategory->addItem(item["name"].toString(), item["id"].toVariant().toString());
        index++;
    }
    ui->ddlCategory->setCurrentIndex(index - 1);
}

XimalayaCreateLiveDialog::~XimalayaCreateLiveDialog()
{
    delete ui;
}

void XimalayaCreateLiveDialog::on_btnContinue_clicked()
{
    int index = ui->listLiving->currentRow();
    if (index < 0)
        return;
    QJsonObject model = livingRecords[index].toObject();
    SelectLive(model);
}

void XimalayaCreateLiveDialog::on_btnWaiting_clicked()
{
    int index = ui->listWaiting->currentRow();
    if (index < 0)
        return;
    QJsonObject model = waitingRecords[index].toObject();
    SelectLive(model);
}

void XimalayaCreateLiveDialog::on_btnCreateLive_clicked()
{
    QString msg;
    QString title = ui->txtLiveTitle->text();
    QString categoryId = ui->ddlCategory->currentData().toString();

    if (title.length() == 0)
        title = QTStr("Ximalaya.Api.DefaultLiveTitle").arg((*ximalayaApi.requests.settings).value("nickname").toString());
    if (!ximalayaApi.liveCreate(title, categoryId, &msg))
    {
        QMessageBox::warning(this, QTStr("Ximalaya.Api.LiveCreateFailed"), msg);
        return;
    }
    accept();
}

void XimalayaCreateLiveDialog::SelectLive(QJsonObject model)
{
    (*ximalayaApi.requests.settings).setValue("liveId", model["id"].toVariant().toString());
    (*ximalayaApi.requests.settings).setValue("roomId", model["roomId"].toVariant().toString());
    (*ximalayaApi.requests.settings).setValue("liveTitle", model["name"].toString());

    accept();
}
