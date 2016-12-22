#include "obs-app.hpp"
#include "ximalaya-login-dialog.hpp"
#include "ui_XimalayaLoginDialog.h"
#include "ximalayaapi.hpp"
#include <QMessageBox>

XimalayaLoginDialog::XimalayaLoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::XimalayaLoginDialog)
{
    ui->setupUi(this);
}

XimalayaLoginDialog::~XimalayaLoginDialog()
{
    delete ui;
}

void XimalayaLoginDialog::on_btnLogin_clicked()
{
    XimalayaApi api;
	QString msg;
    if(!api.login(ui->txtUsername->text(),ui->txtPassword->text(), &msg))
    {
		QMessageBox::warning(this, QTStr("Ximalaya.Api.LoginFailed"), msg);
		return;
	}
	accept();
}
