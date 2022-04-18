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
            localTime.wYear = 2022;//2013��
            localTime.wMonth = 4;//12��
            localTime.wDay = 18;//21��
            localTime.wHour = 16;
            localTime.wMinute = 35;
            localTime.wSecond = 37;
            TzSpecificLocalTimeToSystemTime(NULL, &localTime, &sysTime);//����һ����Ϊ����ʱ��������2013��12��21�� 0��0��0��
            SystemTimeToFileTime(&sysTime, &fileTime);//ϵͳʱ�䵽�ļ�ʱ��
            if (fnd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)//�ļ���Ŀ¼
            {
                A(szSubPath);//������Ŀ¼

                hFile = CreateFile(szSubPath,
                    FILE_WRITE_ATTRIBUTES,//����ֻ��һ��д���ļ����Ե�Ȩ��
                    FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,//�����ܲ�Ӱ�������ļ�
                    NULL,
                    OPEN_EXISTING,//���Ѿ����ڵ��ļ�
                    FILE_FLAG_BACKUP_SEMANTICS,
                    NULL);
            }
            else
            {
                hFile = CreateFile(szSubPath,
                    FILE_WRITE_ATTRIBUTES,//����ֻ��һ��д���ļ����Ե�Ȩ��
                    FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,//�����ܲ�Ӱ�������ļ�
                    NULL,
                    OPEN_EXISTING,//���Ѿ����ڵ��ļ�
                    FILE_ATTRIBUTE_NORMAL,
                    NULL);
            }
            if (hFile != INVALID_HANDLE_VALUE)
            {
                SetFileTime(hFile,
                    &fileTime,
                    &fileTime,
                    &fileTime);//����ʱ�䡢����ʱ�䡢�޸�ʱ�䶼һ��
                CloseHandle(hFile);
            }
            else//��֤����©�κ�һ���ļ�����Ŀ¼
            {
                wsprintf(szMsg, _T("�޷����� %s �ļ�"), szSubPath);
                MessageBox(NULL, szMsg, _T("ע��"), MB_ICONWARNING);
            }
        } while (FindNextFile(hListFile, &fnd));

        FindClose(hListFile);
    }
}

int _tmain(int argc, TCHAR* argv[])
{
    A((LPTSTR)_T("./"));//Ҫ������Ŀ¼
    return 0;
}