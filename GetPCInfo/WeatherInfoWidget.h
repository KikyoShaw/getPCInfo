#pragma once

#include <QWidget>
#include <QNetworkReply>
#include <QObjectCleanupHandler>
#include <QJsonObject>
#include "ui_WeatherInfoWidget.h"

class QNetworkAccessManager;

class WeatherInfo : public QWidget
{
	Q_OBJECT
public:
	WeatherInfo(QWidget *parent = Q_NULLPTR);
	~WeatherInfo();

	void initWeatherInfo();

private slots:
	void sltRequestWeatherInfo();
	void sltGetWeather(QNetworkReply *reply);

private:
	void addWeathInfoItem(QJsonObject object);

private:
	Ui::weatherInfo ui;
	//�������
	QNetworkAccessManager *m_networkGetWeather = nullptr;
	//���ڹ���
	QObjectCleanupHandler m_widgetsHandler;
};
