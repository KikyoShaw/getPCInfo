#include "CPUInfoWidget.h"

#include <Windows.h>

CpuInfo::CpuInfo(QWidget *parent)
	:QWidget(parent)
{
	ui.setupUi(this);

	setWindowFlags(Qt::FramelessWindowHint);
	setAttribute(Qt::WA_TranslucentBackground);

	connect(ui.pushButton, &QPushButton::clicked, this, &QWidget::close);
}

CpuInfo::~CpuInfo()
{
}

void CpuInfo::initCPUInfo()
{
	//初始化UI
	if (parentWidget()) {
		setFixedSize(parentWidget()->size());
	}

	SYSTEM_INFO sysInfo;
	GetSystemInfo(&sysInfo);

	ui.label_pageSize->setText(QStringLiteral("分页大小：%1").arg(sysInfo.dwPageSize));
	ui.label_mask->setText(QStringLiteral("掩码：%1").arg(sysInfo.dwActiveProcessorMask));
	ui.label_processorNum->setText(QStringLiteral("处理器个数：%1").arg(sysInfo.dwNumberOfProcessors));
	ui.label_processorType->setText(QStringLiteral("类型：%1").arg(sysInfo.dwProcessorType));
	ui.label_processorLevel->setText(QStringLiteral("等级：%1").arg(sysInfo.wProcessorLevel));
	ui.label_processorVersion->setText(QStringLiteral("版本：%1").arg(sysInfo.wProcessorRevision));
	char buff[32];
	sprintf(buff, "%p", sysInfo.lpMinimumApplicationAddress);
	ui.label_minAddress->setText(QStringLiteral("最小寻址：%1").arg(buff));
	sprintf(buff, "%p", sysInfo.lpMaximumApplicationAddress);
	ui.label_maxAddress->setText(QStringLiteral("最大寻址：%1").arg(buff));
}
