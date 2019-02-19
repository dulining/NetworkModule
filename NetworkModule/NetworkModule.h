#pragma once

#include <QtWidgets/QDialog>
#include "ui_NetworkModule.h"

struct LiveInfo
{
	int sortNumber;
	long long id;
	QString userID;
	QString name;
	QString pushURL;
	QString startTime;
	QString endTime;
	QString descript;
	QString streamId;
	QString busId;
	int liveStatus;
};

class NetworkModule : public QDialog
{
	Q_OBJECT

public:
	NetworkModule(QWidget *parent = Q_NULLPTR);
	QString getServerUrl();
private:
	//static void OnLogin(int errorCode, QString errorInfo, QVariantMap datamap, void* pCusData);
	static void OnConfig(int errorCode, QString errorInfo, QVariantMap datamap, void* pCusData);
	static void OnUser(int errorCode, QString errorInfo, QVariantMap datamap, void* pCusData);
	static void OnList(int errorCode, QString errorInfo, QVariantMap datamap, void* pCusData);
	static void OnPicDown(int errorCode, QString errorInfo, QString picPath, void* pCusData);
private slots:
	void on_loginButton_clicked();
	void on_configButton_clicked();
	void on_userButton_clicked();
	void on_listButton_clicked();
	void on_picDownButton_clicked();
private:
	Ui::NetworkModuleClass ui;
	//QString m_strToken;
	QString m_strServerUrl;
	std::vector<LiveInfo> m_vecLiveInfo;
	QString m_strPicPath;
};
