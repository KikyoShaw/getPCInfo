#pragma once

#include <QWidget>
#include "ui_CPUInfoWidget.h"

class CpuInfo : public QWidget
{
	Q_OBJECT
public:
	CpuInfo(QWidget *parent = Q_NULLPTR);
	~CpuInfo();

	void initCPUInfo();

private:
	Ui::CPUInfo ui;
};
