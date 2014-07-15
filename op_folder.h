#include <vector>
#include <string>
using namespace std;
class CBrowseDir
{
protected:
	//��ų�ʼĿ¼�ľ���·������'\'��β
	char m_szInitDir[_MAX_PATH];
	vector<string> vfile;
public:
	//ȱʡ������
	CBrowseDir();
	
	//���ó�ʼĿ¼Ϊdir���������false����ʾĿ¼������
	bool SetInitDir(const char *dir);
	bool SetInitDir(const string dir){ return SetInitDir(dir.c_str()); }
	
	//��ʼ������ʼĿ¼������Ŀ¼����filespecָ�����͵��ļ�
	//filespec����ʹ��ͨ��� * ?�����ܰ���·����
	//�������false����ʾ�������̱��û���ֹ
	bool BeginBrowse(const char *filespec);
	bool BeginBrowse(const string filespec){ return BeginBrowse(filespec.c_str()); }

	vector<string> GetVfile(){ return vfile;}
	
protected:
	//����Ŀ¼dir����filespecָ�����ļ�
	//������Ŀ¼,���õ����ķ���
	//�������false,��ʾ��ֹ�����ļ�
	bool BrowseDir(const char *dir,const char *filespec);
	
	//����BrowseDirÿ�ҵ�һ���ļ�,�͵���ProcessFile
	//�����ļ�����Ϊ�������ݹ�ȥ
	//�������false,��ʾ��ֹ�����ļ�
	//�û����Ը�д�ú���,�����Լ��Ĵ������
	virtual bool ProcessFile(const char *filename);
	
	//����BrowseDirÿ����һ��Ŀ¼,�͵���ProcessDir
	//�������ڴ����Ŀ¼������һ��Ŀ¼����Ϊ�������ݹ�ȥ
	//������ڴ�����ǳ�ʼĿ¼,��parentdir=NULL
	//�û����Ը�д�ú���,�����Լ��Ĵ������
	//�����û�����������ͳ����Ŀ¼�ĸ���
	virtual void ProcessDir(const char *currentdir,const char *parentdir);
};
//��CBrowseDir�����������࣬����ͳ��Ŀ¼�е��ļ�����Ŀ¼����
class CStatDir:public CBrowseDir
{
protected:
	int m_nFileCount;   //�����ļ�����
	int m_nSubdirCount; //������Ŀ¼����
	
public:
	//ȱʡ������
	CStatDir()
	{
		//��ʼ�����ݳ�Աm_nFileCount��m_nSubdirCount
		m_nFileCount=m_nSubdirCount=0;
	}
	
	//�����ļ�����
	int GetFileCount()
	{
		return m_nFileCount;
	}
	
	//������Ŀ¼����
	int GetSubdirCount()
	{
		//��Ϊ�����ʼĿ¼ʱ��Ҳ����ú���ProcessDir��
		//���Լ�1�������������Ŀ¼������
		return m_nSubdirCount-1;
	}
	
protected:
	//��д�麯��ProcessFile��ÿ����һ�Σ��ļ�������1
	virtual bool ProcessFile(const char *filename)
	{
		m_nFileCount++;
		cout << filename << endl;
		return CBrowseDir::ProcessFile(filename);
	}
	
	//��д�麯��ProcessDir��ÿ����һ�Σ���Ŀ¼������1
	virtual void ProcessDir
		(const char *currentdir,const char *parentdir)
	{
		m_nSubdirCount++;
		CBrowseDir::ProcessDir(currentdir,parentdir);
	}
};
void op_folder_test();