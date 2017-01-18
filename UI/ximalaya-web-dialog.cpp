#include "ximalaya-web-dialog.hpp"
#include "ui_XimalayaWebDialog.h"

XimalayaWebDialog::XimalayaWebDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::XimalayaWebDialog)
{
    ui->setupUi(this);
}

bool XimalayaWebDialog::SetUrl(QUrl url)
{
    ui->webview->setUrl(url);
	return true;
}

XimalayaWebDialog::~XimalayaWebDialog()
{
    delete ui;
}
