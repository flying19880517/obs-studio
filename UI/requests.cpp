#include "obs-app.hpp"
#include "requests.hpp"
#include <QtNetwork>
#include <QUrl>
#include <QSettings>

Requests::Requests(QObject *parent) : QObject(parent)
{
#if DEBUG
	envId = "4";
#endif

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
	deviceId = "asdf";
	userAgent = "obs_v" + ver + "_c0 (" + device + ")";

#if DEBUG
	QNetworkProxy proxy;
	proxy.setType(QNetworkProxy::HttpProxy);
	proxy.setHostName("192.168.62.43");
	proxy.setPort(8889);
	manager.setProxy(proxy);
#endif
}

QByteArray Requests::getString(const QNetworkRequest &request)
{
	QNetworkReply *response = get(request);
	QByteArray result;
	if (response->error() == QNetworkReply::NoError)
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
	if (response->error() == QNetworkReply::NoError)
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
	if (str.length() > 0)
	{
		QJsonParseError jsonpe;
		QJsonDocument json = QJsonDocument::fromJson(str, &jsonpe);
		if (jsonpe.error == QJsonParseError::NoError)
		{
			if (json.isObject())
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
	if (str.length() > 0)
	{
		QJsonParseError jsonpe;
		QJsonDocument json = QJsonDocument::fromJson(str, &jsonpe);
		if (jsonpe.error == QJsonParseError::NoError)
		{
			if (json.isObject())
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
			QSettings settings("Ximalaya", "obs");
			settings.remove("uid");
			settings.remove("token");
			settings.remove("nickname");
		}
		return ret == 0;
	}
	return true;
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
	req.setHeader(QNetworkRequest::UserAgentHeader, userAgent);

	QSettings settings("Ximalaya", "obs");
	QByteArray cookie;
	cookie.append(envId + "&_device=WP&" + deviceId + "&" + ver + ";impl=obs;channel=Ximalaya;");
	if (settings.contains("uid") && settings.contains("token"))
	{
		if (settings.contains("token"))
			cookie.append(envId + "&_token=" + settings.value("uid").toString() + "&" + settings.value("token").toString());
	}
	req.setRawHeader("Cookie", cookie);

	return req;
}
