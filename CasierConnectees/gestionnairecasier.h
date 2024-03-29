#ifndef GESTIONNAIRECASIER_H
#define GESTIONNAIRECASIER_H

#include <QFile>
#include <QWidget>
#include <QAbstractButton>
#include <QAbstractItemView>
#include <QTimer>
#include <QToolButton>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QPushButton>
#include <QImage>
#include <QSettings>
#include <QJsonObject>
#include <QFileInfo>
#include <QPixmap>
#include <QMessageBox>
#include "accessbdd.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class GestionnaireCasier;
}
QT_END_NAMESPACE

class GestionnaireCasier : public QWidget
{
    Q_OBJECT

public:
    GestionnaireCasier(QWidget *parent = nullptr);
    ~GestionnaireCasier();

private slots:
//Perso
    void connectButtonsPerso();
    void RedirectPerso();
//Styles
    void Style();
//Premiere Identification
    //Slots Calculatrice
    void connectButtonsNumeroBadge();
    void AddNumber();

    //Slots Date De Naissance
    void connectButtonsDateDeNaissance();
    void RedirectPressEvent();
    void RedirectReleaseEvent();
    void on_TimerPressEventEnd();
    void checkAndUpdateDays();
//Remplir
    void connectButtonsRemplissage();
    void ChoixRemplissage();
//Emprunt
    void connectButtonsEmprunt();
    void ChoixEmprunt();
    void RedirectEmprunt();
//Restitution
    void connectButtonsRestitution();
    void RedirectRestitution();
private:
    QTimer *pressEvent;
    QString idUser;
    accessBDD BDD;
    QToolButton *currentButtonChoixEmprunt;
    QToolButton *currentButtonRestitution;
    QToolButton *currentButtonNumeroBadge;
    QToolButton *currentButtonDateNaissance;
    QToolButton *currentButtonRemplirCasier = NULL;
    QPushButton *currentButtonRemplirMateriel = NULL;
    Ui::GestionnaireCasier *ui;
};
#endif // GESTIONNAIRECASIER_H
