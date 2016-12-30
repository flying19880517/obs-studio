#include "obs-app.hpp"
#include "ximalaya-login-dialog.hpp"
#include "ximalaya-api.hpp"
#include "requests.hpp"

#include <openssl/pem.h>
#include <openssl/ssl.h>
#include <openssl/rsa.h>
#include <openssl/evp.h>
#include <openssl/bio.h>
#include <openssl/err.h>

XimalayaApi::XimalayaApi(QObject *parent) : QObject(parent)
{
    if (requests.isTest())
    {
        baseUrl = "http://mobile.test.ximalaya.com";
        flyUrl = "http://192.168.3.131:2900/fly";
    }
}

bool XimalayaApi::login(QString username, QString password, QString *msg)
{
    QJsonObject result = requests.getXimalaya(QUrl(baseUrl + "/passport/token/login"));
    if (requests.checkXimalayaResult(result))
    {
        QString token = result["token"].toString();
        QByteArray rsaPassword;
        if(EncryptRsaPassword(password, token, &rsaPassword))
        {
            QByteArray data;
            data.append("account=");
            data.append(username);
            data.append("&password=");
            data.append(rsaPassword.toPercentEncoding());

            result = requests.postXimalaya(QUrl(baseUrl + "/passport/mobile/security/login"), data);
            if (requests.checkXimalayaResult(result))
            {
                (*requests.settings).setValue("uid", result["uid"].toVariant().toString());
                (*requests.settings).setValue("token", result["token"].toString());
                (*requests.settings).setValue("nickname", result["nickname"].toString());

                return true;
            }
        }
        else
        {
            *msg = "rsa encrypt failed";
            return false;
        }

    }
    if (result.contains("msg"))
        *msg = result["msg"].toString();
    else
        *msg = QTStr("Ximalaya.Api.LoginFailed");
    return false;
}

bool XimalayaApi::logout()
{
    if ((*requests.settings).contains("token"))
    {
        requests.getXimalaya(QUrl(baseUrl + "/mobile/logout"));

        (*requests.settings).remove("uid");
        (*requests.settings).remove("token");
        (*requests.settings).remove("nickname");
    }
    return true;
}

bool XimalayaApi::liveCreate(QString title, QString categoryId, QString *msg) 
{
    QByteArray data;
    data.append("name=");
    data.append(QUrl::toPercentEncoding(title));
    data.append("&create=-1&permissionType=1&categoryId=");
    data.append(categoryId);

    QJsonObject result = requests.postXimalaya(QUrl(baseUrl + "/lamia/v1/live/record/create"), data);
    if (requests.checkXimalayaResult(result))
    {
        QJsonObject jsonCreate = result["data"].toObject();
        QString liveId = jsonCreate["id"].toVariant().toString();
        QString roomId = jsonCreate["roomId"].toVariant().toString();
        (*requests.settings).setValue("liveId", liveId);
        (*requests.settings).setValue("roomId", roomId);
        (*requests.settings).setValue("liveTitle", title);
        return true;
    }
    if (result.contains("msg"))
        *msg = result["msg"].toString();
    else
        *msg = QTStr("Ximalaya.Api.LiveCreateFailed");
    return false;
}

bool XimalayaApi::liveGetPushUrl(QString *msg)
{
    QString uid = (*requests.settings).value("uid").toString();
    QString liveId = (*requests.settings).value("liveId").toString();
    QString roomId = (*requests.settings).value("roomId").toString();

    QUrl url(flyUrl);
    QUrlQuery query;
    query.addQueryItem("anchorId", uid);
    query.addQueryItem("device", "obs");
    query.addQueryItem("liveId", liveId);
    query.addQueryItem("userId", uid);
    query.addQueryItem("userType", "anchor");
    url.setQuery(query);
    QJsonObject result = requests.getXimalaya(url);
    if (requests.checkXimalayaResult(result))
    {
        QString pushurl = result["pushurl"].toArray().first().toString();
        QString pushtoken = result["token"].toString();

        QUrl realPushUrl(pushurl);
        QUrlQuery realPushUrlQuery;
        realPushUrlQuery.addQueryItem("token", (*requests.settings).value("token").toString());
        realPushUrlQuery.addQueryItem("userId", uid);
        realPushUrlQuery.addQueryItem("anchorId", uid);
        realPushUrl.setQuery(realPushUrlQuery);

        (*requests.settings).setValue("server", realPushUrl.toString());
        (*requests.settings).setValue("key", pushtoken);
        return true;
    }
    if (result.contains("msg"))
        *msg = result["msg"].toString();
    else
        *msg = QTStr("Ximalaya.Api.LiveGetPushUrlFailed");
    return false;
}

bool XimalayaApi::liveStart(QJsonObject *result, QString *msg)
{
    QString liveId = (*requests.settings).value("liveId").toString();
    QString roomId = (*requests.settings).value("roomId").toString();

    QByteArray start;
    start.append("id=");
    start.append(liveId);
    start.append("&roomId=");
    start.append(roomId);
    *result = requests.postXimalaya(QUrl(baseUrl + "/lamia/v1/live/record/start"), start);
    if (requests.checkXimalayaResult(*result))
    {
        return true;
    }
    if ((*result).contains("msg"))
        *msg = (*result)["msg"].toString();
    else
        *msg = QTStr("Ximalaya.Api.LiveStartFailed");
    return false;
}

bool XimalayaApi::liveStop(QString liveId)
{
    QByteArray stop;
    if (liveId.length() > 0)
    {
        stop.append("id=");
        stop.append(liveId);
    }
    requests.postXimalaya(QUrl(baseUrl + "/lamia/v1/live/record/stop"), stop);
    if (liveId.length() > 0)
    {
        (*requests.settings).remove("liveId");
        (*requests.settings).remove("roomId");
        (*requests.settings).remove("liveTitle");
        (*requests.settings).remove("server");
        (*requests.settings).remove("key");
    }
    return true;
}

bool XimalayaApi::checkLogin()
{
    if (!(*requests.settings).contains("token"))
        return false;
    QJsonObject result = requests.getXimalaya(QUrl(baseUrl + "/mobile/homePage"));

    if (requests.checkXimalayaResult(result))
    {
        (*requests.settings).setValue("nickname", result["nickname"].toString());
        return true;
    }
    return (*requests.settings).contains("token");
}

bool XimalayaApi::liveMine(QJsonObject * result, QString * msg)
{
    *result = requests.getXimalaya(QUrl(baseUrl + "/lamia/v1/live/record/mine"));
    if (requests.checkXimalayaResult(*result))
    {
        return true;
    }
    if ((*result).contains("msg"))
        *msg = (*result)["msg"].toString();
    else
        *msg = QTStr("Ximalaya.Api.LiveStartFailed");
    return false;
}

bool XimalayaApi::liveCategory(QJsonObject * result, QString * msg)
{
    *result = requests.getXimalaya(QUrl(baseUrl + "/lamia/v1/live/category"));
    if (requests.checkXimalayaResult(*result))
    {
        return true;
    }
    if ((*result).contains("msg"))
        *msg = (*result)["msg"].toString();
    else
        *msg = QTStr("Ximalaya.Api.LiveStartFailed");
    return false;
}

bool XimalayaApi::liveSave(QString liveId, QString albumId, QString *msg)
{
    QByteArray data;
    data.append("id=");
    data.append(liveId);
    data.append("&albumId=");
    data.append(albumId);
    QJsonObject result = requests.postXimalaya(QUrl(baseUrl+"/lamia/v1/live/record/demand/save"), data);
    if(requests.checkXimalayaResult(result))
    {
        return true;
    }
    if (result.contains("msg"))
        *msg = result["msg"].toString();
    else
        *msg = QTStr("Ximalaya.Api.LiveSaveFailed");
    return false;
}

bool XimalayaApi::liveGetCurrentLiving(QJsonObject *result, QString *msg)
{
    *result = requests.getXimalaya(QUrl(baseUrl + "/lamia/v2/record/living"));
    if (requests.checkXimalayaResult(*result))
    {
        return true;
    }
    if ((*result).contains("msg"))
        *msg = (*result)["msg"].toString();
    else
        *msg = QTStr("Ximalaya.Api.GetCurrentLivingFailed");
    return false;
}

bool XimalayaApi::getUploadAlbums(QJsonObject *result, QString *msg)
{
    *result = requests.getXimalaya(QUrl(baseUrl + "/mobile/api1/upload/albums"));
    if (requests.checkXimalayaResult(*result))
    {
        return true;
    }
    if ((*result).contains("msg"))
        *msg = (*result)["msg"].toString();
    else
        *msg = QTStr("Ximalaya.Api.GetUploadAlbumsFailed");
    return false;
}

RSA *XimalayaApi::createRSA(unsigned char * key, int ispublic)
{
    RSA *rsa = NULL;
    BIO *keybio;
    keybio = BIO_new_mem_buf(key, -1);
    if (keybio == NULL)
    {
        printf("Failed to create key BIO");
        return 0;
    }
    if (ispublic)
    {
        rsa = PEM_read_bio_RSA_PUBKEY(keybio, &rsa, NULL, NULL);
    }
    else
    {
        rsa = PEM_read_bio_RSAPrivateKey(keybio, &rsa, NULL, NULL);
    }

    return rsa;
}
int XimalayaApi::public_encrypt(unsigned char * data, int data_len, unsigned char * key, unsigned char *encrypted)
{
    RSA * rsa = createRSA(key, 1);
    int result = RSA_public_encrypt(data_len, data, encrypted, rsa, RSA_PKCS1_PADDING);
    return result;
}
int XimalayaApi::private_decrypt(unsigned char * enc_data, int data_len, unsigned char * key, unsigned char *decrypted)
{
    RSA * rsa = createRSA(key, 0);
    int  result = RSA_private_decrypt(data_len, enc_data, decrypted, rsa, RSA_PKCS1_PADDING);
    return result;
}
int XimalayaApi::private_encrypt(unsigned char * data, int data_len, unsigned char * key, unsigned char *encrypted)
{
    RSA * rsa = createRSA(key, 0);
    int result = RSA_private_encrypt(data_len, data, encrypted, rsa, RSA_PKCS1_PADDING);
    return result;
}
int XimalayaApi::public_decrypt(unsigned char * enc_data, int data_len, unsigned char * key, unsigned char *decrypted)
{
    RSA * rsa = createRSA(key, 1);
    int  result = RSA_public_decrypt(data_len, enc_data, decrypted, rsa, RSA_PKCS1_PADDING);
    return result;
}

bool XimalayaApi::RSAPublicKeyEncrypt(QByteArray data, QByteArray key, QByteArray *result)
{
    char * pwd = data.data();
    unsigned char publicKey[]="-----BEGIN PUBLIC KEY-----\n"\
                              "MIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQCVhaR3Or7suUlwHUl2Ly36uVmb\n"\
                              "oZ3+HhovogDjLgRE9CbaUokS2eqGaVFfbxAUxFThNDuXq/fBD+SdUgppmcZrIw4H\n"\
                              "MMP4AtE2qJJQH/KxPWmbXH7Lv+9CisNtPYOlvWJ/GHRqf9x3TBKjjeJ2CjuVxlPB\n"\
                              "DX63+Ecil2JR9klVawIDAQAB\n"\
                              "-----END PUBLIC KEY-----\n";
    unsigned char  encrypted[128]={};
    int encrypted_length = public_encrypt((unsigned char *)pwd, strlen(pwd), publicKey, encrypted);
    if(encrypted_length == -1)
        return false;
    *result = QByteArray::fromRawData((const char *)encrypted, encrypted_length).toBase64();
    return true;
}

bool XimalayaApi::EncryptRsaPassword(QString password, QString token, QByteArray *result)
{
    QByteArray publicKey("-----BEGIN PUBLIC KEY-----\n"\
                         "MIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQCVhaR3Or7suUlwHUl2Ly36uVmb\n"\
                         "oZ3+HhovogDjLgRE9CbaUokS2eqGaVFfbxAUxFThNDuXq/fBD+SdUgppmcZrIw4H\n"\
                         "MMP4AtE2qJJQH/KxPWmbXH7Lv+9CisNtPYOlvWJ/GHRqf9x3TBKjjeJ2CjuVxlPB\n"\
                         "DX63+Ecil2JR9klVawIDAQAB\n"\
                         "-----END PUBLIC KEY-----\n");
    QByteArray pwdData = QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Md5).toHex();
    pwdData.append(token);
	return RSAPublicKeyEncrypt(pwdData, publicKey, result);
}
