#include "ximalaya-login-dialog.hpp"
#include "ui_XimalayaLogin.h"
#include "ximalayaapi.hpp"
#include <QMessageBox>

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

void LoginDialog::on_btnLogin_clicked()
{
    XimalayaApi api;
	QString msg;
    if(api.login(ui->txtUsername->text(),ui->txtPassword->text(), &msg))
    {
        accept();
	}
	else if(!msg.isEmpty())
	{
		QMessageBox::warning(this, "登录失败", msg);
	}
}
