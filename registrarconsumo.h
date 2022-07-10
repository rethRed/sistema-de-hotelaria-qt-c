#ifndef REGISTRARCONSUMO_H
#define REGISTRARCONSUMO_H

#include <QDialog>
#include <QListWidgetItem>
#include "requests.h"
#include <QMessageBox>

namespace Ui {
class RegistrarConsumo;
}

class RegistrarConsumo : public QDialog
{
    Q_OBJECT

public:
    explicit RegistrarConsumo(QWidget *parent = nullptr,QString reservaId = "");
    ~RegistrarConsumo();
    QString reservaId;


signals:
    void produtoConsumoRegistrado(QMap <QString,QString> Resposta);

private slots:



    void on_pushButton_pesquisar_clicked();

    void on_listWidget_consumos_itemDoubleClicked(QListWidgetItem *item);

private:
    Ui::RegistrarConsumo *ui;
    Requests networkRequest;
};

#endif // REGISTRARCONSUMO_H
