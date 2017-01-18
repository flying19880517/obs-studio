#include "ximalaya-upload-to-album-dialog.hpp"
#include "ui_XimalayaUploadToAlbumDialog.h"
#include <QMessageBox>

XimalayaUploadToAlbumDialog::XimalayaUploadToAlbumDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::XimalayaUploadToAlbumDialog)
{
    ui->setupUi(this);

    if(!LoadAlbums())
        reject();
}

XimalayaUploadToAlbumDialog::~XimalayaUploadToAlbumDialog()
{
    delete ui;
}

void XimalayaUploadToAlbumDialog::on_btnSave_clicked()
{
    int index = ui->listAlbums->currentRow();
    if (index < 0)
        return;
    QJsonObject model = list[index].toObject();
    QString albumId = model["albumId"].toVariant().toString();
    QString msg;
    if(!ximalayaApi.liveSave(liveId, albumId, &msg))
    {
        QMessageBox::warning(this, QTStr("Ximalaya.Api.LiveSaveFailed"), msg);
        return;
    }
    accept();
}

void XimalayaUploadToAlbumDialog::on_btnCancel_clicked()
{
    reject();
}

bool XimalayaUploadToAlbumDialog::LoadAlbums()
{
    QString msg;
    QJsonObject result;
    if(!ximalayaApi.getUploadAlbums(&result, &msg))
    {
        if(QMessageBox::critical(this, QTStr("Ximalaya.Api.GetUploadAlbumsFailed"), msg, QMessageBox::Retry | QMessageBox::Cancel, QMessageBox::Retry) == QMessageBox::Retry)
            return LoadAlbums();
        else
            return false;
    }
    list = result["list"].toArray();
    foreach(QJsonValue var, list)
    {
        QJsonObject item = var.toObject();
        QString title = item["title"].toString();
        //QString intro = item["intro"].toString();
        //QString title = QString("%1 (%2)").arg(name, intro);
        ui->listAlbums->addItem(title);
    }
    return true;
}
