#include "ximalaya-login-dialog.hpp"
#include "ximalayaapi.hpp"
#include "requests.hpp"

XimalayaApi::XimalayaApi(QObject *parent) : QObject(parent)
{

}

bool XimalayaApi::checkLogin(QString *msg)
{
    QSettings settings("Ximalaya", "obs");
    if(settings.contains("token"))
    {
        if(getHomePage(msg))
        {
            return true;
        }
    }
    return loginXimalaya(msg);
}
bool XimalayaApi::loginXimalaya(QString *msg)
{
    LoginDialog dlg;
    return dlg.exec() == QDialog::Accepted;
}

bool XimalayaApi::login(QString username, QString password, QString *msg)
{
    Requests requests;
//        QJsonObject result = requests.getXimalaya(QUrl("http://mobile.ximalaya.com/passport/token/login"));
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
    QJsonObject result = requests.postXimalaya(QUrl("http://mobile.test.ximalaya.com/mobile/login"), data);
	if (result.isEmpty() || !result.contains("token"))
	{
		if (result.contains("msg"))
			*msg = result["msg"].toString();
		else
			*msg = "登录失败";
		return false;
	}
	//result.insert("uid","12228");
	//result.insert("token","9fd8ab5c0d32304e62089610cfb89cb68589");
	//result.insert("nickname","Lionheart");
    QSettings settings("Ximalaya", "obs");
	settings.setValue("uid", result["uid"].toVariant().toString());
    settings.setValue("token", result["token"].toString());
    settings.setValue("nickname", result["nickname"].toString());

	return true;
}

bool XimalayaApi::logout()
{
    Requests requests;
    requests.getXimalaya(QUrl("http://mobile.test.ximalaya.com/mobile/logout"));

    QSettings settings("Ximalaya", "obs");
    settings.remove("uid");
    settings.remove("token");
    settings.remove("nickname");
    return true;
}

bool XimalayaApi::liveStart(QString *server, QString *key, QString *msg)
{
	Requests requests;
	QSettings settings("Ximalaya", "obs");

	XimalayaApi api;
	if (!api.checkLogin(msg))
	{
		return false;
	}
	QByteArray create;
	create.append("name=");
	create.append(QUrl::toPercentEncoding(settings.value("nickname").toString()));
	create.append(QUrl::toPercentEncoding("的直播间"));
	create.append("&create=-1&permissionType=1&categoryId=24");
	QJsonObject json = requests.postXimalaya(QUrl("http://mobile.test.ximalaya.com/lamia/v1/live/record/create"), create);
	if (json.isEmpty() || !json.contains("data"))
	{
		if (json.contains("msg"))
			*msg = json["msg"].toString();
		else
			*msg = "创建直播失败";
		return false;
	}
	QJsonObject jsonCreate = json["data"].toObject();
	if (jsonCreate.isEmpty() || !jsonCreate.contains("id") || !jsonCreate.contains("roomId"))
	{
		if (jsonCreate.contains("msg"))
			*msg = json["msg"].toString();
		else
			*msg = "无法获取直播Id";
		return false;
	}
	QString liveId = jsonCreate["id"].toVariant().toString();
	QString roomId = jsonCreate["roomId"].toVariant().toString();
	QString uid = settings.value("uid").toString();
	settings.setValue("liveId", liveId);
	settings.setValue("roomId", roomId);
	QUrl url("http://192.168.3.131:2900/fly");
	QUrlQuery query;
	query.addQueryItem("anchorId", uid);
	query.addQueryItem("liveId", liveId);
	query.addQueryItem("userId", uid);
	query.addQueryItem("userType", "anchor");
	url.setQuery(query);
	json = requests.getXimalaya(url);
	if (json.isEmpty() || !json.contains("pushurl") || json["pushurl"].toArray().size() == 0 || !json.contains("token"))
	{
		if (json.contains("msg"))
			*msg = json["msg"].toString();
		else
			*msg = "无法获取推流地址";
		return false;
	}
	QString pushurl = json["pushurl"].toArray().first().toString();
	QString pushtoken = json["token"].toString();

	QByteArray start;
	start.append("id=");
	start.append(liveId);
	start.append("&roomId=");
	start.append(roomId);
	json = requests.postXimalaya(QUrl("http://mobile.test.ximalaya.com/lamia/v1/live/record/start"), start);
	if (json.isEmpty())
	{
		if (json.contains("msg"))
			*msg = json["msg"].toString();
		else
			*msg = "无法开始直播...";
		return false;
	}
	QUrl realPushUrl(pushurl);
	QUrlQuery realPushUrlQuery;
	realPushUrlQuery.addQueryItem("token", settings.value("token").toString());
	realPushUrlQuery.addQueryItem("userId", uid);
	realPushUrlQuery.addQueryItem("anchorId", uid);
	realPushUrl.setQuery(realPushUrlQuery);
	qDebug() << server;
	qDebug() << key;
	*server = realPushUrl.toString();
	*key = pushtoken;
	return true;
}

bool XimalayaApi::liveStop()
{
	QSettings settings("Ximalaya", "obs");
	if (!settings.contains("liveId"))
		return false;

	QString liveId = settings.value("liveId").toString();
	Requests requests;
	QByteArray stop;
	stop.append("id=");
	stop.append(liveId);
	requests.postXimalaya(QUrl("http://mobile.test.ximalaya.com/lamia/v1/live/record/stop"), stop);

	settings.remove("liveId");
	settings.remove("roomId");

	return true;
}

bool XimalayaApi::getHomePage(QString *msg)
{
    QJsonObject json = requests.getXimalaya(QUrl("http://mobile.test.ximalaya.com/mobile/homePage"));

    QSettings settings("Ximalaya", "obs");
    if(json.isEmpty())
    {
        if(settings.contains("token"))
        {
//            QMessageBox::warning(this, "warning", "error", QMessageBox::Yes);
            return false;
        }
        else
        {
            return loginXimalaya(msg);
        }
    }
    settings.setValue("nickname", json["nickname"].toString());
    return true;
}

