#include "obs-app.hpp"
#include "ximalaya-login-dialog.hpp"
#include "ximalayaapi.hpp"
#include "requests.hpp"

XimalayaApi::XimalayaApi(QObject *parent) : QObject(parent)
{
#if DEBUG
    baseUrl = "http://mobile.test.ximalaya.com";
    flyUrl = "http://192.168.3.131:2900/fly";
#endif
}

//XimalayaApi api;
//QString msg;
//if (!api.loginXimalaya(&msg))
//{
//	QMessageBox::warning(this, "登录失败", msg);
//}
//bool XimalayaApi::checkLogin(QString *msg)
//{
//	QSettings settings("Ximalaya", "obs");
//	if (settings.contains("token"))
//	{
//		if (getHomePage(msg))
//		{
//			return true;
//		}
//	}
//	return loginXimalaya(msg);
//}

bool XimalayaApi::login(QString username, QString password, QString *msg)
{
    //        QJsonObject result = requests.getXimalaya(QUrl(baseUrl + "/passport/token/login"));
    //        if(result.isEmpty())
    //        {
    //            QMessageBox::warning(this,tr("warning"),tr("error"),QMessageBox::Yes);
    //            return;
    //        }
    //        if(!result.contains("token"))
    //        {
    //            QMessageBox::warning(this,tr("warning"),tr("no login token"),QMessageBox::Yes);
    //            return;
    //        }
    //        QString token = result["token"].toString();

    QByteArray data;
    data.append("account=");
    data.append(username);
    data.append("&password=");
    data.append(password);

    QJsonObject result = requests.postXimalaya(QUrl(baseUrl + "/mobile/login"), data);
    if (requests.checkXimalayaResult(result))
    {
        //result.insert("uid","12228");
        //result.insert("token","9fd8ab5c0d32304e62089610cfb89cb68589");
        //result.insert("nickname","Lionheart");
        QSettings settings("Ximalaya", "obs");
        settings.setValue("uid", result["uid"].toVariant().toString());
        settings.setValue("token", result["token"].toString());
        settings.setValue("nickname", result["nickname"].toString());

        return true;
    }
    if (result.contains("msg"))
        *msg = result["msg"].toString();
    else
        *msg = QTStr("Ximalaya.Api.LoginFailed");
    return false;
}

bool XimalayaApi::logout()
{
    QSettings settings("Ximalaya", "obs");
    if (settings.contains("token"))
    {
        requests.getXimalaya(QUrl(baseUrl + "/mobile/logout"));

        settings.remove("uid");
        settings.remove("token");
        settings.remove("nickname");
    }
    return true;
}

bool XimalayaApi::liveCreate(QString title, QString categoryId, QString *msg) 
{
    QSettings settings("Ximalaya", "obs");

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
        settings.setValue("liveId", liveId);
        settings.setValue("roomId", roomId);
        settings.setValue("liveTitle", title);
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
    QSettings settings("Ximalaya", "obs");

    QString uid = settings.value("uid").toString();
    QString liveId = settings.value("liveId").toString();
    QString roomId = settings.value("roomId").toString();

    QUrl url(flyUrl);
    QUrlQuery query;
    query.addQueryItem("anchorId", uid);
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
        realPushUrlQuery.addQueryItem("token", settings.value("token").toString());
        realPushUrlQuery.addQueryItem("userId", uid);
        realPushUrlQuery.addQueryItem("anchorId", uid);
        realPushUrl.setQuery(realPushUrlQuery);

        settings.setValue("server", realPushUrl.toString());
        settings.setValue("key", pushtoken);
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
    QSettings settings("Ximalaya", "obs");

    QString liveId = settings.value("liveId").toString();
    QString roomId = settings.value("roomId").toString();

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
    QSettings settings("Ximalaya", "obs");
    Requests requests;
    QByteArray stop;
    if (liveId.length() > 0)
    {
        stop.append("id=");
        stop.append(liveId);
    }
    requests.postXimalaya(QUrl(baseUrl + "/lamia/v1/live/record/stop"), stop);
    if (liveId.length() > 0)
    {
        settings.remove("liveId");
        settings.remove("roomId");
		settings.remove("liveTitle");
		settings.remove("server");
		settings.remove("key");
    }
    return true;
}

bool XimalayaApi::checkLogin()
{
    QSettings settings("Ximalaya", "obs");
    if (!settings.contains("token"))
        return false;
    QJsonObject result = requests.getXimalaya(QUrl(baseUrl + "/mobile/homePage"));

    if (requests.checkXimalayaResult(result))
    {
        settings.setValue("nickname", result["nickname"].toString());
        return true;
    }
    return settings.contains("token");
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

