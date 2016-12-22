#include "obs-app.hpp"
#include "ximalaya-create-live-dialog.hpp"
#include "ui_XimalayaCreateLiveDialog.h"
#include "ximalayaapi.hpp"
#include <QMessageBox>

XimalayaCreateLiveDialog::XimalayaCreateLiveDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::XimalayaCreateLiveDialog)
{
    ui->setupUi(this);
}

XimalayaCreateLiveDialog::~XimalayaCreateLiveDialog()
{
    delete ui;
}

void XimalayaCreateLiveDialog::on_btnLogin_clicked()
{
    XimalayaApi api;
	QString msg;
    if(api.login(ui->txtUsername->text(),ui->txtPassword->text(), &msg))
    {
        accept();
	}
	else if(!msg.isEmpty())
	{
		QMessageBox::warning(this, QTStr("Ximalaya.Api.LoginFailed"), msg);
	}
}
