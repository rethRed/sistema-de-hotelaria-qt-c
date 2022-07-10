#include "registrarconsumo.h"
#include "ui_registrarconsumo.h"

RegistrarConsumo::RegistrarConsumo(QWidget *parent,QString reservaId) :
    QDialog(parent),
    ui(new Ui::RegistrarConsumo)
{
    ui->setupUi(this);
    this->reservaId = reservaId;
}

RegistrarConsumo::~RegistrarConsumo()
{
    delete ui;
}

void RegistrarConsumo::on_pushButton_pesquisar_clicked()
{
    QVariantMap userInput;
    userInput["hotelId"] = "1";

    QJsonDocument userInputJsonFormat;

    userInputJsonFormat = userInputJsonFormat.fromVariant(userInput);

    QNetworkReply *reply = networkRequest.makeRequest("http://127.0.0.1:5000/api/GetConsumos","post",userInputJsonFormat,5000);
    QObject::connect(reply, &QNetworkReply::finished, this,[=](){

        ui->listWidget_consumos->clear();
        QJsonDocument jsonResponse = QJsonDocument::fromJson(QString(reply->readAll()).toUtf8());
        if (jsonResponse.isEmpty()) return;

        for(int i=0;true;i++){
            if(jsonResponse[i].isUndefined()) break;
            QString NomeProdutos = jsonResponse[i]["Descricao"].toString();
            QString ProdutoId = QString::number(jsonResponse[i]["Id"].toInt());
            QString Preco = jsonResponse[i]["Preco"].toString();


            QListWidgetItem *itemProduto = new QListWidgetItem;
            itemProduto->setText(NomeProdutos+" "+Preco);
            itemProduto->setWhatsThis(ProdutoId);
            ui->listWidget_consumos->addItem(itemProduto);


        }
    });

}


void RegistrarConsumo::on_listWidget_consumos_itemDoubleClicked(QListWidgetItem *item)
{
    QString idProduto = item->whatsThis();

    QSettings setting("RethsHotel","RethsHotelSetting");
    setting.beginGroup("UserHotelInfo");
    QString curUserId = setting.value("CurUserId").toString();
    QString CurIdCaixa = setting.value("CurCaixaId").toString();
    setting.endGroup();

    QVariantMap userInput;


    userInput["id_reserva"] = reservaId;
    userInput["Id_usuario"] = curUserId;
    userInput["id_caixa"] = CurIdCaixa;
    userInput["id_produto"] = idProduto;


    QJsonDocument userInputJsonFormat;

    userInputJsonFormat = userInputJsonFormat.fromVariant(userInput);

    QNetworkReply *reply = networkRequest.makeRequest("http://127.0.0.1:5000/api/ResgistrarConsumos","post",userInputJsonFormat,5000);
    QObject::connect(reply, &QNetworkReply::finished, this,[=] (){

    QJsonDocument jsonResponse = QJsonDocument::fromJson(QString(reply->readAll()).toUtf8());
    if (jsonResponse.isEmpty()) return;

    int Registroid = jsonResponse.object().take("IdRegistro").toInt();
    QString Descricao = jsonResponse.object().take("descricao").toString();
    int Qtd = jsonResponse.object().take("Qtd").toInt();
    QString precoUn = jsonResponse.object().take("precoUn").toString();
    QString precoTotal= jsonResponse.object().take("precoTotal").toString();
    QString Status= jsonResponse.object().take("Status").toString();

    QMap <QString,QString> ServerResposta;

    ServerResposta["IdRegistro"] = QString::number(Registroid);
    ServerResposta["descricao"] = Descricao;
    ServerResposta["Qtd"] = QString::number(Qtd);
    ServerResposta["precoUn"] = precoUn;
    ServerResposta["precoTotal"] = precoTotal;
    ServerResposta["Status"] = Status;

    emit(produtoConsumoRegistrado(ServerResposta));
    this->close();

    });



}

