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

	bool GetOutputDir();      //��ȡ��־����·��
	bool GetOutputFileName(); //��ȡ��־�ļ�����
	bool DeleteLogFiles();    //������10�����־�ļ�

	QVector<QString> m_vecFileName;
	QVector<QString> m_vecDeleteFileName;
	
};