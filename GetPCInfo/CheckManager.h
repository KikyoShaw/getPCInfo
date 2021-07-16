#pragma once

#include <QObject>

class CheckManager : public QObject
{
	Q_OBJECT
	typedef int(*GetDiskIdFun)(char*, int);

public:
	static CheckManager& getInstance();
	~CheckManager();

public:
	//��ȡ������
	const QString getLocalHostName();
	//��ȡCPU��
	const QString getCPUInfo();
	//��ȡ�Կ�
	const QString getDisplayCard();
	//��ȡ�ڴ�
	const QString getMemory();
	//��ȡ������Ϣ
	const QString getOSInfo();
	//��ȡ��ʾ���ֱ���
	const QString getScreenInfo();
	//��ȡӲ����Ϣ
	const QString getDiskInfo();
	//��ȡӲ�̺�
	const QString getDiskId();
	//��ȡip��Ϣ
	const QString getIpInfo();
	//��ȡ����ip
	const QString getPublicIpInfo();
	//��ȡMAC��ַ
	const QString getMacAddress();
	//��ȡoffice�汾
	const QString getOffice();
	//�ж��Ƿ�����
	bool ipLive();


private:
	QString getHtml(QString url);

private:
	CheckManager(QObject *parent = Q_NULLPTR);
	Q_DISABLE_COPY(CheckManager)

};

#define checkManager CheckManager::getInstance()
