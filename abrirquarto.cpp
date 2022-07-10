#include "abrirquarto.h"
#include "ui_abrirquarto.h"

AbrirQuarto::AbrirQuarto(QWidget *parent, int QuartoId) :
    QDialog(parent),
    ui(new Ui::AbrirQuarto)
{
    ui->setupUi(this);
    this->QuartoId = QuartoId;

}

AbrirQuarto::~AbrirQuarto()
{
    delete ui;
}

void AbrirQuarto::deleteAllLayoutItens(QLayout *layout)
{
        QLayoutItem *child;
        while ((child = layout->takeAt(0)) != 0) {
          delete child;
        }
}

void AbrirQuarto::setLabel(QString numQuarto)
{
    ui->label->setText("Hospedar Quarto "+numQuarto);
}

void AbrirQuarto::Hospedar()
{
    QString obs = ui->textEdit->toPlainText();

    QSettings setting("RethsHotel","RethsHotelSetting");
    setting.beginGroup("UserHotelInfo");
    QString curUserId = setting.value("CurUserId").toString();
    setting.endGroup();

    QVariantMap userInput;

    userInput["hotelId"] = "1";
    userInput["idQuarto"] = this->QuartoId;
    userInput["obs"] = obs;
    userInput["Id_usuario"] = curUserId;

    QJsonDocument userInputJsonFormat;

    userInputJsonFormat = userInputJsonFormat.fromVariant(userInput);

    QNetworkReply *reply = networkRequest.makeRequest("http://127.0.0.1:5000/api/HospedarQuarto","post",userInputJsonFormat,5000);
    QObject::connect(reply, &QNetworkReply::finished, this,[=](){

        QJsonDocument jsonResponse = QJsonDocument::fromJson(QString(reply->readAll()).toUtf8());
        if (jsonResponse.isEmpty()) return;

        QString Status = jsonResponse.object().take("Status").toString();


        if(Status != "Sucesso"){
            QMessageBox::warning(nullptr,"error","Algo deu errado ao hospedar o quarto.");
            return;
        }

        QString Reserva_id = jsonResponse.object().take("Reserva_id").toString();

        emit(QuartoHospedadoComSucesso(Reserva_id));

        this->close();


    });
}




void AbrirQuarto::on_btn_alugar_clicked()
{
    Hospedar();
}

