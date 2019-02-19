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
		QMessageBox::information(nullptr,QString::fromLocal8Bit("提示"),QString::fromLocal8Bit("日志保存路径创建失败"));

	if (g_WriteLog.GetOutputFileName())
		g_WriteLog.WriteLog(QString::fromLocal8Bit("获取日志文件名成功"));
	else
		g_WriteLog.WriteLog(QString::fromLocal8Bit("获取日志文件名失败"));

	if (g_WriteLog.DeleteLogFiles())
		g_WriteLog.WriteLog(QString::fromLocal8Bit("删除日志文件成功"));
	else
		g_WriteLog.WriteLog(QString::fromLocal8Bit("删除日志文件失败"));

	NetworkModule w;
	g_pNetworkModule = &w;
	w.show();
	return a.exec();
}
