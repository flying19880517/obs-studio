#include "requests.hpp"
#include <QtNetwork>
#include <QUrl>
#include <QSettings>

Requests::Requests(QObject *parent) : QObject(parent)
{

}

QByteArray Requests::getString(const QNetworkRequest &request)
{
    QNetworkReply *response = get(request);
    QByteArray result;
    if(response->error() == QNetworkReply::NoError)
    {
        result = response->readAll();
        qDebug(result);
    }
    else
    {
        qDebug() << "response code" << response->attribute(QNetworkRequest::HttpStatusCodeAttribute) << " content:" << response->readAll();
    }
    response->deleteLater();
    return result;
}

QByteArray Requests::postString(const QNetworkRequest &request, const QByteArray &data)
{
    QNetworkReply *response = post(request, data);
    QByteArray result;
    if(response->error() == QNetworkReply::NoError)
    {
        result = response->readAll();
        qDebug(result);
    }
    else
    {
        qDebug() << "response code" << response->attribute(QNetworkRequest::HttpStatusCodeAttribute) << " content:" << response->readAll();
    }
    response->deleteLater();
    return result;
}

QJsonObject Requests::getJson(const QNetworkRequest &request)
{
    QByteArray str = getString(request);
    QJsonObject result;
    if(str.length() > 0)
    {
        QJsonParseError jsonpe;
        QJsonDocument json = QJsonDocument::fromJson(str, &jsonpe);
        if(jsonpe.error==QJsonParseError::NoError)
        {
            if(json.isObject())
            {
                result = json.object();
            }
            else
            {
                qDebug() << "error, shoud json object";
            }
        }
        else
        {
            qDebug() << "error:" << jsonpe.errorString();
        }
    }
    return result;
}

QJsonObject Requests::postJson(const QNetworkRequest &request, const QByteArray &data)
{
    QByteArray str = postString(request, data);
    QJsonObject result;
    if(str.length() > 0)
    {
        QJsonParseError jsonpe;
        QJsonDocument json = QJsonDocument::fromJson(str, &jsonpe);
        if(jsonpe.error==QJsonParseError::NoError)
        {
            if(json.isObject())
            {
                result = json.object();
            }
            else
            {
                qDebug() << "error, shoud json object";
            }
        }
        else
        {
            qDebug() << "error:" << jsonpe.errorString();
        }
    }
    return result;
}

QJsonObject Requests::getXimalaya(const QUrl &url)
{
    QJsonObject result = getJson(getXimalayaRequest(url));
    if(!result.isEmpty())
    {
        if(result.contains("ret") && result["ret"].isDouble())
        {
            int ret = result["ret"].toInt();
            if(ret != 0)
            {
                result = QJsonObject();
            }
        }
    }
    return result;
}

QJsonObject Requests::postXimalaya(const QUrl &url, const QByteArray &data)
{
	QNetworkRequest request = getXimalayaRequest(url);
	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    QJsonObject result = postJson(request, data);
    if(!result.isEmpty())
    {
        if(result.contains("ret") && result["ret"].isDouble())
        {
            int ret = result["ret"].toInt();
            if(ret != 0)
            {
                result = QJsonObject();
            }
        }
    }
    return result;
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

QNetworkRequest Requests::getXimalayaRequest(const QUrl &url)
{
    QNetworkRequest req;
    req.setUrl(url);
    req.setRawHeader("User-Agent", "ting_v1.0.6_c0 (WP 8.0.10328.0,NOKIA RM-822_apac_vietnam_224)");

    QSettings settings("Ximalaya", "obs");
    if(settings.contains("uid") && settings.contains("token"))
    {
        QByteArray cookie;
        cookie.append("4&_device=WP&asef&1.0.0;channel=Ximalaya;impl=obs;");
        cookie.append("4&_token=");
        cookie.append(settings.value("uid").toString());
        cookie.append("&");
        cookie.append(settings.value("token").toString());
        req.setRawHeader("Cookie", cookie);
    }
    return req;
}
