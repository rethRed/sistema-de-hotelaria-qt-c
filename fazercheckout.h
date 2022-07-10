#ifndef FAZERCHECKOUT_H
#define FAZERCHECKOUT_H

#include <QDialog>
#include "requests.h"
#include <QLineEdit>
#include <QListWidgetItem>

namespace Ui {
class FazerCheckOut;
}

class FazerCheckOut : public QDialog
{
    Q_OBJECT

public:
    explicit FazerCheckOut(QWidget *parent = nullptr,QString reservaId = "");
    ~FazerCheckOut();

    QString tempoFechamento;

    QString reservaId;
    bool numeroEhDouble(const QString &userNumero);
    void validarLineEditDouble(QLineEdit *line);
    void getQuartoInfo();
    void recalcularTotal();
    double subTotal;
    double taxa;
    double desconto;
    double total;
    double totalPago;
    double troco;

    QMap<QString,QListWidgetItem*> metodosPagamentos;



signals:
    void checkoutComSucesso();

private slots:
    void on_lineEdit_subTotal_textChanged();


    void on_lineEdit_taxa_textChanged();

    void on_lineEdit_desconto_textChanged();

    void on_pushButton_confirmar_clicked();

    void on_lineEdit_quantia_dinheiro_textChanged();

    void on_lineEdit_quantia_dinheiro_returnPressed();

    void on_btn_voltar_clicked();

    void on_listWidget_pagamento_itemDoubleClicked(QListWidgetItem *item);

    void on_pushButton_finalizar_clicked();

private:
    Ui::FazerCheckOut *ui;
    Requests networkRequest;


};

#endif // FAZERCHECKOUT_H
