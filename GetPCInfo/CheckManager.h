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
	//获取主机名
	const QString getLocalHostName();
	//获取CPU名
	const QString getCPUInfo();
	//获取显卡
	const QString getDisplayCard();
	//获取内存
	const QString getMemory();
	//获取操作信息
	const QString getOSInfo();
	//获取显示器分辨率
	const QString getScreenInfo();
	//获取硬盘信息
	const QString getDiskInfo();
	//获取硬盘号
	const QString getDiskId();
	//获取ip信息
	const QString getIpInfo();
	//获取公网ip
	const QString getPublicIpInfo();
	//获取MAC地址
	const QString getMacAddress();
	//获取office版本
	const QString getOffice();
	//判断是否在线
	bool ipLive();


private:
	QString getHtml(QString url);

private:
	CheckManager(QObject *parent = Q_NULLPTR);
	Q_DISABLE_COPY(CheckManager)

};

#define checkManager CheckManager::getInstance()
