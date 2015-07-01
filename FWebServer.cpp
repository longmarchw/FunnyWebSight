#include "StdAfx.h"
#include "FWebServer.h"
#include "D:\Long\GitHub\mongoose\mongoose\mongoose.h"
CRITICAL_SECTION g_csBMPFile;
const CString strIndexFileContent("<html>\
									   <body>\
									   <script type=\"text/javascript\">\
									   function changePicture(imgId){\
									   document.getElementById(imgId).src = \"image/Temp.bmp\" + \"?rnd=\" + Math.random();\
									   }\
									   setInterval (function(){changePicture(\"flowImage\")}, 1000) ;\
									   </script>\
									   <form name=\"myForm\">\
									   <center><img id=\"flowImage\" src=\"image/Temp.bmp\"/></center>\
									   </form>\
									   </body>\
									   </html>");

static int ev_handler(struct mg_connection *conn, enum mg_event ev) {
  switch (ev) {
    case MG_AUTH: return MG_TRUE;
    case MG_REQUEST:
		if(!strcmp(conn->uri, "/image/Temp.bmp")) {
			//EnterCriticalSection(&g_csBMPFile);
			TRACE("MG_REQUEST enter...");
			mg_send_file(conn, "image/Temp.bmp", NULL);
			TRACE("MG_REQUEST leave...");
			//LeaveCriticalSection(&g_csBMPFile);
		}
		else
		{
			mg_send_file(conn, "index.html", NULL);
		}
		return MG_MORE;
    default: return MG_FALSE;
  }
}

void CFWebServer:: InitWWW()
{
	CreatIndexHtml();
	// Create and configure the server
	m_server = mg_create_server(NULL, ev_handler);
	mg_set_option(m_server, "listening_port", "8080");
	mg_set_option(m_server, "document_root", (LPSTR)(LPCTSTR)m_strWWW);
}

void CFWebServer::CreatIndexHtml()
{
	CFileFind cff;
	if(!cff.FindFile(m_strWWW + "index.html")){
		HANDLE hFile = CreateFile(m_strWWW + "index.html", GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		DWORD dwWrites = 0;
		char szANSIString [10000];
		WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, strIndexFileContent, strIndexFileContent.GetLength(), szANSIString, sizeof(szANSIString), NULL, NULL);
		BOOL blRet = WriteFile(hFile, szANSIString, strIndexFileContent.GetLength() , &dwWrites, NULL);  
		CloseHandle(hFile); 
		BOOL ret = SetFileAttributes (m_strWWW + "index.html", FILE_ATTRIBUTE_HIDDEN );
	}
}
void CFWebServer:: RunWWW()
{
	if(m_server)
	{
		mg_poll_server(m_server, 1000);
	}
}

void CFWebServer:: StopWWW()
{
	if(m_server)
	{
		// Cleanup, and free server instance
		mg_destroy_server(&m_server);
	}
}

CFWebServer::CFWebServer(CString& workPath)
{
	m_strWWW = workPath;
}


CFWebServer::~CFWebServer(void)
{
	return;
}

