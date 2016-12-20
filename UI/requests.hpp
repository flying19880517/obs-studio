#pragma once

#include <QObject>
#include <QtNetwork>
#include <QUrl>

class Requests : public QObject
{
    Q_OBJECT
public:
    explicit Requests(QObject *parent = 0);

    QByteArray getString(const QNetworkRequest &request);

    QByteArray postString(const QNetworkRequest &request, const QByteArray &data);

    QJsonObject getJson(const QNetworkRequest &request);

    QJsonObject postJson(const QNetworkRequest &request, const QByteArray &data);

    QJsonObject getXimalaya(const QUrl &url);

    QJsonObject postXimalaya(const QUrl &url, const QByteArray &data);

signals:

public slots:

private:

    QNetworkAccessManager manager;

    QNetworkReply *get(const QNetworkRequest &request);

    QNetworkReply *post(const QNetworkRequest &request, const QByteArray &data);

    QNetworkRequest getXimalayaRequest(const QUrl &url = QUrl());
};
