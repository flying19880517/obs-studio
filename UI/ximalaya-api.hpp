#pragma once

#include <QObject>
#include "requests.hpp"
#include <openssl/rsa.h>

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
    Requests requests;
signals:

public slots:
private:
    QString baseUrl = "http://mobile.ximalaya.com";
    QString flyUrl = "http://114.80.138.114:2900/fly";



    RSA * createRSA(unsigned char * key, int ispublic);
    int public_encrypt(unsigned char * data, int data_len, unsigned char * key, unsigned char * encrypted);
    int private_decrypt(unsigned char * enc_data, int data_len, unsigned char * key, unsigned char * decrypted);
    int private_encrypt(unsigned char * data, int data_len, unsigned char * key, unsigned char * encrypted);
    int public_decrypt(unsigned char * enc_data, int data_len, unsigned char * key, unsigned char * decrypted);

    bool RSAPublicKeyEncrypt(QByteArray data, QByteArray key, QByteArray *result);
    bool EncryptRsaPassword(QString password, QString token, QByteArray *result);

};
