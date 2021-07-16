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

	//滑动条美化
	QFile QSS(":/qss/images/whiteScrollbar.qss");
	if (QSS.open(QIODevice::ReadOnly)) {
		QString strStyle = QSS.readAll();
		ui.scrollArea->verticalScrollBar()->setStyleSheet(strStyle);
	}

	//网络组件初始化
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
	//初始化UI
	if (parentWidget()) {
		setFixedSize(parentWidget()->size());
	}

	sltRequestWeatherInfo();
}

void WeatherInfo::sltGetWeather(QNetworkReply * reply)
{
	//数据初始化
	m_widgetsHandler.clear();
	QTextCodec *codec = QTextCodec::codecForName("utf8");
	QString all = codec->toUnicode(reply->readAll());

	//解析json对象
	QJsonParseError err;
	QJsonDocument jsonData = QJsonDocument::fromJson(all.toUtf8(), &err);
	if (jsonData.isNull()) {
		qInfo() << err.error;
		QMessageBox::information(NULL,QStringLiteral("提示"), QStringLiteral("未找到该城市的天气数据,确认城市是否存在?"));
		return;
	}
	QJsonObject object = jsonData.object();
	//返回的json数据带有data
	if (!object.contains("data")) {
		qInfo() << err.error;
		QMessageBox::information(NULL, QStringLiteral("提示"), QStringLiteral("未找到该城市的天气数据,确认城市是否存在?"));
		return;
	}
	//根据key值获取对应的 value
	QJsonValue value_data = object.value("data");
	if (value_data.isObject()) {
		QJsonObject object_data = value_data.toObject();
		//获取昨天的数据
		if (object_data.contains("yesterday")) {
			QJsonValue value_yesterday = object_data.value("yesterday");
			if (value_yesterday.isObject()) {
				QJsonObject weather_yesterday = value_yesterday.toObject();
				addWeathInfoItem(weather_yesterday);
			}
		}
		//获取今天开始后续几天数据
		if (object_data.contains("forecast")) {
			QJsonValue value_forecast = object_data.value("forecast");
			//遍历数组
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
	//设置请求数据
	request.setUrl(QUrl(QStringLiteral("http://wthrcdn.etouch.cn/weather_mini?city=%1").arg(_city)));
	request.setHeader(QNetworkRequest::UserAgentHeader, "RT-Thread ART");
	//发送get网络请求
	m_networkGetWeather->get(request);
}