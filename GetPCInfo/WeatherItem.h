#pragma once

#include <QWidget>
#include "ui_WeatherItem.h"

struct weatherData
{
	QString date;
	QString high;
	QString fx;
	QString low;
	QString fl;
	QString type;
};

class WeatherItem : public QWidget
{
	Q_OBJECT
public:
	WeatherItem(QWidget *parent = Q_NULLPTR);
	~WeatherItem();

	void setWeatherInfo(weatherData info);

private:
	Ui::weatherItem ui;
};
