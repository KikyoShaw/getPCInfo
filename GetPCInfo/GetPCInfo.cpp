#include "GetPCInfo.h"
#include <QTimer>
#include <QDateTime>
#include <QMouseEvent>
#include "CheckManager.h"
#include "CPUInfoWidget.h"
#include "WeatherInfoWidget.h"

#define DATETIME qPrintable (QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss"))

GetPCInfo::GetPCInfo(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);

	setWindowFlags(Qt::FramelessWindowHint);
	setAttribute(Qt::WA_TranslucentBackground);

	//时间组件
	ui.lcdNumber->display(DATETIME);
	//定时器控制时间刷新
	timer = new QTimer(this);
	timer->setTimerType(Qt::PreciseTimer);
	connect(timer, &QTimer::timeout, this, &GetPCInfo::sltTimerLcdNumber);
	timer->start(1000);

	//内存CPU控件工作
	ui.label_cpu->start(1000);

	//电脑配置初始化
	initPCInfo();

	connect(ui.pushButton_close, &QPushButton::clicked, this, &QWidget::close);
	connect(ui.pushButton_check, &QPushButton::clicked, this, &GetPCInfo::sltGetCPUInfo);
	connect(ui.pushButton_weather, &QPushButton::clicked, this, &GetPCInfo::sltWeatherInfo);
}

GetPCInfo::~GetPCInfo()
{
}

void GetPCInfo::initPCInfo()
{
	//电脑名称
	auto name = checkManager.getLocalHostName();
	ui.label_name->setText(name);
	//CPU
	auto cpuName = checkManager.getCPUInfo();
	ui.label_cpuName->setText(cpuName);
	//显卡
	auto card = checkManager.getDisplayCard();
	ui.label_card->setText(card);
	//内存
	auto memory = checkManager.getMemory();
	ui.label_mem->setText(memory);
	//操作系统
	auto os = checkManager.getOSInfo();
	ui.label_os->setText(os);
	//显示器屏幕分辩率
	auto screen = checkManager.getScreenInfo();
	ui.label_screen->setText(screen);
	//磁盘空间
	auto disk = checkManager.getDiskInfo();
	ui.label_disk->setText(disk);
	//硬盘号
	//auto diskId = checkManager.getDiskId();
	//mac地址
	auto mac = checkManager.getMacAddress();
	ui.label_mac->setText(mac);
	//office版本
	//auto office = checkManager.getOffice();
}

void GetPCInfo::sltTimerLcdNumber()
{
	ui.lcdNumber->display(DATETIME);
}

void GetPCInfo::sltGetCPUInfo()
{
	m_cpuInfo = QSharedPointer<CpuInfo>(new CpuInfo(this));
	if (m_cpuInfo) {
		m_cpuInfo->initCPUInfo();
		m_cpuInfo->move(rect().center() - m_cpuInfo->rect().center());
		m_cpuInfo->show();
	}
}

void GetPCInfo::sltWeatherInfo()
{
	m_weatherInfo = QSharedPointer<WeatherInfo>(new WeatherInfo(this));
	if (m_weatherInfo) {
		m_weatherInfo->initWeatherInfo();
		m_weatherInfo->move(rect().center() - m_weatherInfo->rect().center());
		m_weatherInfo->show();
	}
}

void GetPCInfo::mouseMoveEvent(QMouseEvent * event)
{
	//判断左键是否被按下，只有左键按下了，其返回值就是1(true)
	if ((event->buttons() & Qt::LeftButton) && m_bMove)
	{
		move(event->globalPos() - m_point);
	}
	QWidget::mouseMoveEvent(event);
}

void GetPCInfo::mousePressEvent(QMouseEvent * event)
{
	if (event->button() == Qt::LeftButton)
	{
		m_bMove = true;
		m_point = event->globalPos() - frameGeometry().topLeft();
	}
	QWidget::mousePressEvent(event);
}

void GetPCInfo::mouseReleaseEvent(QMouseEvent * event)
{
	m_bMove = false;
	QWidget::mouseReleaseEvent(event);
}

