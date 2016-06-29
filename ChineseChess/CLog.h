#pragma once
class CLog
{
public:
	CString m_LogPath, m_InitLogPath;
	CFile m_LogFile;
	void OpenLog(CString );
	void WriteLog(CString strLog, const CString& strFileName, const CString& strFunction, long LineNo);
	static CLog *GetInstance()
	{
		static CLog m_Instance;
		return &m_Instance;
	}
protected:
	void WriteFile(const CString& strLog);
	CRITICAL_SECTION m_CS;
private:
};

#define LOG(target) CLog::GetInstance()->WriteLog(target, __FILE__, __FUNCTION__, __LINE__);
