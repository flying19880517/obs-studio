#pragma once

#include "requests.hpp"

#include <QObject>
#include <QSettings>

class XimalayaApi : public QObject
{
    Q_OBJECT
public:
    explicit XimalayaApi(QObject *parent = 0);
    bool checkLogin(QString *msg);
    bool loginXimalaya(QString *msg);
    bool getHomePage(QString *msg);
    bool login(QString username, QString password, QString *msg);
    bool logout();
	bool liveStart(QString *server, QString *key, QString *msg);
	bool liveStop();
signals:

public slots:
private:

    Requests requests;
};
