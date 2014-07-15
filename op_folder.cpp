#include "stdafx.h"
#include "stdlib.h"
#include "direct.h"
#include "string.h"
#include "io.h"
#include "stdio.h" 
#include <iostream>
#include "op_folder.h"
using namespace std;

CBrowseDir::CBrowseDir()
{
	//�õ�ǰĿ¼��ʼ��m_szInitDir
	_getcwd(m_szInitDir,_MAX_PATH);
	
	//���Ŀ¼�����һ����ĸ����'\',����������һ��'\'
	int len=strlen(m_szInitDir);
	if (m_szInitDir[len-1] != '\\')
		strcat_s(m_szInitDir,"\\");
}

bool CBrowseDir::SetInitDir(const char *dir)
{
	//�Ȱ�dirת��Ϊ����·��
	if (_fullpath(m_szInitDir,dir,_MAX_PATH) == NULL)
		return false;
	
	//�ж�Ŀ¼�Ƿ����
	if (_chdir(m_szInitDir) != 0)
		return false;
	
	//���Ŀ¼�����һ����ĸ����'\',����������һ��'\'
	int len=strlen(m_szInitDir);
	if (m_szInitDir[len-1] != '\\')
		strcat_s(m_szInitDir,"\\");
	
	return true;
}

bool CBrowseDir::BeginBrowse(const char *filespec)
{
	vfile.clear();
	ProcessDir(m_szInitDir,NULL);
	return BrowseDir(m_szInitDir,filespec);
}

bool CBrowseDir::BrowseDir(const char *dir,const char *filespec)
{
	_chdir(dir);
	
	//���Ȳ���dir�з���Ҫ����ļ�
	long hFile;
	_finddata_t fileinfo;
	if ((hFile=_findfirst(filespec,&fileinfo)) != -1)
	{
		do
		{
			//����ǲ���Ŀ¼
			//�������,����д���
			if (!(fileinfo.attrib & _A_SUBDIR))
			{
				char filename[_MAX_PATH];
				strcpy_s(filename,dir);
				strcat_s(filename,fileinfo.name);
				// cout << filename << endl;
				if (!ProcessFile(filename))
					return false;
			}
		} while (_findnext(hFile,&fileinfo) == 0);
		_findclose(hFile);
	}
	//����dir�е���Ŀ¼
	//��Ϊ�ڴ���dir�е��ļ�ʱ���������ProcessFile�п��ܸı���
	//��ǰĿ¼����˻�Ҫ�������õ�ǰĿ¼Ϊdir��
	//ִ�й�_findfirst�󣬿���ϵͳ��¼���������Ϣ����˸ı�Ŀ¼
	//��_findnextû��Ӱ�졣
	_chdir(dir);
	if ((hFile=_findfirst("*.*",&fileinfo)) != -1)
	{
		do
		{
			//����ǲ���Ŀ¼
			//�����,�ټ���ǲ��� . �� .. 
			//�������,���е���
			if ((fileinfo.attrib & _A_SUBDIR))
			{
				if (strcmp(fileinfo.name,".") != 0 && strcmp
					(fileinfo.name,"..") != 0)
				{
					char subdir[_MAX_PATH];
					strcpy_s(subdir,dir);
					strcat_s(subdir,fileinfo.name);
					strcat_s(subdir,"\\");
					ProcessDir(subdir,dir);
					if (!BrowseDir(subdir,filespec))
						return false;
				}
			}
		} while (_findnext(hFile,&fileinfo) == 0);
		_findclose(hFile);
	}
	return true;
}

bool CBrowseDir::ProcessFile(const char *filename)
{
	vfile.push_back(filename);
	return true;
}

void CBrowseDir::ProcessDir(const char 
							*currentdir,const char *parentdir)
{
}



void op_folder_test()
{
	//��ȡĿ¼��
	char buf[256];
	printf("������Ҫͳ�Ƶ�Ŀ¼��:");
	gets_s(buf);
	
	//���������
	CStatDir statdir;
	
	//����Ҫ������Ŀ¼
	if (!statdir.SetInitDir(buf))
	{
		puts("Ŀ¼�����ڡ�");
		return;
	}
	
	//��ʼ����
	statdir.BeginBrowse("*.h");
	printf("�ļ�����: %d\n��Ŀ¼����:%d\n",statdir.GetFileCount(),statdir.GetSubdirCount());
} 
