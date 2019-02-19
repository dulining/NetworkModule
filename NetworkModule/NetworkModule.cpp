#include "NetworkModule.h"
#include "TextStreamHelper.h"
#include "PicDownHelper.h"
#include <QMessageBox>
#include "CLog.h"
QString g_strToken;
CLog g_WriteLog;

NetworkModule::NetworkModule(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	m_strServerUrl = "https://darenkeji.test.100doc.com.cn/api30/yibai-web/";
	PicDownHelper::setPicPath("D:/PicDown/dln/");
	PicDownHelper::clearPicCache();
}
void NetworkModule::on_loginButton_clicked()
{
	QString strUserName = "zhagnsan";
	QString strPwd = "e10adc3949ba59abbe56e057f20f883e";

	//QVariantMap varmap;
	//varmap.insert("username", strUserName);
	//varmap.insert("password", strPwd);
	//TextStreamHelper::postRequest(varmap, "", "user/login", OnLogin, this);


	QTimer timer;
	timer.setInterval(LimitTimeOut);
	timer.setSingleShot(true);
	QJsonObject postJson;
	QNetworkAccessManager Manager;
	QString szUrl = m_strServerUrl + "user/login";
	QUrl url(szUrl);
	QNetworkRequest qnr(url);
	qnr.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/json;charset=UTF-8"));
	qnr.setHeader(QNetworkRequest::UserAgentHeader, QVariant("YibaiPCClient"));
	postJson.insert("username", strUserName);
	postJson.insert("password", strPwd);

	QJsonDocument document;
	document.setObject(postJson);
	QByteArray byteArray = document.toJson(QJsonDocument::Compact);
	QNetworkReply* reply = Manager.post(qnr, byteArray);

	QEventLoop eventLoop;
	connect(&timer, SIGNAL(timeout()), &eventLoop, SLOT(quit()));
	connect(reply, &QNetworkReply::finished, &eventLoop, &QEventLoop::quit);
	timer.start();
	eventLoop.exec(QEventLoop::ExcludeUserInputEvents);

	if (timer.isActive())
	{
		timer.stop();
		if (reply->error() != QNetworkReply::NoError)
		{
			QMessageBox::information(nullptr,QString::fromLocal8Bit("提示"),QString::fromLocal8Bit("密码登录接口请求服务器异常"));
		}
		else
		{
			QByteArray data = reply->readAll();
			QJsonParseError jsonError;
			QJsonDocument json = QJsonDocument::fromJson(data, &jsonError);
			if (jsonError.error == QJsonParseError::NoError)
			{
				if (json.isObject())
				{
					QJsonObject obj = json.object();
					QJsonValue Jsvalue;
					if (obj.contains("code"))
					{
						Jsvalue = obj.value("code");
						int nCode = Jsvalue.toVariant().toInt();
						if (0 == nCode)
						{
							g_WriteLog.WriteLog(QString::fromLocal8Bit("登录接口user/login成功"));
							if (obj.contains("data"))
							{
								Jsvalue = obj.value("data");
								g_strToken = Jsvalue.toVariant().toString().trimmed();
								QMessageBox::information(nullptr, "Token", g_strToken);
							}
						}
						else
						{
							if (obj.contains("message"))
							{
								Jsvalue = obj.value("message");
								QString str = Jsvalue.toVariant().toString();
							    QMessageBox::information(nullptr, QString::fromLocal8Bit("提示"), QString::fromUtf8(str.toStdString().data()));
							}
							else
							{
								QString strErrInfo = QStringLiteral("错误码：") + QString::number(nCode);
								QMessageBox::information(nullptr, QString::fromLocal8Bit("提示"), strErrInfo);
							}
						}
					}
				}
			}
			else
			{
				QMessageBox::information(nullptr, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("密码登录接口返回的数据转换错误"));
			}
		}
	}
	else
	{
		disconnect(reply, &QNetworkReply::finished, &eventLoop, &QEventLoop::quit);
		QMessageBox::information(nullptr, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("密码登录接口请求服务器超时"));
	}
	reply->abort();
	reply->deleteLater();
	reply = nullptr;
}

//void NetworkModule::OnLogin(int errorCode, QString errorInfo, QVariantMap datamap, void* pCusData)
//{
//
//	if (errorCode == E_TextStreamNetworkOK)
//	{
//		QMessageBox::information(nullptr, "Title","success");
//		if (datamap.contains("token"))
//		{
//			//pNetworkModule->m_strToken = datamap.value("token").toString();
//		}
//	}
//}

void NetworkModule::on_configButton_clicked()
{
	TextStreamHelper::postRequest(QVariantMap(), "config", "/get", OnConfig, this);
}

void NetworkModule::OnConfig(int errorCode, QString errorInfo, QVariantMap datamap, void* pCusData)
{
	g_WriteLog.WriteLog("config/get, Code:" + QString::number(errorCode) + " Info: " + errorInfo);
	QString strReg, strPlatName;
	if (errorCode == E_TextStreamOK)
	{
		if (datamap.contains("registerVisible"))
		{
			strReg = datamap.value("registerVisible").toString();
		}
		if (datamap.contains("platomName"))
		{
			strPlatName = datamap.value("platomName").toString();
		}
	}
	QMessageBox::information(nullptr,"retPlat",strReg + strPlatName);
}

void NetworkModule::on_userButton_clicked()
{
	TextStreamHelper::getRequest("user","/",OnUser,this);
}

void NetworkModule::OnUser(int errorCode, QString errorInfo, QVariantMap datamap, void* pCusData)
{
	g_WriteLog.WriteLog("user/, Code:" + QString::number(errorCode) + " Info: " + errorInfo);
	QString strUserId, strNickName;
	if (errorCode == E_TextStreamOK)
	{
		if (datamap.contains("userId"))
		{
			strUserId = datamap.value("userId").toString();
		}
		if (datamap.contains("nickname"))
		{
			strNickName = datamap.value("nickname").toString();
		}
	}
	QMessageBox::information(nullptr, "userInfo", strUserId + strNickName);
}

void NetworkModule::on_listButton_clicked()
{
	QVariantMap varmap;
	varmap.insert("pageNum", 1);
	varmap.insert("pageSize", 50);

	QVariantMap busimap;
	busimap.insert("businessId", 2);
	varmap.insert("params", busimap);

	TextStreamHelper::postRequest(varmap, "video", "/source/push/list", OnList, this);
}

void NetworkModule::OnList(int errorCode, QString errorInfo, QVariantMap datamap, void* pCusData)
{
	g_WriteLog.WriteLog("video/source/push/list, Code:" + QString::number(errorCode) + " Info: " + errorInfo);
	NetworkModule* pNetworkModule = reinterpret_cast<NetworkModule*>(pCusData);
	pNetworkModule->m_vecLiveInfo.clear();
	if (errorCode == E_TextStreamOK)
	{
		if (datamap.contains("data"))
		{
			QVariantList dataList = datamap.value("data").toList();
			for (int i = 0; i < dataList.size(); i++)
			{
				LiveInfo liveInfo;
				QVariantMap livemap = dataList[i].toMap();
				liveInfo.busId = livemap.value("busId").toString();
				liveInfo.name = livemap.value("name").toString();
				liveInfo.id = livemap.value("id").toLongLong();		
				liveInfo.userID = livemap.value("userId").toString();
				liveInfo.pushURL = livemap.value("pushUrl").toString();
				long long nStartTime = livemap.value("startTime").toLongLong();
				long long nEndTime = livemap.value("endTime").toLongLong();
				QString strStartTime = QDateTime::fromMSecsSinceEpoch(nStartTime).toString("yyyy.MM.dd hh:mm");
				QString strEndTime = QDateTime::fromMSecsSinceEpoch(nEndTime).toString("yyyy.MM.dd hh:mm");
				liveInfo.startTime = strStartTime;
				liveInfo.endTime = strEndTime;
				liveInfo.descript = livemap.value("description").toString();
				liveInfo.liveStatus = livemap.value("status").toInt();
				liveInfo.streamId = livemap.value("streamId").toString();

				pNetworkModule->m_vecLiveInfo.push_back(liveInfo);
			}
		}
	}
	QMessageBox::information(nullptr, "userInfo", QString::fromLocal8Bit("获取直播列表信息成功"));
}

void NetworkModule::on_picDownButton_clicked()
{
	m_strPicPath = PicDownHelper::getPicPath();
	QString picUrl = "http://poster-1253412830.cosbj.myqcloud.com/0/8485cbe3-22ff-496f-bd6f-c1c597631f74.png";
	PicDownHelper::getPic(picUrl, "picDown.png", OnPicDown, this);
}

void NetworkModule::OnPicDown(int errorCode, QString errorInfo, QString picPath, void* pCusData)
{
	g_WriteLog.WriteLog("getPic, Code:" + QString::number(errorCode) + " Info: " + errorInfo);
	if (errorCode == E_PicDownOK)
	{
		QMessageBox::information(nullptr, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("图片下载成功"));
	}
	else
	{
		QMessageBox::information(nullptr, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("图片下载失败"));
	}
}

QString NetworkModule::getServerUrl()
{
	return m_strServerUrl;
}

