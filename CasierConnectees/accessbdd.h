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
#include <QtNetwork>
#include <QJsonObject>

class accessBDD
{
public:

    accessBDD();
    int verificationAdherent(QString tag_RFID);
    int rechercheFirstTime(QString dateNaissance, int num_badge);
    bool enregistrementAdherent(QString tag_RFID, int id);
    QJsonArray emplacementMaterielEmprunter();
    QJsonArray emplacementMaterielRemplir();
    int recupererDureeEmprunt(int id_materiel);
    void lierMateriel(QString date_emprunt, QString date_limite, int id_adherent, int id_materiel);
    void changementIndisponibilite(int id_materiel);
    int savoirSiAdmin(QString tag_RFID);
    void changementDisponiblite(int id_materiel);
    QJsonArray demanderMaterielEmprunter(int id_adherent);
    int recupererIdEmprunts(int id_adherent);
    void updateDateRetour(int id_emprunts, QString date_retour);
    int materielEmprunter(int id_adherent);
    void retirerObjet(int id_materiel);
    void ajouterObjet(int id_casier, int id_materiel);
private:
    QString login;
    QString motDePasse;
    QString adresse;
    QString nomBase;
    QSqlDatabase bdd;
    void chargerParametresBdd();
};

#endif // ACCESSBDD_H
