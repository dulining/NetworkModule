#include "CLog.h"
#include <io.h>  
#include <Shlobj.h>  
#include <QDateTime>
#include <QDir>
#include <QFileInfo>

const long DateInterval = 9 * 24 * 60 * 60;  //9天的秒数

CLog::CLog()
{
	m_vecFileName.clear();
	m_vecDeleteFileName.clear();
}
CLog::~CLog()
{
}
bool CLog::GetOutputDir()
{
	wchar_t appdata[MAX_PATH];
	SHGetFolderPathW(NULL, CSIDL_APPDATA, NULL, SHGFP_TYPE_CURRENT, appdata);
	int iLen = WideCharToMultiByte(CP_ACP, 0, appdata, -1, NULL, 0, NULL, NULL);
	char* chRtn = new char[iLen*sizeof(char)];
	WideCharToMultiByte(CP_ACP, 0, appdata, -1, chRtn, iLen, NULL, NULL);
	std::string strAppdata(chRtn);
	if (chRtn)
	{
		delete chRtn;
		chRtn = nullptr;
	}
	QString path = QString::fromLocal8Bit(strAppdata.data()) + "\\interactopenclass\\logs";
	QDir dir(path);
	if (!dir.exists())
	{
		bool suc = dir.mkpath(path);//创建多级目录
		if (!suc)
		{
			return false;
		}
	}
	m_strOutPutPath = path;
	return true;
}
bool CLog::GetOutputFileName()
{
	if (!m_strOutPutPath.isEmpty())
	{
		QString strDate = QDate::currentDate().toString("yyyy-MM-dd");
		char szFileName[MAX_PATH] = { 0 };
		sprintf(szFileName, "%s\\%s.log", m_strOutPutPath.toStdString().c_str(),strDate.toStdString().c_str());
  	    m_strOutPutFileName = szFileName;
		return true;
	}
	else
	{
		return false;
	}
}
bool CLog::DeleteLogFiles()
{
	QDir dir(m_strOutPutPath);
	QFileInfoList fileList;
	if (!dir.exists())
	{
		return false;
	}
	dir.setSorting(QDir::Time);
	fileList = dir.entryInfoList(QDir::Files);
	foreach(QFileInfo curFile, dir.entryInfoList())
	{
		if (curFile.isFile() && (curFile.suffix() == "log"))
		{
			m_vecFileName.push_back(curFile.fileName().left(10));
		}
	}

	QDateTime curDate = QDateTime::fromString(m_strOutPutFileName.right(14).left(10), "yyyy-MM-dd");
    long nCurDate = curDate.toTime_t();
	for (int i = 0; i < m_vecFileName.size(); i++)
	{
		long nDate = QDateTime::fromString(m_vecFileName[i], "yyyy-MM-dd").toTime_t();
		if (nCurDate - nDate > DateInterval)
		{
			m_vecDeleteFileName.push_back(m_vecFileName[i]);
		}
	}

	QString fileName = "";
	for (int i = 0; i < m_vecDeleteFileName.size(); i++)
	{
		fileName = m_vecDeleteFileName[i] + ".log";
		dir.remove(fileName);
	}
	return true;
}
bool CLog::WriteLog(QString msg)
{
	QString strContent = "";
	QString strDateTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss ");
    strContent = strDateTime + msg + "\r\n";
	QFile file(m_strOutPutFileName);
	if (file.open(QIODevice::WriteOnly | QIODevice::Append))
	{
		file.write(strContent.toLocal8Bit());
		file.flush();
		file.close();
		return true;
	}
	else
	{
		return false;
	}
}
