#include "main.h"


#include <Windows.h>
#include <Stdio.h>
#include <Tchar.h>



VOID A(LPTSTR lpDir)
{
    HANDLE                                hListFile;
    TCHAR                                szFileName[MAX_PATH];
    TCHAR                                szSubPath[MAX_PATH];
    WIN32_FIND_DATA                fnd;
    HANDLE                                hFile;
    SYSTEMTIME                        localTime;
    SYSTEMTIME                        sysTime;
    FILETIME                        fileTime;
    TCHAR                                szMsg[512];

    lstrcpy(szFileName, lpDir);
    lstrcat(szFileName, _T("\\*"));

    hListFile = FindFirstFile(szFileName, &fnd);
    if (hListFile != INVALID_HANDLE_VALUE)
    {
        do
        {
            if (lstrcmp(fnd.cFileName, _T(".")) == 0 ||
                lstrcmp(fnd.cFileName, _T("..")) == 0)
                continue;

            lstrcpy(szSubPath, lpDir);
            lstrcat(szSubPath, _T("\\"));
            lstrcat(szSubPath, fnd.cFileName);

            ZeroMemory(&localTime, sizeof(SYSTEMTIME));
            localTime.wYear = 2022;//2013年
            localTime.wMonth = 4;//12月
            localTime.wDay = 18;//21日
            localTime.wHour = 16;
            localTime.wMinute = 35;
            localTime.wSecond = 37;
            TzSpecificLocalTimeToSystemTime(NULL, &localTime, &sysTime);//加这一句是为了让时间正好是2013年12月21日 0点0分0秒
            SystemTimeToFileTime(&sysTime, &fileTime);//系统时间到文件时间
            if (fnd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)//文件是目录
            {
                A(szSubPath);//遍历子目录

                hFile = CreateFile(szSubPath,
                    FILE_WRITE_ATTRIBUTES,//这里只给一个写入文件属性的权限
                    FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,//尽可能不影响其它文件
                    NULL,
                    OPEN_EXISTING,//打开已经存在的文件
                    FILE_FLAG_BACKUP_SEMANTICS,
                    NULL);
            }
            else
            {
                hFile = CreateFile(szSubPath,
                    FILE_WRITE_ATTRIBUTES,//这里只给一个写入文件属性的权限
                    FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,//尽可能不影响其它文件
                    NULL,
                    OPEN_EXISTING,//打开已经存在的文件
                    FILE_ATTRIBUTE_NORMAL,
                    NULL);
            }
            if (hFile != INVALID_HANDLE_VALUE)
            {
                SetFileTime(hFile,
                    &fileTime,
                    &fileTime,
                    &fileTime);//创建时间、访问时间、修改时间都一样
                CloseHandle(hFile);
            }
            else//保证不遗漏任何一个文件或者目录
            {
                wsprintf(szMsg, _T("无法访问 %s 文件"), szSubPath);
                MessageBox(NULL, szMsg, _T("注意"), MB_ICONWARNING);
            }
        } while (FindNextFile(hListFile, &fnd));

        FindClose(hListFile);
    }
}

int _tmain(int argc, TCHAR* argv[])
{
    A((LPTSTR)_T("./"));//要遍历的目录
    return 0;
}