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

int accessBDD::verificationAdherent(QString tag_RFID)
{
    int id=-1;
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
            id = requetePreparee.value(0).toInt();

        }
    }
    return id;
}

int accessBDD::recherchePremiereFois(QString dateNaissance, int num_badge)
{
    int id=-1;
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
            id = requetePreparee.value(0).toInt();

        }
    }
    return id;
}

bool accessBDD::enregistrementAdherent(QString tag_RFID, int id)
{
    bool retour = false;
    QSqlQuery requetePreparee;
    requetePreparee.prepare("UPDATE adherents SET tag_RFID = :tag_RFID WHERE id = :id;");
    requetePreparee.bindValue(":tag_RFID",tag_RFID);
    requetePreparee.bindValue(":id",id);
    if (requetePreparee.exec())
    {
        retour=true;
    }
    else
    {
       qDebug()<<requetePreparee.lastError().text() << "verif adherent";
    }
    return retour;
}

QJsonArray accessBDD::recupererMaterielDisponible()
{
    QJsonArray emplacementMateriel;
    QSqlQuery requete("select id, nom, etat, id_casier, image from materiels where etat = 'Disponible' or etat='Indisponible';");
    if(requete.exec()){
        while(requete.next())
        {
            QJsonObject materiel;
            materiel["id"]=requete.value(0).toInt();
            materiel["nom"]=requete.value(1).toString();
            materiel["etat"]=requete.value(2).toString();
            materiel["id_casier"]=requete.value(3).toInt();
            materiel["image"]=requete.value(4).toString();
            emplacementMateriel.append(materiel);
        }
    }
    return emplacementMateriel;
}

QJsonArray accessBDD::recupererMaterielEnStock()
{
    QJsonArray emplacementMateriel;
    QSqlQuery requete("select id, nom from materiels where etat = 'En Stock' and id_casier IS NULL;");
    if(requete.exec()){
        while(requete.next())
        {
            QJsonObject materiel;
            materiel["id"]=requete.value(0).toInt();
            materiel["nom"]=requete.value(1).toString();
            emplacementMateriel.append(materiel);
        }
    }
    return emplacementMateriel;
}

int accessBDD::recupererDureeEmprunt(int id_materiel)
{
    int dureeEmprunt = -1;
    QSqlQuery requetePreparee;
    requetePreparee.prepare("select duree_emprunt from materiels where id = :id_materiel;");
    requetePreparee.bindValue(":id_materiel",id_materiel);
    if (requetePreparee.exec())
    {
        if (requetePreparee.next()) {
            dureeEmprunt = requetePreparee.value(0).toInt();
        }
    }
    return dureeEmprunt;
}

void accessBDD::attribuerMateriel(QString date_emprunt, QString date_limite, int id_adherent, int id_materiel)
{
    QSqlQuery requetePreparee;
    requetePreparee.prepare("insert into emprunts(date_emprunt,date_limite,id_adherent,id_materiel) values(:date_emprunt,:date_limite,:id_adherent,:id_materiel);");
    requetePreparee.bindValue(":date_emprunt",date_emprunt);
    requetePreparee.bindValue(":date_limite",date_limite);
    requetePreparee.bindValue(":id_adherent",id_adherent);
    requetePreparee.bindValue(":id_materiel",id_materiel);
    if (!requetePreparee.exec())
    {
        qDebug()<<requetePreparee.lastError().text() << "verif adherent";
    }
}

void accessBDD::changementIndisponibilite(int id_materiel)
{
    QSqlQuery requetePreparee;
    requetePreparee.prepare("update materiels set etat = 'Indisponible' where id = :id_materiel;");
    requetePreparee.bindValue(":id_materiel",id_materiel);
    if (!requetePreparee.exec())
    {
        qDebug()<<requetePreparee.lastError().text() << "verif adherent";
    }
}

int accessBDD::verificationAdmin(QString tag_RFID)
{
    int admin=-1;
    QSqlQuery requetePreparee;
    requetePreparee.prepare("select Admin from adherents where tag_RFID = :tag_RFID;");
    requetePreparee.bindValue(":tag_RFID",tag_RFID);
    if (!requetePreparee.exec())
    {
        qDebug()<<requetePreparee.lastError().text() << "verif adherent";
    }
    else
    {
        if (requetePreparee.next()) {
            admin = requetePreparee.value(0).toInt();

        }
    }
    return admin;
}

void accessBDD::changementDisponiblite(int id_materiel)
{
    QSqlQuery requetePreparee;
    requetePreparee.prepare("update materiels set etat = 'Disponible' where id = :id_materiel;");
    requetePreparee.bindValue(":id_materiel",id_materiel);
    if (!requetePreparee.exec())
    {
        qDebug()<<requetePreparee.lastError().text() << "verif adherent";
    }
}

QJsonArray accessBDD::demanderMaterielEmprunter(int id_adherent)
{
    QJsonArray emplacementMateriel;
    QSqlQuery requetePreparee;
    requetePreparee.prepare("select nom, date_emprunt, date_limite, materiels.id from emprunts, materiels where materiels.id = emprunts.id_materiel and emprunts.id_adherent = :id_adherent and date_retour IS NULL;");
    requetePreparee.bindValue(":id_adherent",id_adherent);
    if(requetePreparee.exec())
    {
        while(requetePreparee.next()) // Parcourez tous les résultats de la requête
        {
            QJsonObject materiel;
            materiel["nom"]=requetePreparee.value(0).toString();
            materiel["date_emprunt"]=requetePreparee.value(1).toString();
            materiel["date_limite"]=requetePreparee.value(2).toString();
            materiel["id_materiel"]=requetePreparee.value(3).toInt();
            emplacementMateriel.append(materiel);
        }
    }
    return emplacementMateriel;
}

int accessBDD::recupererIdEmprunts(int id_adherent)
{
    int idEmprunt = -1;
    QSqlQuery requetePreparee;
    requetePreparee.prepare("select emprunts.id from emprunts, adherents where emprunts.id_adherent = adherents.id and adherents.id = :id_adherent and emprunts.date_retour IS NULL;");
    requetePreparee.bindValue(":id_adherent",id_adherent);
    if(requetePreparee.exec())
    {
        while(requetePreparee.next()) // Parcourez tous les résultats de la requête
        {
            idEmprunt = requetePreparee.value(0).toInt();
        }
    }
    return idEmprunt;
}

void accessBDD::ajoutDateRetour(int id_emprunts, QString date_retour)
{
    QSqlQuery requetePreparee;
    requetePreparee.prepare("update emprunts set date_retour = :date_retour where id = :id_emprunt");
    requetePreparee.bindValue(":id_emprunt",id_emprunts);
    requetePreparee.bindValue(":date_retour",date_retour);
    if(requetePreparee.exec())
    {

    }
}

int accessBDD::recupererIdMaterielEmprunter(int id_adherent)
{
    int materielEmprunter =-1;
    QSqlQuery requetePreparee;
    requetePreparee.prepare("select id_materiel from emprunts where id_adherent = :id_adherent and date_retour IS NULL;");
    requetePreparee.bindValue(":id_adherent",id_adherent);
    if(requetePreparee.exec())
    {
        while(requetePreparee.next()) // Parcourez tous les résultats de la requête
        {
            materielEmprunter = requetePreparee.value(0).toInt();
        }
    }
    return materielEmprunter;
}

void accessBDD::retirerObjet(int id_materiel)
{
    QSqlQuery requetePreparee;
    requetePreparee.prepare("update materiels set etat = 'En Stock', id_casier = NULL where id = :id_materiel;");
    requetePreparee.bindValue(":id_materiel",id_materiel);
    if (!requetePreparee.exec())
    {
        qDebug()<<requetePreparee.lastError().text() << "verif adherent";
    }
}

void accessBDD::ajouterObjet(int id_casier, int id_materiel)
{
    QSqlQuery requetePreparee;
    requetePreparee.prepare("update materiels set etat = 'Disponible', id_casier = :id_casier where id = :id_materiel;");
    requetePreparee.bindValue(":id_casier",id_casier);
    requetePreparee.bindValue(":id_materiel",id_materiel);
    if (!requetePreparee.exec())
    {
        qDebug()<<requetePreparee.lastError().text() << "verif adherent";
    }
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
