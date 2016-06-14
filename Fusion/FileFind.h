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
    lstrcat(szFind, "*.*"); // 找所有文件
    WIN32_FIND_DATA wfd;
    HANDLE hFind = FindFirstFile(szFind, &wfd);
    if (hFind == INVALID_HANDLE_VALUE) // 如果没有找到或查找失败
    return;
 
    do
    {
        if (wfd.cFileName[0] == '.')
            continue; // 过滤这两个目录
        if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
            TCHAR szFile[MAX_PATH];
            if (IsRoot(lpszPath))
                wsprintf(szFile, "%s%s", lpszPath, wfd.cFileName);
            else
                wsprintf(szFile, "%s\\%s", lpszPath, wfd.cFileName);
            FindInAll(szFile,fpath,nLen); // 如果找到的是目录，则进入此目录进行递归
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
            // 对文件进行操作
        }
    } while (FindNextFile(hFind, &wfd));
    FindClose(hFind); // 关闭查找句柄
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
    lstrcat(szFind, "*.*"); // 找所有文件
    WIN32_FIND_DATA wfd;
    HANDLE hFind = FindFirstFile(szFind, &wfd);
    if (hFind == INVALID_HANDLE_VALUE) // 如果没有找到或查找失败
    return;
 
    do
    {
        if (wfd.cFileName[0] == '.')
            continue; // 过滤这两个目录
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
            //FindInAll(szFile,fpath,nLen); // 如果找到的是目录，则进入此目录进行递归
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
    FindClose(hFind); // 关闭查找句柄
}


#endif