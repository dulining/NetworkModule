#include <QString>
#include <QVector>
class CLog
{
public:
	CLog();
	~CLog();
	bool WriteLog(QString msg);
	QString m_strOutPutPath;
	QString m_strOutPutFileName;

	bool GetOutputDir();      //获取日志保存路径
	bool GetOutputFileName(); //获取日志文件名称
	bool DeleteLogFiles();    //保留近10天的日志文件

	QVector<QString> m_vecFileName;
	QVector<QString> m_vecDeleteFileName;
	
};