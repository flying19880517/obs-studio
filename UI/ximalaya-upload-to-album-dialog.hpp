#ifndef XIMALAYAUPLOADTOALBUMDIALOG_H
#define XIMALAYAUPLOADTOALBUMDIALOG_H

#include "ximalayaapi.hpp"

#include <QDialog>

namespace Ui {
class XimalayaUploadToAlbumDialog;
}

class XimalayaUploadToAlbumDialog : public QDialog
{
    Q_OBJECT

public:
    explicit XimalayaUploadToAlbumDialog(QWidget *parent = 0);
    ~XimalayaUploadToAlbumDialog();

    QString liveId;

private slots:
    void on_btnSave_clicked();

private:
    Ui::XimalayaUploadToAlbumDialog *ui;
    XimalayaApi ximalayaApi;

    QJsonArray list;

    bool LoadAlbums();
};

#endif // XIMALAYAUPLOADTOALBUMDIALOG_H
