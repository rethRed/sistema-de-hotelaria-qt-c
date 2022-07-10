#ifndef REQUESTS_H
#define REQUESTS_H
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include<QAuthenticator>
#include <QNetworkProxy>
#include <QObject>
#include <QDebug>
#include <QJsonValue>
#include <QJsonDocument>
#include <QJsonObject>
#include <QVariantMap>
#include <QJsonArray>
#include <QMessageBox>
#include <QSettings>
#include <QObject>

class Requests : public QObject
{
    Q_OBJECT
public:
    explicit Requests(QObject *parent = nullptr);
    QNetworkReply* makeRequest(QString url,QString method = "get",QJsonDocument data = {},int timeout = 0);

signals:

private:
    QNetworkAccessManager *network = new QNetworkAccessManager(this);

};

#endif // REQUESTS_H
