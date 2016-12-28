#include "obs-app.hpp"
#include "ximalaya-login-dialog.hpp"
#include "ximalayaapi.hpp"
#include "requests.hpp"

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
        (*requests.settings).setValue("uid", result["uid"].toVariant().toString());
        (*requests.settings).setValue("token", result["token"].toString());
        (*requests.settings).setValue("nickname", result["nickname"].toString());

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

