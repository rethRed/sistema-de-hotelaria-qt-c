#include "fazercheckout.h"
#include "ui_fazercheckout.h"

FazerCheckOut::FazerCheckOut(QWidget *parent,QString reservaId) :
    QDialog(parent),
    ui(new Ui::FazerCheckOut)
{
    ui->setupUi(this);
    this->reservaId = reservaId;
    getQuartoInfo();
    
}

FazerCheckOut::~FazerCheckOut()
{
    delete ui;
}



void FazerCheckOut::getQuartoInfo()
{
    QVariantMap userInput;

    userInput["hotelId"] = "1";
    userInput["reservaId"] = this->reservaId;

    QJsonDocument userInputJsonFormat;

    userInputJsonFormat = userInputJsonFormat.fromVariant(userInput);

    QNetworkReply *reply = networkRequest.makeRequest("http://127.0.0.1:5000/api/getCheckoutInfo","post",userInputJsonFormat,5000);
    QObject::connect(reply, &QNetworkReply::finished, this,[=] (){

        QJsonDocument jsonResponse = QJsonDocument::fromJson(QString(reply->readAll()).toUtf8());
        if (jsonResponse.isEmpty()) return;
        
        QString Status= jsonResponse.object().take("Status").toString();

        if(Status != "Enviado Com Sucesso"){
            QMessageBox::warning(nullptr,"error","Não foi possivel pegar as informações da reserva.");
            this->close();
            return;
        }
        QString horarioEntrada= jsonResponse.object().take("HorarioEntrada").toString();
        QString horarioSaida= jsonResponse.object().take("HorarioSaida").toString();
        QString tempoPermanencia= jsonResponse.object().take("tempoPermanecido").toString();
        double totalConsumo = jsonResponse.object().take("TotalConsumo").toDouble();
        double valorSugerido = jsonResponse.object().take("ValorSugerido").toDouble();


        QJsonArray listaPagamentos = jsonResponse.object().take("MetodoPagamento").toArray();

        for(int i=0; i < listaPagamentos.size();i++){
            ui->comboBox_metodo_pagamento->addItem(listaPagamentos.at(i)["Pagamento"].toString());
        }

        this->tempoFechamento = jsonResponse.object().take("horarioSaidaNotFormated").toString();

        ui->label_horario_entrada->setText("Entrada: "+horarioEntrada);
        ui->label_horario_saida->setText("Saida: "+horarioSaida);
        ui->label_tempo_permanencia->setText("Tempo de permanencia: "+tempoPermanencia);
        ui->label_valor_sugerido->setText("Valor Sugerido: "+QString::number(valorSugerido,'f',2).replace(".",","));
        ui->label_total_consumos->setText("Total Consumos: "+QString::number(totalConsumo,'f',2).replace(".",","));

        subTotal = valorSugerido + totalConsumo;


        ui->lineEdit_subTotal->setText(QString::number(subTotal,'f',2).replace(".",","));
        ui->lineEdit_subTotal->setAttribute(Qt::WA_MacShowFocusRect, 1);

        total = subTotal + taxa - desconto;

        ui->label_total->setText("Total: "+QString::number(total,'f',2).replace(".",","));


    });
}

void FazerCheckOut::recalcularTotal()
{
    this->total = this->subTotal + this->taxa - this->desconto;
    ui->label_total->setText("Total: "+QString::number(this->total,'f',2).replace(".",","));
}


bool FazerCheckOut::numeroEhDouble(const QString &userNumero)
{
    QString numero = userNumero;
    numero.replace(",",".");

    bool podeSerDouble;
    numero.toDouble(&podeSerDouble);

    return podeSerDouble;

}

void FazerCheckOut::validarLineEditDouble(QLineEdit *line)
{
    QString lineText = line->text();

    if(numeroEhDouble(lineText)){
        return;
    }else{
        line->backspace();
    }

}


void FazerCheckOut::on_lineEdit_subTotal_textChanged()
{
   validarLineEditDouble(ui->lineEdit_subTotal);
   this->subTotal = ui->lineEdit_subTotal->text().replace(",",".").toDouble();
   recalcularTotal();
}


void FazerCheckOut::on_lineEdit_taxa_textChanged()
{
    validarLineEditDouble(ui->lineEdit_taxa);
    this->taxa = ui->lineEdit_taxa->text().replace(",",".").toDouble();
    recalcularTotal();
}


void FazerCheckOut::on_lineEdit_desconto_textChanged()
{
    validarLineEditDouble(ui->lineEdit_desconto);
    this->desconto = ui->lineEdit_desconto->text().replace(",",".").toDouble();
    recalcularTotal();
}

void FazerCheckOut::on_lineEdit_quantia_dinheiro_textChanged()
{
    validarLineEditDouble(ui->lineEdit_quantia_dinheiro);
}


void FazerCheckOut::on_pushButton_confirmar_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
    ui->label_total_de_Quanto_Deu->setText("Total: "+QString::number(total,'f',2).replace(".",","));
    ui->label_total_pago->setText("Total Pago: "+QString::number(totalPago,'f',2).replace(".",","));
    ui->label_total_troco->setText("Troco: "+QString::number(troco,'f',2).replace(".",","));


}


void FazerCheckOut::on_lineEdit_quantia_dinheiro_returnPressed()
{
    QString NomePagamento = ui->comboBox_metodo_pagamento->currentText();

    double valorPagamento = ui->lineEdit_quantia_dinheiro->text().replace(",",".").toDouble();

    if(metodosPagamentos.contains(NomePagamento)){

        QListWidgetItem *itemProduto = metodosPagamentos.value(NomePagamento);
        double valorCurPagamento = itemProduto->whatsThis().toDouble();

        QString novoValor = QString::number(valorCurPagamento + valorPagamento);
        itemProduto->setWhatsThis(novoValor);

        itemProduto->setText(NomePagamento+" R$"+QString::number(valorPagamento + valorCurPagamento,'f',2).replace(".",","));

    }else{
        QListWidgetItem *itemProduto = new QListWidgetItem;
        itemProduto->setWhatsThis(QString::number(valorPagamento));
        itemProduto->setText(NomePagamento+" R$"+QString::number(valorPagamento,'f',2).replace(".",","));
        ui->listWidget_pagamento->addItem(itemProduto);
        metodosPagamentos.insert(NomePagamento,itemProduto);
    }

    ui->lineEdit_quantia_dinheiro->clear();
    totalPago += valorPagamento;

    if(totalPago >= total ){
        troco = totalPago - total;
    }
    ui->label_total_pago->setText("Total Pago: "+QString::number(totalPago,'f',2).replace(".",","));
    ui->label_total_troco->setText("Troco: "+QString::number(troco,'f',2).replace(".",","));

}


void FazerCheckOut::on_btn_voltar_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}


void FazerCheckOut::on_listWidget_pagamento_itemDoubleClicked(QListWidgetItem *item)
{
    double valorCurPagamento = item->whatsThis().toDouble();

    totalPago -= valorCurPagamento;

    if(totalPago >= total ){
        troco = totalPago - total;
    }else troco = 0.00;

    ui->label_total_pago->setText("Total Pago: "+QString::number(totalPago,'f',2).replace(".",","));
    ui->label_total_troco->setText("Troco: "+QString::number(troco,'f',2).replace(".",","));

    metodosPagamentos.remove(metodosPagamentos.key(item));
    delete item;

}


void FazerCheckOut::on_pushButton_finalizar_clicked()
{
    QVariantMap userInput;
    QVariantMap listaDePagamento;

    userInput["hotelId"] = "1";
    userInput["reservaId"] = this->reservaId;
    userInput["tempoSaida"] = this->tempoFechamento;
    userInput["Total"] = this->total;
    userInput["taxa"] = this->taxa;
    userInput["desconto"] = this->desconto;
    userInput["subTotal"] = this->subTotal;


    for(auto pagamento : metodosPagamentos.keys()){
        double valorPagoPeloMetodo = metodosPagamentos.value(pagamento)->whatsThis().toDouble();
        listaDePagamento.insert(pagamento,valorPagoPeloMetodo);
    }

    userInput["listaPagamentos"] = listaDePagamento;

    QJsonDocument userInputJsonFormat;

    userInputJsonFormat = userInputJsonFormat.fromVariant(userInput);

    QNetworkReply *reply = networkRequest.makeRequest("http://127.0.0.1:5000/api/completarCheckout","post",userInputJsonFormat,5000);
    QObject::connect(reply, &QNetworkReply::finished, this,[=] (){

        QJsonDocument jsonResponse = QJsonDocument::fromJson(QString(reply->readAll()).toUtf8());
        if (jsonResponse.isEmpty()) return;

        QString Status= jsonResponse.object().take("Status").toString();

        if(Status != "Sucesso"){
            QMessageBox::warning(nullptr,"error","Não foi possivel fazer checkout.");
            return;
        }
        this->close();
        emit(checkoutComSucesso());

    });
}

