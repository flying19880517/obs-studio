#pragma once

#include "obs-app.hpp"
#include <QObject>
#include <QtNetwork>
#include <QUrl>
#include <openssl/pem.h>
#include <openssl/ssl.h>
#include <openssl/rsa.h>
#include <openssl/evp.h>
#include <openssl/bio.h>
#include <openssl/err.h>

class Requests : public QObject
{
    Q_OBJECT
public:
    explicit Requests(QObject *parent = 0);
	~Requests();

    QByteArray getString(const QNetworkRequest &request);

    QByteArray postString(const QNetworkRequest &request, const QByteArray &data);

    QJsonObject getJson(const QNetworkRequest &request);

    QJsonObject postJson(const QNetworkRequest &request, const QByteArray &data);

    QJsonObject getXimalaya(const QUrl &url);

    QJsonObject postXimalaya(const QUrl &url, const QByteArray &data);

    bool checkXimalayaResult(QJsonObject result);

	bool isTest();

	QSettings *settings;
signals:

public slots:

private:

    QNetworkAccessManager manager;

    QNetworkReply *get(const QNetworkRequest &request);

    QNetworkReply *post(const QNetworkRequest &request, const QByteArray &data);

    QByteArray processStringResult(QNetworkReply *response);

    QJsonObject processJsonResult(QByteArray data);

    QNetworkRequest getXimalayaRequest(const QUrl &url = QUrl());

	bool setProxy();

    QString envId = "1";
    QString ver = OBS_VERSION;
    QString userAgent;
    QString deviceId;
    QString deviceType = "obs";



	//RSA * createRSA(unsigned char * key, int ispublic);

	//int public_encrypt(unsigned char * data, int data_len, unsigned char * key, unsigned char * encrypted);

	//int private_decrypt(unsigned char * enc_data, int data_len, unsigned char * key, unsigned char * decrypted);

	//int private_encrypt(unsigned char * data, int data_len, unsigned char * key, unsigned char * encrypted);

	//int public_decrypt(unsigned char * enc_data, int data_len, unsigned char * key, unsigned char * decrypted);

};
