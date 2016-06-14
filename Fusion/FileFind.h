#ifndef _FILE_FIND_H_
#define _FILE_FIND_H_

#include<iostream>
#include<windows.h>
#include<vector>
using namespace std;

BOOL IsRoot(LPCTSTR lpszPath)
{
    TCHAR szRoot[4];
    wsprintf(szRoot, "%c:\\", lpszPath[0]);
    return (lstrcmp(szRoot, lpszPath) == 0);
}
 
void FindInAll(::LPCTSTR lpszPath,vector<string>& fpath,int& nLen)
{
    TCHAR szFind[MAX_PATH];
	char* fname;
	string spath;
	nLen=0;
    lstrcpy(szFind, lpszPath);
    if (!IsRoot(szFind))
    lstrcat(szFind, "\\");
    lstrcat(szFind, "*.*"); // �������ļ�
    WIN32_FIND_DATA wfd;
    HANDLE hFind = FindFirstFile(szFind, &wfd);
    if (hFind == INVALID_HANDLE_VALUE) // ���û���ҵ������ʧ��
    return;
 
    do
    {
        if (wfd.cFileName[0] == '.')
            continue; // ����������Ŀ¼
        if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
            TCHAR szFile[MAX_PATH];
            if (IsRoot(lpszPath))
                wsprintf(szFile, "%s%s", lpszPath, wfd.cFileName);
            else
                wsprintf(szFile, "%s\\%s", lpszPath, wfd.cFileName);
            FindInAll(szFile,fpath,nLen); // ����ҵ�����Ŀ¼��������Ŀ¼���еݹ�
        }
        else
        {
            TCHAR szFile[MAX_PATH];
            if (IsRoot(lpszPath))
                wsprintf(szFile, "%s%s", lpszPath, wfd.cFileName);
            else
                wsprintf(szFile, "%s\\%s", lpszPath, wfd.cFileName);
			//fname= ConvertLPWSTRToLPSTR((LPWSTR)szFile);
			fname=szFile;
			//fname=ConvertLPWSTRToLPSTR((LPWSTR)szFile);
			nLen=nLen+1;
			spath=fname;
            //printf("%s\n",fname);
			fpath.push_back(spath);
            // ���ļ����в���
        }
    } while (FindNextFile(hFind, &wfd));
    FindClose(hFind); // �رղ��Ҿ��
}

void FindSubDir(::LPCTSTR lpszPath,vector<string>& fdir,vector<string>& Fname,int& nLen)
{
    TCHAR szFind[MAX_PATH];
	TCHAR szName[MAX_PATH];
	char* fname;
	string spath;
	nLen=0;
    lstrcpy(szFind, lpszPath);
    if (!IsRoot(szFind))
    lstrcat(szFind, "\\");
    lstrcat(szFind, "*.*"); // �������ļ�
    WIN32_FIND_DATA wfd;
    HANDLE hFind = FindFirstFile(szFind, &wfd);
    if (hFind == INVALID_HANDLE_VALUE) // ���û���ҵ������ʧ��
    return;
 
    do
    {
        if (wfd.cFileName[0] == '.')
            continue; // ����������Ŀ¼
        if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
            TCHAR szFile[MAX_PATH];
            if (IsRoot(lpszPath))
			{
                wsprintf(szFile, "%s%s", lpszPath, wfd.cFileName);
				wsprintf(szName, "%s",  wfd.cFileName);
			}
            else
			{
                wsprintf(szFile, "%s\\%s", lpszPath, wfd.cFileName);
				wsprintf(szName, "%s", wfd.cFileName);
			}
            //FindInAll(szFile,fpath,nLen); // ����ҵ�����Ŀ¼��������Ŀ¼���еݹ�
			fname=szFile;
			nLen=nLen+1;
			spath=fname;
			fdir.push_back(spath);

			fname=szName;
			spath=fname;
			Fname.push_back(spath);
			//cout<<szFile<<endl;
        }
    } while (FindNextFile(hFind, &wfd));
    FindClose(hFind); // �رղ��Ҿ��
}


#endif