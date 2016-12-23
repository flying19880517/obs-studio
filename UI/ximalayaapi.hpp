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
    bool liveMine(QJsonObject *result, QString *msg);
    bool liveCategory(QJsonObject *result, QString *msg);
    bool liveSave(QString liveId, QString albumId, QString *msg);
    bool liveGetCurrentLiving(QJsonObject *result, QString *msg);

    bool getUploadAlbums(QJsonObject *result, QString *msg);
signals:

public slots:
private:
    Requests requests;
    QString baseUrl = "http://mobile.ximalaya.com";
    QString flyUrl = "http://114.80.138.114:2900/fly";
};
