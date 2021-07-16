#pragma once

#include <QtWidgets/QWidget>
#include <QSharedPointer>
#include "ui_GetPCInfo.h"

class QTimer;
class CpuInfo;
class WeatherInfo;

class GetPCInfo : public QWidget
{
    Q_OBJECT

public:
    GetPCInfo(QWidget *parent = Q_NULLPTR);
	~GetPCInfo();

private:
	void initPCInfo();

private slots:
	void sltTimerLcdNumber();
	void sltGetCPUInfo();
	void sltWeatherInfo();

private:
	virtual void mouseMoveEvent(QMouseEvent *event);
	virtual void mousePressEvent(QMouseEvent *event);
	virtual void mouseReleaseEvent(QMouseEvent *event);

private:
    Ui::GetPCInfoClass ui;
	//窗口移动属性值
	QPoint m_point;
	volatile bool m_bMove = false;
	//定时器刷新时间
	QTimer *timer = nullptr;
	//CPU详细信息
	QSharedPointer<CpuInfo> m_cpuInfo = nullptr;
	//天气查询
	QSharedPointer<WeatherInfo> m_weatherInfo = nullptr;
};
