#include "WeatherInfoWidget.h"
#include "WeatherItem.h"
#include <QTextCodec>
#include <QJsonArray>
#include <QJsonDocument>
#include <QNetworkAccessManager>
#include <QMessageBox>
#include <QFile>
#include <QScrollBar>

WeatherInfo::WeatherInfo(QWidget *parent)
	:QWidget(parent)
{
	ui.setupUi(this);

	setWindowFlags(Qt::FramelessWindowHint);
	setAttribute(Qt::WA_TranslucentBackground);

	//����������
	QFile QSS(":/qss/images/whiteScrollbar.qss");
	if (QSS.open(QIODevice::ReadOnly)) {
		QString strStyle = QSS.readAll();
		ui.scrollArea->verticalScrollBar()->setStyleSheet(strStyle);
	}

	//���������ʼ��
	m_networkGetWeather = new QNetworkAccessManager(this);
	connect(m_networkGetWeather, SIGNAL(finished(QNetworkReply *)), this, SLOT(sltGetWeather(QNetworkReply*)));

	connect(ui.pushButton, &QPushButton::clicked, this, &QWidget::close);
	connect(ui.pushButton_find, &QPushButton::clicked, this, &WeatherInfo::sltRequestWeatherInfo);
}

WeatherInfo::~WeatherInfo()
{
}

void WeatherInfo::initWeatherInfo()
{
	//��ʼ��UI
	if (parentWidget()) {
		setFixedSize(parentWidget()->size());
	}

	sltRequestWeatherInfo();
}

void WeatherInfo::sltGetWeather(QNetworkReply * reply)
{
	//���ݳ�ʼ��
	m_widgetsHandler.clear();
	QTextCodec *codec = QTextCodec::codecForName("utf8");
	QString all = codec->toUnicode(reply->readAll());

	//����json����
	QJsonParseError err;
	QJsonDocument jsonData = QJsonDocument::fromJson(all.toUtf8(), &err);
	if (jsonData.isNull()) {
		qInfo() << err.error;
		QMessageBox::information(NULL,QStringLiteral("��ʾ"), QStringLiteral("δ�ҵ��ó��е���������,ȷ�ϳ����Ƿ����?"));
		return;
	}
	QJsonObject object = jsonData.object();
	//���ص�json���ݴ���data
	if (!object.contains("data")) {
		qInfo() << err.error;
		QMessageBox::information(NULL, QStringLiteral("��ʾ"), QStringLiteral("δ�ҵ��ó��е���������,ȷ�ϳ����Ƿ����?"));
		return;
	}
	//����keyֵ��ȡ��Ӧ�� value
	QJsonValue value_data = object.value("data");
	if (value_data.isObject()) {
		QJsonObject object_data = value_data.toObject();
		//��ȡ���������
		if (object_data.contains("yesterday")) {
			QJsonValue value_yesterday = object_data.value("yesterday");
			if (value_yesterday.isObject()) {
				QJsonObject weather_yesterday = value_yesterday.toObject();
				addWeathInfoItem(weather_yesterday);
			}
		}
		//��ȡ���쿪ʼ������������
		if (object_data.contains("forecast")) {
			QJsonValue value_forecast = object_data.value("forecast");
			//��������
			if (value_forecast.isArray()) {
				for (auto & each : value_forecast.toArray()) {
					QJsonObject weather_forecast = each.toObject();
					addWeathInfoItem(weather_forecast);
				}
			}
		}
	}
}

void WeatherInfo::addWeathInfoItem(QJsonObject object)
{
	weatherData data;
	data.date = object.value("date").toString();
	data.high = object.value("high").toString();
	if (object.contains("fx")) {
		data.fx = object.value("fx").toString();
	}
	else if (object.contains("fengxiang")) {
		data.fx = object.value("fengxiang").toString();
	}
	data.low = object.value("low").toString();
	if (object.contains("fl")) {
		data.fl = object.value("fl").toString();
	}
	else if (object.contains("fengli")) {
		data.fl = object.value("fengli").toString();
	}
	data.type = object.value("type").toString();
	WeatherItem *item = new WeatherItem();
	if (item) {
		item->setWeatherInfo(data);
		m_widgetsHandler.add(item);
		ui.verticalLayout_item->addWidget(item);
	}
}

void WeatherInfo::sltRequestWeatherInfo()
{
	QString _city = ui.lineEdit->text();
	QNetworkRequest request;
	//������������
	request.setUrl(QUrl(QStringLiteral("http://wthrcdn.etouch.cn/weather_mini?city=%1").arg(_city)));
	request.setHeader(QNetworkRequest::UserAgentHeader, "RT-Thread ART");
	//����get��������
	m_networkGetWeather->get(request);
}