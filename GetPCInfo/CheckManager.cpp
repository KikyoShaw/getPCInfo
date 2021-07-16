#include "CheckManager.h"
#include <QHostInfo>
#include <QSettings>
#include <QDesktopWidget>
#include <QApplication>
#include <QFileInfoList>
#include <QDir>
#include <QNetworkInterface>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QTimer>
#include <QLibrary>

#include <Windows.h>

#define GB (1024 * 1024 * 1024)
#define MB (1024 * 1024)
#define KB (1024)

CheckManager & CheckManager::getInstance()
{
	static CheckManager m_instance;
	return m_instance;
}

CheckManager::CheckManager(QObject *parent)
	: QObject(parent)
{
}

CheckManager::~CheckManager()
{
}

const QString CheckManager::getLocalHostName()
{
	QString _localHostName = QHostInfo::localHostName();
	return _localHostName;
}

const QString CheckManager::getCPUInfo()
{
	QSettings cpuSetting("HKEY_LOCAL_MACHINE\\HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0", QSettings::NativeFormat);
	QString _cpuName = cpuSetting.value("ProcessorNameString").toString();
	return _cpuName;
}

const QString CheckManager::getDisplayCard()
{
	QString _displayCard;
	QSettings displayCards1("HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\services\\nvlddmkm\\Device0", QSettings::NativeFormat);
	QString cardStr1 = displayCards1.value("Device Description").toString();
	cardStr1.trimmed();
	if (!cardStr1.isEmpty())
		_displayCard = cardStr1;

	QSettings displayCards2("HKEY_LOCAL_MACHINE\\SYSTEM\\ControlSet001\\Services\\igfx\\Device0", QSettings::NativeFormat);
	QString cardStr2 = displayCards2.value("Device Description").toString();
	cardStr2.trimmed();
	if (!cardStr2.isEmpty())
		_displayCard = _displayCard + "\n" + cardStr2;

	QSettings displayCards3("HKEY_LOCAL_MACHINE\\SYSTEM\\ControlSet001\\Services\\amdkmdap\\Device0", QSettings::NativeFormat);
	QString cardStr3 = displayCards3.value("Device Description").toString();
	cardStr3.trimmed();
	if (!cardStr3.isEmpty())
		_displayCard = _displayCard + "\n" + cardStr3;

	_displayCard.trimmed();
	return _displayCard;
}

const QString CheckManager::getMemory()
{
	MEMORYSTATUSEX statex;
	statex.dwLength = sizeof(statex);
	GlobalMemoryStatusEx(&statex);
	float totalM = statex.ullTotalPhys  * 1.0 / GB;
	float freeM = statex.ullAvailPhys * 1.0 / GB;
	QString _memoryTxt = QStringLiteral("可用 %1 GB / 共 %2 GB").arg(QString::asprintf("%.2f", freeM)).arg(QString::asprintf("%.2f", totalM));
	return _memoryTxt;
}

const QString CheckManager::getOSInfo()
{
	typedef BOOL(WINAPI *LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);
	LPFN_ISWOW64PROCESS fnIsWow64Process;
	BOOL bIsWow64 = FALSE;
	fnIsWow64Process = (LPFN_ISWOW64PROCESS)GetProcAddress(GetModuleHandle(TEXT("kernel32")), "IsWow64Process");
	if (NULL != fnIsWow64Process){
		fnIsWow64Process(GetCurrentProcess(), &bIsWow64);
	}
	QString sysBit = "unknown";
	if (bIsWow64)
		sysBit = QStringLiteral("64位");
	else
		sysBit = QStringLiteral("32位");
	QString _osTxt = QSysInfo::prettyProductName() + " " + sysBit;
	return _osTxt;
}

const QString CheckManager::getScreenInfo()
{
	QString _screenTxt = QString();
	QList<QSize> screenSizeList;
	QList <int> screenCountList;
	for (int i = 0; i < QApplication::desktop()->screenCount(); i++){
		QRect screenRect = QApplication::desktop()->screenGeometry(i);
		QSize size(screenRect.width(), screenRect.height());
		bool bExist = false;
		for (int j = 0; j < screenSizeList.length(); j++){
			QSize existSize = screenSizeList.at(j);
			if (size == existSize){
				screenCountList[j]++;
				bExist = true;
				break;
			}
		}
		if (!bExist){
			screenSizeList.append(size);
			screenCountList.append(1);
		}
	}
	for (int i = 0; i < screenSizeList.length(); i++)
	{
		int width = screenSizeList.at(i).width();
		int height = screenSizeList.at(i).height();
		_screenTxt += QStringLiteral("(%1像素 x %2像素) x %3个").arg(width).arg(height).arg(screenCountList.at(i));
		if (i != screenSizeList.length() - 1)
			_screenTxt += QStringLiteral("、 ");
	}
	return _screenTxt;
}

const QString CheckManager::getDiskId()
{
	QString _diskId;
	QLibrary diskLib("diskid.dll");
	if (diskLib.load()){
		GetDiskIdFun GetDiskId = (GetDiskIdFun)diskLib.resolve("GetHardDriveSerialNumber");
		if (GetDiskId){
			char diskIdBuff[255];
			int result = GetDiskId(diskIdBuff, 255);
			if (result){
				_diskId = diskIdBuff;
			}
		}
		diskLib.unload();
	}
	return _diskId;
}

const QString CheckManager::getDiskInfo()
{
	QString _diskTxt = QString();
	QFileInfoList list = QDir::drives();
	foreach(QFileInfo dir, list){
		QString dirName = dir.absolutePath();
		dirName.remove("/");
		LPCWSTR lpcwstrDriver = (LPCWSTR)dirName.utf16();
		ULARGE_INTEGER liFreeBytesAvailable, liTotalBytes, liTotalFreeBytes;
		if (GetDiskFreeSpaceEx(lpcwstrDriver, &liFreeBytesAvailable, &liTotalBytes, &liTotalFreeBytes)){
			QString free = QString::number((double)liTotalFreeBytes.QuadPart / GB, 'f', 1);
			free += "G";
			QString all = QString::number((double)liTotalBytes.QuadPart / GB, 'f', 1);
			all += "G";
			QString str = QStringLiteral("%1 %2/%3       ").arg(dirName, free, all);
			_diskTxt += str;
		}
	}
	return _diskTxt;
}

const QString CheckManager::getIpInfo()
{
	QString _ipTxt = QString();
	QList<QNetworkInterface> interFaceList = QNetworkInterface::allInterfaces();
	for (int i = 0; i < interFaceList.size(); i++){
		QNetworkInterface _interface = interFaceList.at(i);
		if (_interface.flags().testFlag(QNetworkInterface::IsRunning)){
			QList<QNetworkAddressEntry> entryList = _interface.addressEntries();
			foreach(QNetworkAddressEntry entry, entryList){
				if (QAbstractSocket::IPv4Protocol == entry.ip().protocol() &&
					entry.ip() != QHostAddress::LocalHost && entry.ip().toString().startsWith("192.168.")){
					_ipTxt = entry.ip().toString();
					break;
				}
			}
		}
	}
	if (_ipTxt.isEmpty()) {
		_ipTxt = QStringLiteral("IP无法访问");
	}
	return _ipTxt;
}

const QString CheckManager::getPublicIpInfo()
{
	QString _publicIp = QString();
	QString webCode = getHtml("http://whois.pconline.com.cn/");
	if (!webCode.isEmpty()){
		QString web = webCode.replace(" ", "");
		web = web.replace("\r", "");
		web = web.replace("\n", "");
		QStringList list = web.split("<br/>");
		QString tar = list[3];
		QStringList ip = tar.split("=");
		_publicIp = ip[1];
	}
	else
		_publicIp = QStringLiteral("无法获取公网ip");
	return _publicIp;
}

const QString CheckManager::getMacAddress()
{
	QString _macTxt = QString();
	QList<QNetworkInterface> netList = QNetworkInterface::allInterfaces();
	foreach(QNetworkInterface item, netList){
		if ((QNetworkInterface::IsUp & item.flags()) && (QNetworkInterface::IsRunning & item.flags())){
			if (_macTxt.isEmpty() || _macTxt < item.hardwareAddress()){
				_macTxt = item.hardwareAddress();
			}
		}
	}
	return _macTxt;
}

const QString CheckManager::getOffice()
{
	QSettings settings("HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths\\excel.exe", QSettings::NativeFormat);
	QString path = settings.value("Path").toString();
	if (!path.isEmpty()){
		QSettings versionSettings("HKEY_LOCAL_MACHINE\\SOFTWARE\\Classes\\Excel.Application\\CurVer", QSettings::NativeFormat);
		QString version = versionSettings.value(".").toString();
		if (!version.isEmpty())
		{
			version = version.split(".").last();
			int versionNum = version.toInt();
		}
	}
	return QString();
}

bool CheckManager::ipLive()
{
	//能接通百度IP说明可以通外网
	QTcpSocket tcpClient;
	tcpClient.abort();
	tcpClient.connectToHost("202.108.22.5", 80);
	//100毫秒没有连接上则判断不在线
	return tcpClient.waitForConnected(100);
}

QString CheckManager::getHtml(QString url)
{
	QNetworkAccessManager *manager = new QNetworkAccessManager();
	QNetworkReply *reply = manager->get(QNetworkRequest(QUrl(url)));
	QByteArray responseData;
	QEventLoop eventLoop;
	QTimer timer;
	timer.setSingleShot(true);
	connect(manager, SIGNAL(finished(QNetworkReply *)), &eventLoop, SLOT(quit()));
	connect(&timer, SIGNAL(timeout()), &eventLoop, SLOT(quit()));
	timer.start(300);
	eventLoop.exec();
	responseData = reply->readAll();
	return QString(responseData);
}
