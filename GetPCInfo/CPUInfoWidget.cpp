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
	//��ʼ��UI
	if (parentWidget()) {
		setFixedSize(parentWidget()->size());
	}

	SYSTEM_INFO sysInfo;
	GetSystemInfo(&sysInfo);

	ui.label_pageSize->setText(QStringLiteral("��ҳ��С��%1").arg(sysInfo.dwPageSize));
	ui.label_mask->setText(QStringLiteral("���룺%1").arg(sysInfo.dwActiveProcessorMask));
	ui.label_processorNum->setText(QStringLiteral("������������%1").arg(sysInfo.dwNumberOfProcessors));
	ui.label_processorType->setText(QStringLiteral("���ͣ�%1").arg(sysInfo.dwProcessorType));
	ui.label_processorLevel->setText(QStringLiteral("�ȼ���%1").arg(sysInfo.wProcessorLevel));
	ui.label_processorVersion->setText(QStringLiteral("�汾��%1").arg(sysInfo.wProcessorRevision));
	char buff[32];
	sprintf(buff, "%p", sysInfo.lpMinimumApplicationAddress);
	ui.label_minAddress->setText(QStringLiteral("��СѰַ��%1").arg(buff));
	sprintf(buff, "%p", sysInfo.lpMaximumApplicationAddress);
	ui.label_maxAddress->setText(QStringLiteral("���Ѱַ��%1").arg(buff));
}
