#include "gestionnairecasier.h"
#include "ui_gestionnairecasier.h"

GestionnaireCasier::GestionnaireCasier(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::GestionnaireCasier)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);
    showFullScreen();
    Style();
    connectButtonsEmprunt();
    connectButtonsRestitution();
    connectButtonsRetirer();
    connectButtonsRedirection();
    connectButtonsDateDeNaissance();
    connectButtonsNumeroBadge();
    connectButtonsRemplissage();
    //timerIDCarte.setSingleShot(true);
    //connect(&timerIDCarte,&QTimer::timeout,this,&GestionnaireCasier::RecupererIDCarte);
    //timerIDCarte.start(500);
    accessGache = new AccesGache(this);
    imageDownloader = new ImageDownloader(this);
    connect(imageDownloader, &ImageDownloader::imageDownloaded, this, &GestionnaireCasier::onImageDownloaded);
    connect(&lecteur, &Wiegand::NouvelleTrame, this, &GestionnaireCasier::on_NouvelleTrame);
}


GestionnaireCasier::~GestionnaireCasier()
{
    delete ui;
}

void GestionnaireCasier::on_NouvelleTrame(QString &tag_RFID)
{
    viderToolButton();
    tagRFID = tag_RFID;
    qDebug() << QString::number(BDD.verificationAdmin(tag_RFID));
    if(BDD.verificationAdmin(tag_RFID) == 1){
        ui->stackedWidget->setCurrentIndex(6);
    }
    if(BDD.recupererIdMaterielEmprunter(BDD.verificationAdherent(tag_RFID)) == -1 && BDD.verificationAdmin(tag_RFID) != 1){
        idUser = BDD.verificationAdherent(tag_RFID);
        ui->stackedWidget->setCurrentIndex(1);
        QJsonArray emplacementMateriel = BDD.recupererMaterielDisponible();
        for (int i = 0; i < emplacementMateriel.size(); ++i) {
            QJsonObject materiel = emplacementMateriel.at(i).toObject();
            QString nomBouton = materiel["nom"].toString();
            QString etatBouton = materiel["etat"].toString();
            int idCasierBouton = materiel["id_casier"].toInt();
            QString nomImage = materiel["image"].toString();
            // Assurez-vous que votre UI contient des objets QToolButton avec les noms correspondants
            QToolButton *toolButton = findChild<QToolButton*>(QString("toolbutton_Choix_Casier%1").arg(idCasierBouton));
            if (toolButton) {
                // Inclure le nom et l'état dans le setText()
                QString buttonText = nomBouton + "\n" + etatBouton;
                toolButton->setText(buttonText);
                toolButton->setProperty("id", materiel["id"].toInt());
                toolButton->setProperty("id_casier", materiel["id_casier"].toInt());
                downloadImage(nomImage, toolButton);
                // Appliquer la couleur en fonction de l'état
                if (etatBouton == "Indisponible") {
                    toolButton->setStyleSheet("color: red;background-color:orange;");
                } else if (etatBouton == "Disponible") {
                    toolButton->setStyleSheet("color: green;background-color:orange;");
                }
            }
        }
    }
    if(BDD.verificationAdherent(tag_RFID) == -1){
        ui->stackedWidget->setCurrentIndex(5);
    }

    if(BDD.recupererIdMaterielEmprunter(BDD.verificationAdherent(tag_RFID)) != -1 && BDD.verificationAdmin(tag_RFID) != 1){
        ui->label_ObjetRestitution->clear();
        ui->label_TempsRestitution->clear();
        ui->stackedWidget->setCurrentIndex(2);
        idUser = BDD.verificationAdherent(tag_RFID);
        qDebug() << QString::number(idUser);
        QJsonArray emplacementMateriel = BDD.demanderMaterielEmprunter(idUser);
        QJsonObject materiel = emplacementMateriel.at(0).toObject();
        ui->label_ObjetRestitution->setText("Voulez-vous restituer l'objet : " + materiel["nom"].toString());
        ui->label_ObjetRestitution->adjustSize(); // Ajustement de la taille du label
        idMateriel = materiel["id_materiel"].toInt();

        QDateTime date_emprunt = QDateTime::currentDateTime();
        QDateTime date_limite = QDateTime::fromString(materiel["date_limite"].toString(), Qt::ISODate);
        QString datelimiteforce = date_limite.toString("yyyy-MM-dd hh:mm:ss");
        qDebug() << materiel["nom"].toString() << datelimiteforce;
        qint64 differenceEnSecondes = date_limite.toSecsSinceEpoch() - date_emprunt.toSecsSinceEpoch();
        qDebug() << QString::number(differenceEnSecondes);

        // Calcul du nombre total d'heures et de jours
        int jours = differenceEnSecondes / (24 * 60 * 60);
        int heures = (differenceEnSecondes % (24 * 60 * 60)) / (60 * 60);
        int minutes = (differenceEnSecondes % (60 * 60)) / 60;

        QString texteDifference;
        if (jours > 0) {
            if (jours == 1) {
                texteDifference = QString("%1 jour").arg(jours);
            }
            else {
                texteDifference = QString("%1 jours").arg(jours);
            }
            if(heures > 0){
                texteDifference += QString(" %1 heure(s)").arg(heures);
            }
            if(minutes > 0){
                texteDifference += QString(" %1 minute(s)").arg(minutes);
            }
        }
        else
        {
            if(heures>0){
                texteDifference += QString("%1 heure(s)").arg(heures);
            }
            if(minutes>0){
                texteDifference += QString(" %1 minute(s)").arg(minutes);
            }
        }

        ui->label_TempsRestitution->setText("Il vous reste : " + texteDifference);
        ui->label_TempsRestitution->adjustSize(); // Ajustement de la taille du label

    }
}

void GestionnaireCasier::viderToolButton()
{
    QToolButton *toolbutton;
    for(int i = 1; i < 12 ;i++){
        toolbutton = findChild<QToolButton*>(QString("toolbutton_Choix_Casier%1").arg(i));
        toolbutton->setText("");
        toolbutton->setIcon(QIcon());
    }
    for(int i = 1; i < 12 ;i++){
        toolbutton = findChild<QToolButton*>(QString("toolbutton_Remplir_Casier%1").arg(i));
        toolbutton->setText("");
        toolbutton->setIcon(QIcon());
    }
    for(int i = 1; i < 12 ;i++){
        toolbutton = findChild<QToolButton*>(QString("toolbutton_Retirer_Casier%1").arg(i));
        toolbutton->setText("");
        toolbutton->setIcon(QIcon());
    }
}


void GestionnaireCasier::connectButtonsRedirection()
{
    connect(ui->pushButton_RedirectionRemplir, &QPushButton::clicked,this, &GestionnaireCasier::InitialisationRemplir);
    connect(ui->pushButton_RedirectionRetirer, &QPushButton::clicked,this, &GestionnaireCasier::InitialisationRetirer);
    connect(ui->pushButton_RedirectionAccueil, &QPushButton::clicked,this, &GestionnaireCasier::RedirectionAccueil);
    connect(ui->pushButton_RedirectionAccueil_2, &QPushButton::clicked,this, &GestionnaireCasier::RedirectionAccueil);
    connect(ui->pushButton_RedirectionAccueil_3, &QPushButton::clicked,this, &GestionnaireCasier::RedirectionAccueil);
    connect(ui->pushButton_RedirectionAccueil_4, &QPushButton::clicked,this, &GestionnaireCasier::RedirectionAccueil);
    connect(ui->pushButton_RedirectionAccueil_9, &QPushButton::clicked,this, &GestionnaireCasier::RedirectionAccueil);
    connect(ui->pushButton_RedirectionAccueil_5, &QPushButton::clicked,this, &GestionnaireCasier::RedirectionAccueil);
}

void GestionnaireCasier::RedirectionAccueil()
{
    QPushButton *pushButton = qobject_cast<QPushButton*>(sender());
    if(pushButton == ui->pushButton_RedirectionAccueil || pushButton == ui->pushButton_RedirectionAccueil_2 || pushButton == ui->pushButton_RedirectionAccueil_4 || pushButton == ui->pushButton_RedirectionAccueil_5){
        ui->stackedWidget->setCurrentIndex(0);
    }
    if(pushButton == ui->pushButton_RedirectionAccueil_9 || pushButton == ui->pushButton_RedirectionAccueil_3){
        ui->stackedWidget->setCurrentIndex(6);
    }
}

void GestionnaireCasier::Style()
{
    QString stylePath = ":/ressources/style.qss"; // Chemin vers le fichier QSS
    QFile styleFile(stylePath);
    if (styleFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream textStream(&styleFile);
        QString styleSheet = textStream.readAll();
        qApp->setStyleSheet(styleSheet);
        styleFile.close();
    }

    QPixmap image(":/images/MDL-removebg-preview.png");

    image = image.scaled(ui->label_Image1->size(), Qt::KeepAspectRatio);
    ui->label_Image1->setPixmap(image);
    image = image.scaled(ui->label_Image2->size(), Qt::KeepAspectRatio);
    ui->label_Image2->setPixmap(image);
    image = image.scaled(ui->label_Image3->size(), Qt::KeepAspectRatio);
    ui->label_Image3->setPixmap(image);
    image = image.scaled(ui->label_Image4->size(), Qt::KeepAspectRatio);
    ui->label_Image4->setPixmap(image);
    image = image.scaled(ui->label_Image5->size(), Qt::KeepAspectRatio);
    ui->label_Image5->setPixmap(image);
}

void GestionnaireCasier::downloadImage(QString link, QToolButton *toolButton)
{
    QUrl imageUrl("http://172.18.58.49/" + link);
    imageDownloader->downloadImage(imageUrl, toolButton);
}

void GestionnaireCasier::onImageDownloaded(const QPixmap &pixmap, QToolButton *toolButton)
{
    if (!pixmap.isNull() && toolButton) {
        QSize buttonSize = toolButton->size();
        buttonSize.setHeight(buttonSize.height() - 50);

        // Créer une nouvelle icône avec l'image téléchargée
        QPixmap scaledPixmap = pixmap.scaledToWidth(buttonSize.width());
        QIcon newIcon(scaledPixmap);

        // Définir l'icône sur le bouton
        toolButton->setIcon(newIcon);
        toolButton->setIconSize(buttonSize);

        // Définir le style pour afficher l'icône sous le texte
        toolButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    } else {
        qDebug() << "Failed to load image";
    }
}


void GestionnaireCasier::connectButtonsNumeroBadge()
{
    connect(ui->toolButton_Add0, &QToolButton::clicked, this, &GestionnaireCasier::AjoutChiffre);
    connect(ui->toolButton_Add1, &QToolButton::clicked, this, &GestionnaireCasier::AjoutChiffre);
    connect(ui->toolButton_Add2, &QToolButton::clicked, this, &GestionnaireCasier::AjoutChiffre);
    connect(ui->toolButton_Add3, &QToolButton::clicked, this, &GestionnaireCasier::AjoutChiffre);
    connect(ui->toolButton_Add4, &QToolButton::clicked, this, &GestionnaireCasier::AjoutChiffre);
    connect(ui->toolButton_Add5, &QToolButton::clicked, this, &GestionnaireCasier::AjoutChiffre);
    connect(ui->toolButton_Add6, &QToolButton::clicked, this, &GestionnaireCasier::AjoutChiffre);
    connect(ui->toolButton_Add7, &QToolButton::clicked, this, &GestionnaireCasier::AjoutChiffre);
    connect(ui->toolButton_Add8, &QToolButton::clicked, this, &GestionnaireCasier::AjoutChiffre);
    connect(ui->toolButton_Add9, &QToolButton::clicked, this, &GestionnaireCasier::AjoutChiffre);
    connect(ui->toolButton_Valider, &QToolButton::clicked, this, &GestionnaireCasier::AjoutChiffre);
    connect(ui->toolButton_Supprimer, &QToolButton::clicked, this, &GestionnaireCasier::AjoutChiffre);
}


void GestionnaireCasier::AjoutChiffre()
{
    currentButtonNumeroBadge = qobject_cast<QToolButton*>(sender());
    if(currentButtonNumeroBadge->text() == "←" || currentButtonNumeroBadge->text() == "☑"){
        if(currentButtonNumeroBadge->text() == "←"){
            ui->lineEdit_NumeroBadge->backspace();
        }
        if(currentButtonNumeroBadge->text() == "☑"){
            QString date_naissance = ui->lineEdit_Jour->text()+'/'+ui->lineEdit_Mois->text()+'/'+ui->lineEdit_Annee->text();
            qDebug() << date_naissance;
            int id = BDD.recherchePremiereFois(date_naissance, ui->lineEdit_NumeroBadge->text().toInt());
            qDebug() << QString::number(id);
            qDebug() << tagRFID;
            BDD.enregistrementAdherent(tagRFID,id);
            ui->stackedWidget->setCurrentIndex(0);
            ui->lineEdit_NumeroBadge->clear();
        }
    }
    else
    {
        ui->lineEdit_NumeroBadge->setText(ui->lineEdit_NumeroBadge->text() + currentButtonNumeroBadge->text());
    }
}

void GestionnaireCasier::connectButtonsDateDeNaissance()
{
    connect(ui->toolButton_AnneeMoins, &QPushButton::pressed, this, &GestionnaireCasier::on_ToolButtonPressEvent);
    connect(ui->toolButton_AnneeMoins, &QPushButton::released, this, &GestionnaireCasier::on_ToolButtonReleaseEvent);
    connect(ui->toolButton_AnneeMoins, &QPushButton::clicked, this, &GestionnaireCasier::on_TimerPressEventEnd);

    connect(ui->toolButton_AnneePlus, &QPushButton::pressed, this, &GestionnaireCasier::on_ToolButtonPressEvent);
    connect(ui->toolButton_AnneePlus, &QPushButton::released, this, &GestionnaireCasier::on_ToolButtonReleaseEvent);
    connect(ui->toolButton_AnneePlus, &QPushButton::clicked, this, &GestionnaireCasier::on_TimerPressEventEnd);

    connect(ui->toolButton_MoisMoins, &QPushButton::pressed, this, &GestionnaireCasier::on_ToolButtonPressEvent);
    connect(ui->toolButton_MoisMoins, &QPushButton::released, this, &GestionnaireCasier::on_ToolButtonReleaseEvent);
    connect(ui->toolButton_MoisMoins, &QPushButton::clicked, this, &GestionnaireCasier::on_TimerPressEventEnd);

    connect(ui->toolButton_MoisPlus, &QPushButton::pressed, this, &GestionnaireCasier::on_ToolButtonPressEvent);
    connect(ui->toolButton_MoisPlus, &QPushButton::released, this, &GestionnaireCasier::on_ToolButtonReleaseEvent);
    connect(ui->toolButton_MoisPlus, &QPushButton::clicked, this, &GestionnaireCasier::on_TimerPressEventEnd);

    connect(ui->toolButton_JourMoins, &QPushButton::pressed, this, &GestionnaireCasier::on_ToolButtonPressEvent);
    connect(ui->toolButton_JourMoins, &QPushButton::released, this, &GestionnaireCasier::on_ToolButtonReleaseEvent);
    connect(ui->toolButton_JourMoins, &QPushButton::clicked, this, &GestionnaireCasier::on_TimerPressEventEnd);

    connect(ui->toolButton_JourPlus, &QPushButton::pressed, this, &GestionnaireCasier::on_ToolButtonPressEvent);
    connect(ui->toolButton_JourPlus, &QPushButton::released, this, &GestionnaireCasier::on_ToolButtonReleaseEvent);
    connect(ui->toolButton_JourPlus, &QPushButton::clicked, this, &GestionnaireCasier::on_TimerPressEventEnd);
}

void GestionnaireCasier::on_ToolButtonPressEvent()
{
    currentButtonDateNaissance = qobject_cast<QToolButton*>(sender());
    pressEvent = new QTimer(this);
    connect(pressEvent, &QTimer::timeout, this, &GestionnaireCasier::on_TimerPressEventEnd);
    pressEvent->start(150);
}

void GestionnaireCasier::on_ToolButtonReleaseEvent()
{
    pressEvent->stop();
    delete pressEvent;
    pressEvent = nullptr;
}

void GestionnaireCasier::on_TimerPressEventEnd()
{
    if (currentButtonDateNaissance == ui->toolButton_AnneeMoins){
        if(ui->lineEdit_Annee->text().toInt() > 1920)
            ui->lineEdit_Annee->setText(QString::number(ui->lineEdit_Annee->text().toInt() - 1));
    }
    if (currentButtonDateNaissance == ui->toolButton_AnneePlus){
        if(ui->lineEdit_Annee->text().toInt() < 2024)
            ui->lineEdit_Annee->setText(QString::number(ui->lineEdit_Annee->text().toInt() + 1));
    }
    if (currentButtonDateNaissance == ui->toolButton_MoisMoins){
        if(ui->lineEdit_Mois->text().toInt() > 0)
            ui->lineEdit_Mois->setText(QString::number(ui->lineEdit_Mois->text().toInt() - 1));
    }
    if (currentButtonDateNaissance == ui->toolButton_MoisPlus){
        if(ui->lineEdit_Mois->text().toInt() < 12)
            ui->lineEdit_Mois->setText(QString::number(ui->lineEdit_Mois->text().toInt() + 1));
    }
    if (currentButtonDateNaissance == ui->toolButton_JourMoins){
        if(ui->lineEdit_Jour->text().toInt() > 0)
            ui->lineEdit_Jour->setText(QString::number(ui->lineEdit_Jour->text().toInt() - 1));
    }
    if (currentButtonDateNaissance == ui->toolButton_JourPlus) {
        int mois = ui->lineEdit_Mois->text().toInt();
        int annee = ui->lineEdit_Annee->text().toInt();
        int jour = ui->lineEdit_Jour->text().toInt();

        bool estAnneeBissextile = (annee % 4 == 0 && annee % 100 != 0) || (annee % 400 == 0);
        int joursDansMois = 0;

        switch (mois) {
        case 2: // Février
            joursDansMois = estAnneeBissextile ? 29 : 28;
            break;
        case 4: case 6: case 9: case 11: // Avril, Juin, Septembre, Novembre
            joursDansMois = 30;
            break;
        default:
            joursDansMois = 31;
            break;
        }

        if (jour < joursDansMois) {
            ui->lineEdit_Jour->setText(QString::number(jour + 1));
            if(ui->lineEdit_Jour->text().length() == 1){
                ui->lineEdit_Jour->setText("0" + ui->lineEdit_Jour->text());
            }
        }
    }
    checkAndUpdateDays();
}

void GestionnaireCasier::checkAndUpdateDays() {
    int mois = ui->lineEdit_Mois->text().toInt();
    int annee = ui->lineEdit_Annee->text().toInt();
    int jour = ui->lineEdit_Jour->text().toInt();

    int joursDansMois = 0;

    switch (mois) {
    case 2: // Février
        joursDansMois = (annee % 4 == 0 && annee % 100 != 0) || (annee % 400 == 0) ? 29 : 28;
        break;
    case 4: case 6: case 9: case 11: // Avril, Juin, Septembre, Novembre
        joursDansMois = 30;
        break;
    default:
        joursDansMois = 31;
        break;
    }

    // Vérifie si le jour actuel est valide pour le mois
    if (jour > joursDansMois) {
        // Si le jour actuel dépasse le nombre de jours dans le mois, le réinitialiser au dernier jour valide
        ui->lineEdit_Jour->setText(QString::number(joursDansMois));
    }
    if(ui->lineEdit_Mois->text().length() == 1){
        ui->lineEdit_Mois->setText("0" + ui->lineEdit_Mois->text());
    }
}

void GestionnaireCasier::InitialisationRemplir()
{
    ui->stackedWidget->setCurrentIndex(3);
    QJsonArray emplacementMaterielRemplir = BDD.recupererMaterielEnStock();
    QWidget *widget = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout(widget);
    for (int i = 0; i < emplacementMaterielRemplir.size(); ++i) {
        QJsonObject materiel = emplacementMaterielRemplir.at(i).toObject();
        QString nomBouton = materiel["nom"].toString();
        int idBouton = materiel["id"].toInt();

        QPushButton *button = new QPushButton(nomBouton);
        button->setProperty("id", idBouton);
        layout->addWidget(button);

        connect(button, &QPushButton::clicked, this, &GestionnaireCasier::ChoixRemplissage);
    }
    ui->scrollArea->setWidget(widget);

    QJsonArray emplacementMateriel = BDD.recupererMaterielDisponible();
    for (int i = 0; i < emplacementMateriel.size(); ++i) {
        QJsonObject materiel = emplacementMateriel.at(i).toObject();
        QString nomBouton = materiel["nom"].toString();
        QString etatBouton = materiel["etat"].toString();
        int idCasierBouton = materiel["id_casier"].toInt();
        QString nomImage = materiel["image"].toString();

        // Assurez-vous que votre UI contient des objets QToolButton avec les noms correspondants
        QToolButton *toolButton = findChild<QToolButton*>(QString("toolbutton_Remplir_Casier%1").arg(idCasierBouton));
        if (toolButton) {
            // Inclure le nom et l'état dans le setText()
            QString buttonText = nomBouton + "\n" + etatBouton;
            toolButton->setText(buttonText);
            toolButton->setProperty("id", materiel["id"].toInt());
            toolButton->setProperty("id_casier", materiel["id_casier"].toInt());
            downloadImage(nomImage, toolButton);

            // Appliquer la couleur en fonction de l'état
            if (etatBouton == "Indisponible") {
                toolButton->setStyleSheet("color: red;background-color:orange;");
            } else if (etatBouton == "Disponible") {
                toolButton->setStyleSheet("color: green;background-color:orange;");
            }
        }
    }
    for(int i = 1; i <= 11; i++){
        QToolButton *toolButton = findChild<QToolButton*>(QString("toolbutton_Remplir_Casier%1").arg(i));
        toolButton->setProperty("id_casier", i);
    }
}


void GestionnaireCasier::connectButtonsRemplissage()
{
    connect(ui->toolbutton_Remplir_Casier1, &QToolButton::clicked, this, &GestionnaireCasier::ChoixRemplissage);
    connect(ui->toolbutton_Remplir_Casier2, &QToolButton::clicked, this, &GestionnaireCasier::ChoixRemplissage);
    connect(ui->toolbutton_Remplir_Casier3, &QToolButton::clicked, this, &GestionnaireCasier::ChoixRemplissage);
    connect(ui->toolbutton_Remplir_Casier4, &QToolButton::clicked, this, &GestionnaireCasier::ChoixRemplissage);
    connect(ui->toolbutton_Remplir_Casier5, &QToolButton::clicked, this, &GestionnaireCasier::ChoixRemplissage);
    connect(ui->toolbutton_Remplir_Casier6, &QToolButton::clicked, this, &GestionnaireCasier::ChoixRemplissage);
    connect(ui->toolbutton_Remplir_Casier7, &QToolButton::clicked, this, &GestionnaireCasier::ChoixRemplissage);
    connect(ui->toolbutton_Remplir_Casier8, &QToolButton::clicked, this, &GestionnaireCasier::ChoixRemplissage);
    connect(ui->toolbutton_Remplir_Casier9, &QToolButton::clicked, this, &GestionnaireCasier::ChoixRemplissage);
    connect(ui->toolbutton_Remplir_Casier10, &QToolButton::clicked, this, &GestionnaireCasier::ChoixRemplissage);
    connect(ui->toolbutton_Remplir_Casier11, &QToolButton::clicked, this, &GestionnaireCasier::ChoixRemplissage);
    connect(ui->pushButton_Remplir_Valider, &QPushButton::clicked, this, &GestionnaireCasier::ChoixRemplissage);
    connect(ui->pushButton_Remplir_Annuler, &QPushButton::clicked, this, &GestionnaireCasier::ChoixRemplissage);
}

void GestionnaireCasier::ChoixRemplissage()
{
    QPushButton *pushButton = qobject_cast<QPushButton*>(sender());
    QToolButton *toolButton = qobject_cast<QToolButton*>(sender());

    // Vérifier si le toolButton a déjà du texte
    if (toolButton && !toolButton->text().isEmpty()) {
        // Afficher un message si le QToolButton a déjà du texte
        QMessageBox::information(this, "Information", "Ce casier est déjà rempli.");
        return; // Quitter la fonction car le bouton est déjà rempli
    }

    // Si le sender est un QToolButton et si aucun bouton casier n'est sélectionné
    if (toolButton && !currentButtonRemplirCasier) {
        currentButtonRemplirCasier = toolButton;
        currentButtonRemplirCasier->setEnabled(false);
        currentButtonRemplirCasier->setStyleSheet("background-color: grey");
    }

    // Si le sender est un QPushButton et si aucun bouton matériel n'est sélectionné
    if (pushButton && !currentButtonRemplirMateriel && pushButton->text() != "Valider" && pushButton->text() != "Annuler") {
        currentButtonRemplirMateriel = pushButton;
        currentButtonRemplirMateriel->setEnabled(false);
        currentButtonRemplirMateriel->setStyleSheet("background-color: grey");
    }

    // Si le sender est un QPushButton "Valider" ou "Annuler", réactiver le casier et le matériel
    if (pushButton && (pushButton->text() == "Valider" || pushButton->text() == "Annuler")) {
        if (currentButtonRemplirCasier && currentButtonRemplirMateriel) {
            // Exécutez votre action associée à "Valider
            // Par exemple, vous pouvez appeler une fonction pour traiter les boutons sélectionnés
            BDD.ajouterObjet(currentButtonRemplirCasier->property("id_casier").toInt(), currentButtonRemplirMateriel->property("id").toInt());
            accessGache->ouvertureGache(currentButtonRemplirCasier->property("id_casier").toInt());
            // Réinitialiser les boutons après avoir traité l'action
            currentButtonRemplirCasier->setEnabled(true);
            currentButtonRemplirMateriel->setEnabled(true);
            currentButtonRemplirCasier->setStyleSheet("background-color: orange");
            currentButtonRemplirMateriel->setStyleSheet("background-color: orange");
            currentButtonRemplirCasier = nullptr;
            currentButtonRemplirMateriel = nullptr;
            InitialisationRemplir();
        } else {
            // Affichez un message si un seul bouton est sélectionné ou aucun bouton n'est sélectionné
            QMessageBox::information(this, "Information", "Veuillez sélectionner un bouton casier et un bouton matériel.");
        }
    }
}


void GestionnaireCasier::InitialisationRetirer()
{
    ui->stackedWidget->setCurrentIndex(4);
    QJsonArray emplacementMateriel = BDD.recupererMaterielDisponible();
    for (int i = 0; i < emplacementMateriel.size(); ++i) {
        QJsonObject materiel = emplacementMateriel.at(i).toObject();
        QString nomBouton = materiel["nom"].toString();
        QString etatBouton = materiel["etat"].toString();
        int idCasierBouton = materiel["id_casier"].toInt();
        QString nomImage = materiel["image"].toString();

        // Assurez-vous que votre UI contient des objets QToolButton avec les noms correspondants
        QToolButton *toolButton = findChild<QToolButton*>(QString("toolbutton_Retirer_Casier%1").arg(idCasierBouton));
        if (toolButton) {
            // Inclure le nom et l'état dans le setText()
            QString buttonText = nomBouton + "\n" + etatBouton;
            toolButton->setText(buttonText);
            toolButton->setProperty("id", materiel["id"].toInt());
            toolButton->setProperty("id_casier", materiel["id_casier"].toInt());
            downloadImage(nomImage, toolButton);

            // Appliquer la couleur en fonction de l'état
            if (etatBouton == "Indisponible") {
                toolButton->setStyleSheet("color: red;background-color:orange;");
            } else if (etatBouton == "Disponible") {
                toolButton->setStyleSheet("color: green;background-color:orange;");
            }
        }
    }
}

void GestionnaireCasier::connectButtonsRetirer()
{
    connect(ui->toolbutton_Retirer_Casier1, &QToolButton::clicked, this, &GestionnaireCasier::ChoixRetirer);
    connect(ui->toolbutton_Retirer_Casier2, &QToolButton::clicked, this, &GestionnaireCasier::ChoixRetirer);
    connect(ui->toolbutton_Retirer_Casier3, &QToolButton::clicked, this, &GestionnaireCasier::ChoixRetirer);
    connect(ui->toolbutton_Retirer_Casier4, &QToolButton::clicked, this, &GestionnaireCasier::ChoixRetirer);
    connect(ui->toolbutton_Retirer_Casier5, &QToolButton::clicked, this, &GestionnaireCasier::ChoixRetirer);
    connect(ui->toolbutton_Retirer_Casier6, &QToolButton::clicked, this, &GestionnaireCasier::ChoixRetirer);
    connect(ui->toolbutton_Retirer_Casier7, &QToolButton::clicked, this, &GestionnaireCasier::ChoixRetirer);
    connect(ui->toolbutton_Retirer_Casier8, &QToolButton::clicked, this, &GestionnaireCasier::ChoixRetirer);
    connect(ui->toolbutton_Retirer_Casier9, &QToolButton::clicked, this, &GestionnaireCasier::ChoixRetirer);
    connect(ui->toolbutton_Retirer_Casier10, &QToolButton::clicked, this, &GestionnaireCasier::ChoixRetirer);
    connect(ui->toolbutton_Retirer_Casier11, &QToolButton::clicked, this, &GestionnaireCasier::ChoixRetirer);
}

void GestionnaireCasier::ChoixRetirer()
{
    currentButtonRetirerObjet = qobject_cast<QToolButton*>(sender());
    QString toolButtonText = currentButtonRetirerObjet->text();
    if(toolButtonText.isEmpty() || toolButtonText.contains("Indisponible")){
        if (toolButtonText.isEmpty()) {
            QMessageBox::information(this, "Information", "Veuillez sélectionner un matériel valide");
        }
        if (toolButtonText.contains("Indisponible")) {
            QMessageBox::information(this, "Information", "Ce matériel est en cours d'emprunt");
        }
    }
    else
    {
        QMessageBox msgBox(QMessageBox::Question, "Confirmation", "Etes-vous sûr de vouloir retirer " + toolButtonText + " du casier ?", QMessageBox::No | QMessageBox::Yes,this);
        QAbstractButton *yesButton = msgBox.button(QMessageBox::Yes);
        yesButton->setText("Valider");
        connect(yesButton, &QAbstractButton::clicked, this, &GestionnaireCasier::RedirectionRetirer);
        QAbstractButton *noButton = msgBox.button(QMessageBox::No);
        noButton->setText("Annuler");

        msgBox.exec();
    }
}

void GestionnaireCasier::RedirectionRetirer()
{
    BDD.retirerObjet(currentButtonRetirerObjet->property("id").toInt());
    viderToolButton();
    InitialisationRetirer();
    accessGache->ouvertureGache(currentButtonRetirerObjet->property("id_casier").toInt());
}

void GestionnaireCasier::connectButtonsEmprunt()
{
    connect(ui->toolbutton_Choix_Casier1, &QToolButton::clicked, this, &GestionnaireCasier::ChoixEmprunt);
    connect(ui->toolbutton_Choix_Casier2, &QToolButton::clicked, this, &GestionnaireCasier::ChoixEmprunt);
    connect(ui->toolbutton_Choix_Casier3, &QToolButton::clicked, this, &GestionnaireCasier::ChoixEmprunt);
    connect(ui->toolbutton_Choix_Casier4, &QToolButton::clicked, this, &GestionnaireCasier::ChoixEmprunt);
    connect(ui->toolbutton_Choix_Casier5, &QToolButton::clicked, this, &GestionnaireCasier::ChoixEmprunt);
    connect(ui->toolbutton_Choix_Casier6, &QToolButton::clicked, this, &GestionnaireCasier::ChoixEmprunt);
    connect(ui->toolbutton_Choix_Casier7, &QToolButton::clicked, this, &GestionnaireCasier::ChoixEmprunt);
    connect(ui->toolbutton_Choix_Casier8, &QToolButton::clicked, this, &GestionnaireCasier::ChoixEmprunt);
    connect(ui->toolbutton_Choix_Casier11, &QToolButton::clicked, this, &GestionnaireCasier::ChoixEmprunt);
    connect(ui->toolbutton_Choix_Casier10, &QToolButton::clicked, this, &GestionnaireCasier::ChoixEmprunt);
    connect(ui->toolbutton_Choix_Casier9, &QToolButton::clicked, this, &GestionnaireCasier::ChoixEmprunt);
}

void GestionnaireCasier::ChoixEmprunt()
{
    currentButtonChoixEmprunt = qobject_cast<QToolButton*>(sender());
    QString toolButtonText = currentButtonChoixEmprunt->text();

    if (toolButtonText.isEmpty() || toolButtonText.contains("Indisponible")) {
        if (toolButtonText.contains("Indisponible")) {
            QMessageBox::information(this, "Information", "Ce matériel est déjà emprunté");
        }
        if (toolButtonText.isEmpty()) {
            QMessageBox::information(this, "Information", "Veuillez choisir un materiel disponible !");
        }
    } else {
        QMessageBox msgBox(QMessageBox::Question, "Confirmation", "Etes-vous sûr de vouloir emprunter " + 
                            toolButtonText + " ?", QMessageBox::No | QMessageBox::Yes,this);
        QAbstractButton *yesButton = msgBox.button(QMessageBox::Yes);
        yesButton->setText("Valider");
        connect(yesButton, &QAbstractButton::clicked, this, &GestionnaireCasier::RedirectionEmprunt);
        QAbstractButton *noButton = msgBox.button(QMessageBox::No);
        noButton->setText("Annuler");

        msgBox.exec();
    }
}

void GestionnaireCasier::RedirectionEmprunt()
{
    accessGache->ouvertureGache(currentButtonChoixEmprunt->property("id_casier").toInt());
    QDateTime currentDateTime = QDateTime::currentDateTime();
    QString DateEmprunt = currentDateTime.toString("yyyy-MM-dd hh:mm:ss");

    // Récupérer le nombre d'heures à ajouter
    int nbHeure = BDD.recupererDureeEmprunt(currentButtonChoixEmprunt->property("id").toInt());

    // Ajouter le nombre d'heures à la date actuelle
    QDateTime dateLimite = currentDateTime.addSecs(nbHeure * 3600); // Convertir les heures en secondes

    // Formater la date limite dans le format requis
    QString DateLimiteString = dateLimite.toString("yyyy-MM-dd hh:mm:ss");
    qDebug() << DateLimiteString + "|" + DateEmprunt;
    // Lier le matériel avec les dates calculées
    BDD.attribuerMateriel(DateEmprunt, DateLimiteString, idUser, currentButtonChoixEmprunt->property("id").toInt());
    BDD.changementIndisponibilite(currentButtonChoixEmprunt->property("id").toInt());
    ui->stackedWidget->setCurrentIndex(0);
}

void GestionnaireCasier::connectButtonsRestitution()
{
    connect(ui->toolButton_RestitutionOui, &QToolButton::clicked, this, &GestionnaireCasier::RedirectionRestitution);
    connect(ui->toolButton_RestitutionNon, &QToolButton::clicked, this, &GestionnaireCasier::RedirectionRestitution);
}

void GestionnaireCasier::RedirectionRestitution()
{
    currentButtonRestitution = qobject_cast<QToolButton*>(sender());
    if(currentButtonRestitution == ui->toolButton_RestitutionNon){
        ui->stackedWidget->setCurrentIndex(0);
    }
    if(currentButtonRestitution == ui->toolButton_RestitutionOui)
    {
        accessGache->ouvertureGache(idMateriel);
        QDateTime currentDateTime = QDateTime::currentDateTime();
        QString DateRetour= currentDateTime.toString("yyyy-MM-dd hh:mm:ss");
        qDebug() << DateRetour;
        BDD.changementDisponiblite(BDD.recupererIdMaterielEmprunter(idUser));
        BDD.ajoutDateRetour(BDD.recupererIdEmprunts(idUser), DateRetour);
        ui->stackedWidget->setCurrentIndex(0);
    }
}
