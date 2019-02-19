#include "NetworkModule.h"
#include <QtWidgets/QApplication>
#include "CLog.h"
#include <QMessageBox>

extern CLog g_WriteLog;
NetworkModule* g_pNetworkModule = NULL;
int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	if (!g_WriteLog.GetOutputDir())
		QMessageBox::information(nullptr,QString::fromLocal8Bit("��ʾ"),QString::fromLocal8Bit("��־����·������ʧ��"));

	if (g_WriteLog.GetOutputFileName())
		g_WriteLog.WriteLog(QString::fromLocal8Bit("��ȡ��־�ļ����ɹ�"));
	else
		g_WriteLog.WriteLog(QString::fromLocal8Bit("��ȡ��־�ļ���ʧ��"));

	if (g_WriteLog.DeleteLogFiles())
		g_WriteLog.WriteLog(QString::fromLocal8Bit("ɾ����־�ļ��ɹ�"));
	else
		g_WriteLog.WriteLog(QString::fromLocal8Bit("ɾ����־�ļ�ʧ��"));

	NetworkModule w;
	g_pNetworkModule = &w;
	w.show();
	return a.exec();
}
