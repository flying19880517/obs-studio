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

	QString msg;
	QJsonObject result;
	if (!ximalayaApi.liveCategory(&result, &msg))
	{
		reject();
		QMessageBox::critical(this, QTStr("Ximalaya.CreateLiveDialog.GetLiveCategoryFailed"), msg);
		return;
	}
	int index = 0;
	for each (QJsonValue var in result["data"].toArray())
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

void XimalayaCreateLiveDialog::on_btnCreateLive_clicked()
{
	QSettings settings("Ximalaya", "obs");

	QString msg;
	QString title = ui->txtLiveTitle->text();
	QString categoryId = ui->ddlCategory->currentData().toString();
	
	if (title.length() == 0)
		title = settings.value("nickname").toString() + "µÄÖ±²¥";
	if (!ximalayaApi.liveCreate(title, categoryId, &msg))
	{
		QMessageBox::warning(this, QTStr("Ximalaya.Api.LiveCreateFailed"), msg);
		return;
	}
	accept();
}
