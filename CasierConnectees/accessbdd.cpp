#include "accessbdd.h"

accessBDD::accessBDD() {
    chargerParametresBdd();
    bdd = QSqlDatabase::addDatabase("QMYSQL");
    bdd.setHostName(adresse);
    bdd.setDatabaseName(nomBase);
    bdd.setUserName(login);
    bdd.setPassword(motDePasse);
    if(!bdd.open()){
        QMessageBox msg;
        msg.setText("Erreur de connexion bdd" + bdd.lastError().text());
        msg.exec();
    }
    else
    {
        qDebug() << "Ouverture de la base de données" << bdd.databaseName();
    }
}

QString accessBDD::verificationAdherent(QString tag_RFID)
{
    QSqlQuery requetePreparee;
    requetePreparee.prepare("select id from adherents where :tag_RFID = tag_RFID;");
    requetePreparee.bindValue(":tag_RFID",tag_RFID);
    if (!requetePreparee.exec())
    {
        qDebug()<<requetePreparee.lastError().text() << "verif adherent";
    }
    else
    {
        if (requetePreparee.next()) {
            QString id = requetePreparee.value(0).toString();
            return id;
        }
    }
}

QString accessBDD::rechercheFirstTime(QString dateNaissance, QString num_badge)
{
    QSqlQuery requetePreparee;
    requetePreparee.prepare("select id from adherents where date_naissance = :date_naissance and num_badge = :num_badge;");
    requetePreparee.bindValue(":date_naissance",dateNaissance);
    requetePreparee.bindValue(":num_badge",num_badge);
    if (!requetePreparee.exec())
    {
        qDebug()<<requetePreparee.lastError().text() << "verif adherent";
    }
    else
    {
        if (requetePreparee.next()) {
            QString id = requetePreparee.value(0).toString();
            return id;
        }
    }
    return "PbRechercheFirstTime";
}

bool accessBDD::enregistrementAdherent(QString tag_RFID, QString id)
{
    QSqlQuery requetePreparee;
    requetePreparee.prepare("UPDATE adherents SET tag_RFID = :tag_RFID WHERE id = :id;");
    requetePreparee.bindValue(":tag_RFID",tag_RFID);
    requetePreparee.bindValue(":id",id);
    if (!requetePreparee.exec())
    {
        qDebug()<<requetePreparee.lastError().text() << "verif adherent";
    }
    else
    {
        if (requetePreparee.next()) {
            // Il y a au moins une ligne de résultat
            return true;
        } else {
            return false;
        }
    }
    return false;
}

QJsonArray accessBDD::emplacementMaterielEmprunter()
{
    QJsonArray emplacementMateriel;
    QSqlQuery requete("select id, nom, etat, id_casier from materiels where etat = 'Disponible' or etat='Indisponible';");
    if(requete.exec()){
        while(requete.next())
        {
            QJsonObject materiel;
            materiel["id"]=requete.value(0).toString();
            materiel["nom"]=requete.value(1).toString();
            materiel["etat"]=requete.value(2).toString();
            materiel["id_casier"]=requete.value(3).toString();
            emplacementMateriel.append(materiel);
        }
    }
    return emplacementMateriel;
}

QJsonArray accessBDD::emplacementMaterielRemplir()
{
    QJsonArray emplacementMateriel;
    QSqlQuery requete("select id, nom from materiels where etat = 'En Stock' and id_casier = NULL;");
    if(requete.exec()){
        while(requete.next())
        {
            QJsonObject materiel;
            materiel["id"]=requete.value(0).toString();
            materiel["nom"]=requete.value(1).toString();
            emplacementMateriel.append(materiel);
        }
    }
    return emplacementMateriel;
}

void accessBDD::lierMateriel(QString tag_RFID)
{
    QSqlQuery requetePreparee;
    requetePreparee.prepare("insert into emprunts(date_emprunt,date_retour,id_adherent,id_materiel) values('a','a',532,2);");
    requetePreparee.bindValue(":tag_RFID",tag_RFID);
    if (!requetePreparee.exec())
    {
        qDebug()<<requetePreparee.lastError().text() << "verif adherent";
    }

}


void accessBDD::delierMateriel()
{

}

void accessBDD::demanderMaterielEmprunter()
{

}

void accessBDD::enregistrementEmplacement()
{

}

void accessBDD::enregistrementImage()
{

}


void accessBDD::chargerParametresBdd()
{
    QString nomFichierIni=":/accessBDD/accessBDD.ini";
    QFileInfo testFichier(nomFichierIni);
    if(testFichier.exists() && testFichier.isFile()){
        QSettings paramsSocket(nomFichierIni, QSettings::IniFormat);
        adresse = paramsSocket.value("BDD/adresseIp").toString();
        motDePasse =paramsSocket.value("BDD/motDePasse").toString();
        nomBase= paramsSocket.value("BDD/nomDeLaBase").toString();
        login = paramsSocket.value("BDD/login").toString();
    }
}
