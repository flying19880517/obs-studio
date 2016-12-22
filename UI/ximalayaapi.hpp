#pragma once

#include "requests.hpp"

#include <QObject>
#include <QSettings>

class XimalayaApi : public QObject
{
    Q_OBJECT
public:
    explicit XimalayaApi(QObject *parent = 0);
    //bool loginXimalaya(QString *msg);
    bool login(QString username, QString password, QString *msg);
    bool logout();
	bool liveCreate(QString title, QString categoryId, QString *msg);
	bool liveGetPushUrl(QString *msg);
	bool liveStart(QJsonObject *result, QString *msg);
	bool liveStop(QString liveId);
    bool checkLogin();
signals:

public slots:
private:
    Requests requests;
};
