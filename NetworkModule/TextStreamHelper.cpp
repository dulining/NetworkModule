#include "TextStreamHelper.h"
#include "NetworkModule.h"
extern NetworkModule* g_pNetworkModule;

TextStreamHelper::TextStreamHelper(const QString& url, const QString& content, TextStreamRecFun receiver, void* data)
{
	m_url = url;
	m_content = content;
	m_receiver = receiver;
	m_pCusdata = data;
}

TextStreamHelper::~TextStreamHelper()
{
	
}

void TextStreamHelper::postRequest(QVariantMap varmap, QString svc, QString cmd, TextStreamRecFun receiver, void* data)
{
	//QString szUrl =  g_pMainWindow->getServerUrl() + "?svc=" + svc+ "&cmd=" + cmd;
	QString szUrl = g_pNetworkModule->getServerUrl() + svc + cmd;
	QJsonDocument jsondoc;
	jsondoc.setObject( QJsonObject::fromVariantMap(varmap) );
	QString szContent = jsondoc.toJson();
	TextStreamHelper* p = new TextStreamHelper(szUrl, szContent, receiver, data);
	p->doPostRequest();
}

void TextStreamHelper::doPostRequest()
{
	NetworkHelper::post( m_url, m_content, OnNetworkReply, this );
}

void TextStreamHelper::getRequest(QString svc, QString cmd, TextStreamRecFun receiver, void* data)
{
	QString szUrl = g_pNetworkModule->getServerUrl() + svc + cmd;
	TextStreamHelper* p = new TextStreamHelper(szUrl, "", receiver, data);
	p->doGetRequest();
}

void TextStreamHelper::doGetRequest()
{
	NetworkHelper::get(m_url, OnNetworkReply, this);
}

void TextStreamHelper::OnNetworkReply( int errCode, const QByteArray& bytes, void* pCusData )
{
	TextStreamHelper* pTextStreamHelper = reinterpret_cast<TextStreamHelper*>(pCusData);
	
	if (!pTextStreamHelper->m_receiver)
	{
		delete pTextStreamHelper;
		return;
	}

	if (errCode==E_NetTimeOut)
	{
		pTextStreamHelper->m_receiver(E_TextStreamTimeOut, "Not received reply unitl timeout.", QVariantMap(), pTextStreamHelper->m_pCusdata);
	}
	else if (errCode==E_NetReplyError)
	{
		pTextStreamHelper->m_receiver(E_TextStreamReplyError, "Network reply error.", QVariantMap(), pTextStreamHelper->m_pCusdata);
	}
	else if (errCode==E_NetOK)
	{
		//解析接收到的json
		QJsonParseError json_error;
		QJsonDocument parse_doucment = QJsonDocument::fromJson(bytes, &json_error);
		if( json_error.error == QJsonParseError::NoError && parse_doucment.isObject() )
		{
			QJsonObject obj = parse_doucment.object();
			QVariantMap varmap = obj.toVariantMap();

			int errorCode = E_TextStreamJsonParseError;
			QString errorInfo;
			QVariantMap datamap;
			if (varmap.contains("code"))
			{
				errorCode = varmap.value("code").toInt();
			}
			if (varmap.contains("message"))
			{
				errorInfo = varmap.value("message").toString();
			}
			if (varmap.contains("data"))
			{
				datamap = varmap.value("data").toMap();
			}
			pTextStreamHelper->m_receiver(errorCode, errorInfo, datamap, pTextStreamHelper->m_pCusdata);
		}
		else
		{
			pTextStreamHelper->m_receiver(E_TextStreamJsonParseError, "Josn parse error.", QVariantMap(), pTextStreamHelper->m_pCusdata);
		}
	}
	delete pTextStreamHelper;
}

