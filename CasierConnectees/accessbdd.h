#ifndef ACCESSBDD_H
#define ACCESSBDD_H
#include <QFileInfo>
#include <QSettings>
#include <QString>
#include <QJsonArray>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QDebug>
#include <QJsonObject>

class accessBDD
{
public:
    accessBDD();
    bool verificationAdherent(QString tag_RFID);
    QString rechercheFirstTime(QString dateNaissance, QString num_badge);
    bool enregistrementAdherent(QString tag_RFID, QString id);
    QJsonArray emplacementMaterielEmprunter();
    QJsonArray emplacementMaterielRemplir();
    void lierMateriel(QString tag_RFID);
    void delierMateriel();
    void demanderMaterielEmprunter();
    void enregistrementEmplacement();
    void enregistrementImage();
private:
    QString login;
    QString motDePasse;
    QString adresse;
    QString nomBase;
    QSqlDatabase bdd;
    void chargerParametresBdd();
};

#endif // ACCESSBDD_H
