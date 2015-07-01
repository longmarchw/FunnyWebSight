#pragma once

///////////////////////////////////////////////////////////////////
class CFWebServer
{
public:
	CFWebServer(CString&);
	~CFWebServer(void);

	void InitWWW();
	void RunWWW();
	void StopWWW();
private:
	void CreatIndexHtml();
public:
	CString m_strWWW;

private:
	BOOL m_boolStop;
	struct mg_server * m_server;
};

