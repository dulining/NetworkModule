#pragma once
#include "TextStreamHelper.h"
#include "NetworkHelper.h"
#include <QtCore>

enum E_NormalNetworkRelpy
{
	E_TextStreamOK,
	E_TextStreamTimeOut,//³¬Ê±
	E_TextStreamReplyError,
	E_TextStreamJsonParseError,//Json½âÎö´íÎó
};

typedef void (*TextStreamRecFun)(int errorCode, QString errorInfo, QVariantMap datamap, void* pCusData);

class TextStreamHelper
{
public:
	static void postRequest(QVariantMap varmap, QString svc, QString cmd, TextStreamRecFun receiver, void* data);
	static void getRequest(QString svc, QString cmd, TextStreamRecFun receiver, void* data);
private:
	TextStreamHelper(const QString& url, const QString& content, TextStreamRecFun receiver, void* data);
	~TextStreamHelper();
	void doPostRequest();
	void doGetRequest();

	static void OnNetworkReply(int errCode, const QByteArray& bytes, void* pCusData);

private:
	QString	  m_url;
	QString	  m_content;
	TextStreamRecFun m_receiver;
	void*	  m_pCusdata;
};

