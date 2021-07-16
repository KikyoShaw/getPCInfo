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
	//����,��Ҫ��װ
	QString date = QDateTime::currentDateTime().toString("yyyy-MM");
	date = date + "-" + info.date;
	ui.label_date->setText(date);

	//����
	ui.label_weather->setText(QStringLiteral("������%1").arg(info.type));

	//�¶ȣ���Ҫƴ��
	QString temp = info.low.mid(info.low.length() - 3, 4) + "~" + info.high.mid(info.high.length() - 3, 4);;
	ui.label_TEMP->setText(QStringLiteral("�¶ȣ�%1").arg(temp));

	//�磬��Ҫƴ��
	QString wind = info.fl.remove(0, 8);
	wind = wind.remove(wind.length() - 2, 2);
	wind = info.fx + wind;
	ui.label_wind->setText(QStringLiteral("�磺%1").arg(wind));
}
