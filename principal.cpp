#include "principal.h"
#include "ui_principal.h"





Principal::Principal(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Principal)
{
    ui->setupUi(this);

    QHeaderView* header =  ui->tableWidget_consumo->horizontalHeader();
    header->setSectionResizeMode(0,QHeaderView::Stretch);

    QVariantMap userInput;

    userInput["hotelId"] = "1";

    QJsonDocument userInputJsonFormat;

    userInputJsonFormat = userInputJsonFormat.fromVariant(userInput);

    QNetworkReply *reply = networkRequest.makeRequest("http://127.0.0.1:5000/api/GetAllRooms","post",userInputJsonFormat,5000);
    QObject::connect(reply, &QNetworkReply::finished, this,[=](){


        QJsonDocument jsonResponse = QJsonDocument::fromJson(QString(reply->readAll()).toUtf8());
        if (jsonResponse.isEmpty()) return;

       for(int i=0;true;i++){
           if(jsonResponse[i].isUndefined()) break;



           QString NumeroQuarto = jsonResponse[i]["numQuarto"].toString();
           QString QuartoId = QString::number(jsonResponse[i]["Id"].toInt());
           QString StatusQuarto = jsonResponse[i]["Status"].toString();
           QString TipoQuarto = jsonResponse[i]["Tipo"].toString();
           QString ReservaId = QString::number(jsonResponse[i]["IdReserva"].toInt());


           QPushButton *btn = new QPushButton(NumeroQuarto+"\n\n"+TipoQuarto);

           QString backColor;

           if(StatusQuarto == "Disponivel")backColor = "background-color: green;";
           else if (StatusQuarto == "Ocupado") backColor = "background-color: red;";
           else  backColor = "background-color: blue;";

           QString style = "color:white;padding-top:5px;Text-align:top;font: 700 14pt 'Nirmala UI';"+backColor;

           btn->setStyleSheet(style);
           btn->setMinimumHeight(90);
           btn->setCursor(Qt::PointingHandCursor);

           btn->setProperty("QuartoId",QuartoId);
           btn->setProperty("NumeroQuarto",NumeroQuarto);
           btn->setProperty("Status",StatusQuarto);

           btn->setProperty("ReservaId",ReservaId);



           connect(btn,&QPushButton::clicked,this,&Principal::GoToHospedes);

           ui->LayoutQuartos->addWidget(btn);
       }

    });
}

Principal::~Principal()
{
    delete ui;
}

void Principal::GoToHospedes()
{
    QObject *btn = sender();
    QPushButton* button = qobject_cast<QPushButton*>(sender());

    int quartoId = btn->property("QuartoId").toInt();
    QString numeroQuarto = btn->property("NumeroQuarto").toString();
    QString StatusQuarto = btn->property("Status").toString();

    if (StatusQuarto == "Disponivel"){

        AbrirQuarto window(nullptr,quartoId);
        connect(&window,&AbrirQuarto::QuartoHospedadoComSucesso,this,[button] (QString reservaId){

            QString style = "color:white;padding-top:5px;Text-align:top;font: 700 14pt 'Nirmala UI';background-color:red";
            button->setStyleSheet(style);
            button->setProperty("ReservaId",reservaId);
            button->setProperty("Status","Ocupado");

        });
        window.setLabel(numeroQuarto);
        window.exec();
    }

    else if (StatusQuarto == "Ocupado"){



        QString reservaId = button->property("ReservaId").toString();

        QVariantMap userInput;
        userInput["hotelId"] = "1";
        userInput["quartoId"] = QString::number(quartoId);
        userInput["reservaId"] = reservaId;

        QJsonDocument userInputJsonFormat;
        userInputJsonFormat = userInputJsonFormat.fromVariant(userInput);

        QNetworkReply *reply = networkRequest.makeRequest("http://127.0.0.1:5000/api/informacoesDoQuarto","post",userInputJsonFormat,5000);
        QObject::connect(reply, &QNetworkReply::finished, this,[=] (){
            ui->tableWidget_consumo->setRowCount(0);
            QJsonDocument jsonResponse = QJsonDocument::fromJson(QString(reply->readAll()).toUtf8());
            if (jsonResponse.isEmpty()) return;

            QJsonObject reservaInfo = jsonResponse["ReservaInfo"].toObject();

            QuartoInfo["ReservaId"] = reservaId;
            QuartoInfo["quartoId"] = QString::number(quartoId);
            QuartoInfo["numeroQuarto"] = numeroQuarto;
            QuartoInfo["StatusQuarto"] = StatusQuarto;
            curPushButton = button;

            ui->label_horario_entrada->setText("Horário de Entrada: "+reservaInfo["HorarioEntrada"].toString());
            ui->textEdit->setText(reservaInfo["obs"].toString());
            ui->stackedWidget->setCurrentIndex(1);

            QJsonArray consumoInfo = jsonResponse["ConsumoInfo"].toArray();


            for(int i=0; i < consumoInfo.size();i++){

                int Qtd = consumoInfo.at(i).toObject()["Qtd"].toInt();
                QString precoTotal = consumoInfo.at(i).toObject()["precoTotal"].toString();
                QString descricao = consumoInfo.at(i).toObject()["Descricao"].toString();
                QString precoUn = consumoInfo.at(i).toObject()["precoUn"].toString();
                int IdRegistro = consumoInfo.at(i).toObject()["IdRegistro"].toInt();

                QTableWidgetItem *item = new QTableWidgetItem;
                item->setText(descricao+"  "+precoUn+" X "+QString::number(Qtd)+" = "+precoTotal);
                item->setFont(QFont("",12));


                QPushButton *btn = new QPushButton;
                btn->setIcon(QIcon(":/Icons White/Icons White/trash-2.svg"));
                btn->setIconSize(QSize(24,24));
                btn->setFlat(true);
                btn->setProperty("RegistroId",QString::number(IdRegistro));


                connect(btn,&QPushButton::clicked,this,[this,btn] (){

                    QWidget *w = qobject_cast<QWidget *>(sender()->parent());
                        if(w){

                            int row = ui->tableWidget_consumo->indexAt(w->pos()).row();
                            QString Id_registro = btn->property("RegistroId").toString();

                            deletarConsumo(row,Id_registro);

                        }

                });

                ConsumoInfo.insert(QString::number(IdRegistro),btn);

                btn->setCursor(Qt::PointingHandCursor);


                ui->tableWidget_consumo->setRowCount(consumoInfo.size());



                ui->tableWidget_consumo->setItem(i,0,item);
                ui->tableWidget_consumo->setCellWidget(i,1,btn);



            }

          });


    }
    else if (StatusQuarto == "Limpando"){

        QString menssage = "deseja abrir o quarto?";

        if(QMessageBox::Yes != QMessageBox::question(nullptr,"terminar",menssage)){
            return;
        }

        QVariantMap userInput;
        userInput["hotelId"] = "1";
        userInput["QuartoId"] = QString::number(quartoId);

        QJsonDocument userInputJsonFormat;
        userInputJsonFormat = userInputJsonFormat.fromVariant(userInput);

        QNetworkReply *reply = networkRequest.makeRequest("http://127.0.0.1:5000/api/abrirQuarto","post",userInputJsonFormat,5000);
        QObject::connect(reply, &QNetworkReply::finished, this,[=] (){

        QJsonDocument jsonResponse = QJsonDocument::fromJson(QString(reply->readAll()).toUtf8());
        if (jsonResponse.isEmpty()) return;

        QString Status= jsonResponse.object().take("Status").toString();

        qDebug() <<  Status;

        if(Status != "Sucesso"){
            QMessageBox::warning(nullptr,"error","algo deu erro ao abrir o quarto.");
            return;
        }
        button->setStyleSheet("color:white;padding-top:5px;Text-align:top;font: 700 14pt 'Nirmala UI';background-color:green;");
        button->setProperty("Status","Disponivel");
        });

    }
    
}





void Principal::on_btn_voltarListaQuartos_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    ConsumoInfo.clear();
    ui->tableWidget_consumo->clearContents();
    ui->tableWidget_consumo->setRowCount(0);
    ui->textEdit->clear();
    QuartoInfo.clear();
    curPushButton = nullptr;
}






void Principal::on_pushButton_registrar_consumo_clicked()
{


    RegistrarConsumo windowConsumo(nullptr,QuartoInfo["ReservaId"]);

    connect(&windowConsumo,&RegistrarConsumo::produtoConsumoRegistrado,this,[this] (QMap<QString,QString> Resposta){

        if(Resposta["Status"] == "Produto ja Existente"){


            QPushButton *curBtn =  ConsumoInfo.value(Resposta["IdRegistro"]);
            QTableWidgetItem *item = new QTableWidgetItem;
            item->setText(Resposta["descricao"]+"  "+Resposta["precoUn"]+" X "+Resposta["Qtd"]+" = "+Resposta["precoTotal"]);
            item->setFont(QFont("",12));
            QWidget *w = qobject_cast<QWidget *>(curBtn->parent());
            if(w){
                int row = ui->tableWidget_consumo->indexAt(w->pos()).row();
                ui->tableWidget_consumo->setItem(row,0,item);
                return;
            }

        }

        else if (Resposta["Status"] == "Produto nao Existente"){

            QTableWidgetItem *item = new QTableWidgetItem;
            item->setText(Resposta["descricao"]+"  "+Resposta["precoUn"]+" X "+Resposta["Qtd"]+" = "+Resposta["precoTotal"]);
            item->setFont(QFont("",12));

            QPushButton *btn = new QPushButton;
            btn->setIcon(QIcon(":/Icons White/Icons White/trash-2.svg"));
            btn->setIconSize(QSize(24,24));
            btn->setFlat(true);
            btn->setProperty("RegistroId",Resposta["IdRegistro"]);

            connect(btn,&QPushButton::clicked,this,[this,btn] (){

                QWidget *w = qobject_cast<QWidget *>(sender()->parent());
                    if(w){

                        int row = ui->tableWidget_consumo->indexAt(w->pos()).row();
                        QString Id_registro = btn->property("RegistroId").toString();

                        deletarConsumo(row,Id_registro);

                    }

            });

            ConsumoInfo.insert(Resposta["IdRegistro"],btn);

            btn->setCursor(Qt::PointingHandCursor);

            int currentRowCount = ui->tableWidget_consumo->rowCount();
            ui->tableWidget_consumo->setRowCount(currentRowCount+1);

            QHeaderView* header =  ui->tableWidget_consumo->horizontalHeader();
            header->setSectionResizeMode(0,QHeaderView::Stretch);


            ui->tableWidget_consumo->setItem(currentRowCount,0,item);
            ui->tableWidget_consumo->setCellWidget(currentRowCount,1,btn);



        }else{return; }

    });


    windowConsumo.exec();

}
void Principal::deletarConsumo(int row, QString id_Registro_consumo)
{
    QVariantMap userInput;

    userInput["hotelId"] = "1";
    userInput["ConsumoRegistroId"] = id_Registro_consumo;

    QJsonDocument userInputJsonFormat;

    userInputJsonFormat = userInputJsonFormat.fromVariant(userInput);

    QNetworkReply *reply = networkRequest.makeRequest("http://127.0.0.1:5000/api/DeletarConsumos","post",userInputJsonFormat,5000);
    QObject::connect(reply, &QNetworkReply::finished, this,[this,row,reply,id_Registro_consumo] (){
        
        QJsonDocument jsonResponse = QJsonDocument::fromJson(QString(reply->readAll()).toUtf8());
        if (jsonResponse.isEmpty()) return;
        
        QString Status= jsonResponse.object().take("Status").toString();

        if(Status != "Deletado Com Sucesso"){
            QMessageBox::warning(nullptr,"error","Não foi possivel deletar o consumo.");
            return;
        }

        ConsumoInfo.take(id_Registro_consumo);
        ui->tableWidget_consumo->removeRow(row);

        
    });
}







//fazer checkout
void Principal::on_pushButton_checkout_clicked()
{
    FazerCheckOut windowCheckOut(nullptr,QuartoInfo.value("ReservaId"));

    connect(&windowCheckOut,&FazerCheckOut::checkoutComSucesso,this,[=] (){

    curPushButton->setStyleSheet("color:white;padding-top:5px;Text-align:top;font: 700 14pt 'Nirmala UI';background-color:blue;");
    curPushButton->setProperty("Status","Limpando");
    ui->stackedWidget->setCurrentIndex(0);

    });

    windowCheckOut.exec();


}











