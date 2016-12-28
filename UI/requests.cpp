#include "obs-app.hpp"
#include "requests.hpp"
#include <QtNetwork>
#include <QUrl>
#include <QSettings>

#include <openssl/pem.h>
#include <openssl/ssl.h>
#include <openssl/rsa.h>
#include <openssl/evp.h>
#include <openssl/bio.h>
#include <openssl/err.h>

Requests::Requests(QObject *parent) : QObject(parent)
{
    settings = new QSettings("XimalayaFM", "obs-studio");
	if (isTest())
	{
		envId = "4";
	}

    QString device;
#ifdef _WIN32
    if (sizeof(void*) == 8)
        device = "windows 64bit";
    else
        device = "windows";
#elif __APPLE__
    device = "mac";
#elif __FreeBSD__
    device = "freebsd";
#else /* assume linux for the time being */
    device = "linux";
#endif

    if (!(*settings).contains("deviceId"))
    {
        (*settings).setValue("deviceId", QUuid::createUuid().toString());
    }
    deviceId = (*settings).value("deviceId").toString();
    userAgent = "obs_v" + ver + "_c0 (" + device + ")";

	setProxy();
}

Requests::~Requests()
{
	delete settings;
}

QByteArray Requests::getString(const QNetworkRequest &request)
{
	QByteArray url("GET ");
	url.append(request.url().toString());
	blog(LOG_INFO, url);

    return processStringResult(get(request));
}

QByteArray Requests::postString(const QNetworkRequest &request, const QByteArray &data)
{
	QByteArray url("POST ");
	url.append(request.url().toString());
	blog(LOG_INFO, url);
	blog(LOG_INFO, data);

    return processStringResult(post(request, data));
}

QJsonObject Requests::getJson(const QNetworkRequest &request)
{
    return processJsonResult(getString(request));
}

QJsonObject Requests::postJson(const QNetworkRequest &request, const QByteArray &data)
{
    return processJsonResult(postString(request, data));
}

QJsonObject Requests::getXimalaya(const QUrl &url)
{
    QJsonObject result = getJson(getXimalayaRequest(url));
    return result;
}

QJsonObject Requests::postXimalaya(const QUrl &url, const QByteArray &data)
{
    QNetworkRequest request = getXimalayaRequest(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    QJsonObject result = postJson(request, data);
    return result;
}

bool Requests::checkXimalayaResult(QJsonObject result)
{
    if (result.isEmpty())
        return false;
    if (result.contains("ret") && result["ret"].isDouble())
    {
        int ret = result["ret"].toInt();
        if (ret == 50)
        {
            (*settings).remove("uid");
            (*settings).remove("token");
            (*settings).remove("nickname");
        }
        return ret == 0;
    }
    return true;
}

bool Requests::isTest()
{
	if ((*settings).contains("test"))
	{
		return (*settings).value("test").toBool();
	}
	return false;
}

QNetworkReply *Requests::get(const QNetworkRequest &request)
{
    QNetworkReply *response = manager.get(request);
    QEventLoop loop;
    connect(response, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();
    return response;
}

QNetworkReply *Requests::post(const QNetworkRequest &request, const QByteArray &data)
{
    QNetworkReply *response = manager.post(request, data);
    QEventLoop loop;
    connect(response, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();
    return response;
}

QByteArray Requests::processStringResult(QNetworkReply *response)
{
    QByteArray result;
    if (response->error() == QNetworkReply::NoError)
    {
        result = response->readAll();
        blog(LOG_INFO, result);
    }
    else
    {
		QByteArray msg;
		msg.append("code:");
		msg.append(response->attribute(QNetworkRequest::HttpStatusCodeAttribute).toString());
		msg.append(" content: ");
		msg.append(response->readAll());
        blog(LOG_ERROR, msg);
    }
    response->deleteLater();
    return result;
}

QJsonObject Requests::processJsonResult(QByteArray data)
{
    QJsonObject result;
    if (data.length() > 0)
    {
        QJsonParseError jsonpe;
        QJsonDocument json = QJsonDocument::fromJson(data, &jsonpe);
        if (jsonpe.error == QJsonParseError::NoError)
        {
            if (json.isObject())
            {
                result = json.object();
            }
            else
            {
                const char *msg("error, shoud json object");
                blog(LOG_ERROR, msg);
            }
        }
        else
        {
			QByteArray msg("error:");
			msg.append(jsonpe.errorString());
            blog(LOG_ERROR, msg);
        }
    }
    return result;
}

QNetworkRequest Requests::getXimalayaRequest(const QUrl &url)
{
    QNetworkRequest req;
    req.setUrl(url);
    req.setHeader(QNetworkRequest::UserAgentHeader, userAgent);

    QByteArray cookie;
    cookie.append(envId + "&_device=" + deviceType + "&" + deviceId + "&" + ver + ";impl=obs;channel=Ximalaya;");
    if ((*settings).contains("uid") && (*settings).contains("token"))
    {
        if ((*settings).contains("token"))
            cookie.append(envId + "&_token=" + (*settings).value("uid").toString() + "&" + (*settings).value("token").toString());
    }
    req.setRawHeader("Cookie", cookie);

    return req;
}

bool Requests::setProxy()
{
	if ((*settings).contains("proxy"))
	{
		if ((*settings).value("proxy").toBool())
		{
			QString proxyHost = (*settings).value("proxyHost").toString();
			int proxyPort = (*settings).value("proxyPort").toInt();

			QNetworkProxy proxy;
			proxy.setType(QNetworkProxy::HttpProxy);
			proxy.setHostName(proxyHost);
			proxy.setPort(proxyPort);
			manager.setProxy(proxy);
			return true;
		}
	}
	return false;
}

//RSA *Requests::createRSA(unsigned char * key, int ispublic)
//{
//	RSA *rsa = NULL;
//	BIO *keybio;
//	keybio = BIO_new_mem_buf(key, -1);
//	if (keybio == NULL)
//	{
//		printf("Failed to create key BIO");
//		return 0;
//	}
//	if (ispublic)
//	{
//		rsa = PEM_read_bio_RSA_PUBKEY(keybio, &rsa, NULL, NULL);
//	}
//	else
//	{
//		rsa = PEM_read_bio_RSAPrivateKey(keybio, &rsa, NULL, NULL);
//	}
//
//	return rsa;
//}
//int Requests::public_encrypt(unsigned char * data, int data_len, unsigned char * key, unsigned char *encrypted)
//{
//	RSA * rsa = createRSA(key, 1);
//	int result = RSA_public_encrypt(data_len, data, encrypted, rsa, RSA_PKCS1_PADDING);
//	return result;
//}
//int Requests::private_decrypt(unsigned char * enc_data, int data_len, unsigned char * key, unsigned char *decrypted)
//{
//	RSA * rsa = createRSA(key, 0);
//	int  result = RSA_private_decrypt(data_len, enc_data, decrypted, rsa, RSA_PKCS1_PADDING);
//	return result;
//}
//
//
//int Requests::private_encrypt(unsigned char * data, int data_len, unsigned char * key, unsigned char *encrypted)
//{
//	RSA * rsa = createRSA(key, 0);
//	int result = RSA_private_encrypt(data_len, data, encrypted, rsa, RSA_PKCS1_PADDING);
//	return result;
//}
//int Requests::public_decrypt(unsigned char * enc_data, int data_len, unsigned char * key, unsigned char *decrypted)
//{
//	RSA * rsa = createRSA(key, 1);
//	int  result = RSA_public_decrypt(data_len, enc_data, decrypted, rsa, RSA_PKCS1_PADDING);
//	return result;
//}

