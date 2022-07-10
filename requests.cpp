#include "requests.h"

Requests::Requests(QObject *parent)
    : QObject{parent}
{

}

QNetworkReply* Requests::makeRequest(QString url,QString method,QJsonDocument data,int timeout)
{
    QNetworkReply *reply = nullptr;

    if (method == "get"){
        network->setTransferTimeout(timeout);
        reply = network->get(QNetworkRequest(QUrl(url)));
        return reply;

    }
    else if (method == "post"){
        network->setTransferTimeout(timeout);
        QNetworkRequest whereToRequest((QUrl(url)));
        whereToRequest.setHeader(QNetworkRequest::ContentTypeHeader,QString("application/json"));
        reply = network->post( whereToRequest,data.toJson());
        return reply;
    }
    else return reply;

}



