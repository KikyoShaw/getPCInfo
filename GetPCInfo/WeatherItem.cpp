#include "WeatherItem.h"
#include <QDateTime>

WeatherItem::WeatherItem(QWidget *parent)
	:QWidget(parent)
{
	ui.setupUi(this);
}

WeatherItem::~WeatherItem()
{
}

void WeatherItem::setWeatherInfo(weatherData info)
{
	//日期,需要组装
	QString date = QDateTime::currentDateTime().toString("yyyy-MM");
	date = date + "-" + info.date;
	ui.label_date->setText(date);

	//天气
	ui.label_weather->setText(QStringLiteral("天气：%1").arg(info.type));

	//温度，需要拼接
	QString temp = info.low.mid(info.low.length() - 3, 4) + "~" + info.high.mid(info.high.length() - 3, 4);;
	ui.label_TEMP->setText(QStringLiteral("温度：%1").arg(temp));

	//风，需要拼接
	QString wind = info.fl.remove(0, 8);
	wind = wind.remove(wind.length() - 2, 2);
	wind = info.fx + wind;
	ui.label_wind->setText(QStringLiteral("风：%1").arg(wind));
}
