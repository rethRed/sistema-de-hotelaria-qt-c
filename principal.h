#ifndef PRINCIPAL_H
#define PRINCIPAL_H

#include <QMainWindow>
#include <QPushButton>
#include <QLabel>
#include <QMap>
#include "requests.h"
#include "abrirquarto.h"
#include "registrarconsumo.h"
#include "fazercheckout.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Principal; }
QT_END_NAMESPACE

class Principal : public QMainWindow
{
    Q_OBJECT

public:
    Principal(QWidget *parent = nullptr);
    ~Principal();
    void GoToHospedes();
    void deletarConsumo(int row, QString id_Registro_consumo);

    QPushButton *curPushButton;

private slots:
    void on_btn_voltarListaQuartos_clicked();

    void on_pushButton_registrar_consumo_clicked();

    void on_pushButton_checkout_clicked();

private:
    Ui::Principal *ui;
    Requests networkRequest;
    QMap<QString,QString> QuartoInfo;
    QMap <QString,QPushButton*> ConsumoInfo;
    QMap <QString,int> Consumo;

};


#endif // PRINCIPAL_H
