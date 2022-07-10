#ifndef ABRIRQUARTO_H
#define ABRIRQUARTO_H

#include <QDialog>
#include "requests.h"
#include <QMessageBox>

namespace Ui {
class AbrirQuarto;
}

class AbrirQuarto : public QDialog
{
    Q_OBJECT

public:
    explicit AbrirQuarto(QWidget *parent = nullptr, int QuartoId = 0);
    ~AbrirQuarto();
    int QuartoId;
    void deleteAllLayoutItens(QLayout *layout);
    void setLabel(QString numQuarto);
    void Hospedar();

signals:
    void QuartoHospedadoComSucesso(QString reservaId);

private slots:
    void on_btn_alugar_clicked();

private:
    Ui::AbrirQuarto *ui;
    Requests networkRequest;

};

#endif // ABRIRQUARTO_H
