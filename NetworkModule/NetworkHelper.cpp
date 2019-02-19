#include "NetworkHelper.h"
#include "CLog.h"
extern QString g_strToken;
extern CLog g_WriteLog;

NetworkHelper::NetworkHelper(QString url, QString content, ReceiveFun receiver, void* pCusData, int timeout)
{
	m_url = url;
	m_content = content;
	m_receiver = receiver;
	m_pCusdata = pCusData;
	m_timeout = timeout;

	m_pNetworkAccessManager = new QNetworkAccessManager();
	m_pTimer = new QTimer();

	connect( m_pNetworkAccessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(OnReplyFinished(QNetworkReply*)) );
	connect( m_pTimer, SIGNAL(timeout()), this, SLOT(OnTimer()) );
}

NetworkHelper::~NetworkHelper()
{
	if (m_pNetworkAccessManager)
	{
		delete m_pNetworkAccessManager;
		m_pNetworkAccessManager = nullptr;
	}
	if (m_pTimer)
	{
		delete m_pTimer;
		m_pTimer = nullptr;
	}
}

void NetworkHelper::get( const QString& url, ReceiveFun receiver, void* data, int timeout /*= LimitTimeOut*/ )
{
	NetworkHelper* p = new NetworkHelper( url, "", receiver, data, timeout );
	p->excuteGet();
}

void NetworkHelper::post( const QString& url, const QString& content, ReceiveFun receiver, void* data, int timeout /*= LimitTimeOut*/ )
{
	NetworkHelper* p = new NetworkHelper( url, content ,receiver, data, timeout );
	p->excutePost();
}

void NetworkHelper::OnReplyFinished( QNetworkReply* reply )
{
	m_pTimer->stop();
	if (reply->error() == QNetworkReply::NoError)
	{
		QByteArray bytes = reply->readAll();
		if (m_receiver)
		{
			m_receiver(E_NetOK, bytes, m_pCusdata);
		}
	}
	else
	{
		if (m_receiver)
		{
			m_receiver(E_NetReplyError, QByteArray(), m_pCusdata);
		}
		QVariant statusCodeV = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
		QString strReplyInfo = QString::number(statusCodeV.toInt()) + "," + QString::number(reply->error()) + "," + reply->errorString();
		g_WriteLog.WriteLog(strReplyInfo);
	}
	reply->deleteLater();
	this->deleteLater();
}

void NetworkHelper::OnTimer()
{
	m_pTimer->stop();
	if (m_receiver)
	{
		m_receiver( E_NetTimeOut, QByteArray(), m_pCusdata ); //超时
	}
	this->deleteLater();
}

void NetworkHelper::excuteGet()
{
	QNetworkRequest network_request;
	network_request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/json;charset=UTF-8"));
	//Token应放在请求体中，提前与服务器端约定
	QByteArray qbaUserToken(g_strToken.toStdString().c_str());
	network_request.setRawHeader("Authorization", qbaUserToken);
	network_request.setUrl( QUrl(m_url) );
	m_pNetworkAccessManager->get(network_request);
	m_pTimer->start(m_timeout);
}

void NetworkHelper::excutePost()
{
	QNetworkRequest network_request;
	network_request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/json;charset=UTF-8"));
	//Token应放在请求体中，提前与服务器端约定
	QByteArray qbaUserToken(g_strToken.toStdString().c_str());
	network_request.setRawHeader("Authorization", qbaUserToken);
	network_request.setUrl( QUrl(m_url) );
	QByteArray post_data;
	post_data.append(m_content);
	m_pNetworkAccessManager->post(network_request, post_data);
	m_pTimer->start(m_timeout);
}

