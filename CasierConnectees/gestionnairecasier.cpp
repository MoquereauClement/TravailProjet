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
    connectButtonsPerso();
    connectButtonsDateDeNaissance();
    connectButtonsNumeroBadge();
    connectButtonsRemplissage();
 }


GestionnaireCasier::~GestionnaireCasier()
{
    delete ui;
}


void GestionnaireCasier::connectButtonsPerso()
{
    connect(ui->pushButton_RedirectionEmprunt, &QPushButton::clicked,this, &GestionnaireCasier::RedirectPerso);
    connect(ui->pushButton_RedirectionFirstTime, &QPushButton::clicked,this, &GestionnaireCasier::RedirectPerso);
    connect(ui->pushButton_RedirectionRemplir, &QPushButton::clicked,this, &GestionnaireCasier::RedirectPerso);
    connect(ui->pushButton_RedirectionRestitution, &QPushButton::clicked,this, &GestionnaireCasier::RedirectPerso);
    connect(ui->pushButton_RedirectionAccueil, &QPushButton::clicked,this, &GestionnaireCasier::RedirectPerso);
    connect(ui->pushButton_RedirectionAccueil_2, &QPushButton::clicked,this, &GestionnaireCasier::RedirectPerso);
    connect(ui->pushButton_RedirectionAccueil_3, &QPushButton::clicked,this, &GestionnaireCasier::RedirectPerso);
    connect(ui->pushButton_RedirectionAccueil_4, &QPushButton::clicked,this, &GestionnaireCasier::RedirectPerso);
}

void GestionnaireCasier::RedirectPerso()
{
    QPushButton *pushButton = qobject_cast<QPushButton*>(sender());
    if(pushButton == ui->pushButton_RedirectionEmprunt){
        ui->stackedWidget->setCurrentIndex(1);
        QJsonArray emplacementMateriel = BDD.emplacementMaterielEmprunter();
        for (int i = 0; i < emplacementMateriel.size(); ++i) {
            QJsonObject materiel = emplacementMateriel.at(i).toObject();
            QString nomBouton = materiel["nom"].toString();
            QString idBouton = materiel["id"].toString();

            // Assurez-vous que votre UI contient des objets QToolButton avec les noms correspondants
            QToolButton *toolButton = findChild<QToolButton*>(QString("toolbutton_Choix_Casier%1").arg(i + 1));
            if (toolButton) {
                toolButton->setText(nomBouton);
                toolButton->setProperty("id", idBouton);
            }
        }
    }
    if(pushButton == ui->pushButton_RedirectionFirstTime){
        ui->stackedWidget->setCurrentIndex(4);
    }
    if(pushButton == ui->pushButton_RedirectionRemplir){
        ui->stackedWidget->setCurrentIndex(3);
        QJsonArray emplacementMateriel = BDD.emplacementMaterielRemplir();
        QWidget *widget = new QWidget;
        QVBoxLayout *layout = new QVBoxLayout(widget);
        for (int i = 0; i < emplacementMateriel.size(); ++i) {
            QJsonObject materiel = emplacementMateriel.at(i).toObject();
            QString nomBouton = materiel["nom"].toString();
            QString idBouton = materiel["id"].toString();

            QPushButton *button = new QPushButton(nomBouton);
            button->setProperty("id", idBouton);
            layout->addWidget(button);

            connect(button, &QPushButton::clicked, this, &GestionnaireCasier::ChoixRemplissage);
        }
        ui->scrollArea->setWidget(widget);
    }
    if(pushButton == ui->pushButton_RedirectionRestitution){
        ui->stackedWidget->setCurrentIndex(2);
    }
    if(pushButton == ui->pushButton_RedirectionAccueil || pushButton == ui->pushButton_RedirectionAccueil_2 || pushButton == ui->pushButton_RedirectionAccueil_3 || pushButton == ui->pushButton_RedirectionAccueil_4){
        ui->stackedWidget->setCurrentIndex(0);
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


void GestionnaireCasier::connectButtonsNumeroBadge()
{
    connect(ui->toolButton_Add0, &QToolButton::clicked, this, &GestionnaireCasier::AddNumber);
    connect(ui->toolButton_Add1, &QToolButton::clicked, this, &GestionnaireCasier::AddNumber);
    connect(ui->toolButton_Add2, &QToolButton::clicked, this, &GestionnaireCasier::AddNumber);
    connect(ui->toolButton_Add3, &QToolButton::clicked, this, &GestionnaireCasier::AddNumber);
    connect(ui->toolButton_Add4, &QToolButton::clicked, this, &GestionnaireCasier::AddNumber);
    connect(ui->toolButton_Add5, &QToolButton::clicked, this, &GestionnaireCasier::AddNumber);
    connect(ui->toolButton_Add6, &QToolButton::clicked, this, &GestionnaireCasier::AddNumber);
    connect(ui->toolButton_Add7, &QToolButton::clicked, this, &GestionnaireCasier::AddNumber);
    connect(ui->toolButton_Add8, &QToolButton::clicked, this, &GestionnaireCasier::AddNumber);
    connect(ui->toolButton_Add9, &QToolButton::clicked, this, &GestionnaireCasier::AddNumber);
    connect(ui->toolButton_Valider, &QToolButton::clicked, this, &GestionnaireCasier::AddNumber);
    connect(ui->toolButton_Supprimer, &QToolButton::clicked, this, &GestionnaireCasier::AddNumber);
}


void GestionnaireCasier::AddNumber()
{
    currentButtonNumeroBadge = qobject_cast<QToolButton*>(sender());
    if(currentButtonNumeroBadge->text() == "←"){
        ui->lineEdit_NumeroBadge->backspace();
    }
    if(currentButtonNumeroBadge->text() == "☑"){
        QString id = BDD.rechercheFirstTime(ui->lineEdit_Jour->text()+'/'+ui->lineEdit_Mois->text()+'/'+ui->lineEdit_Annee->text(),ui->lineEdit_NumeroBadge->text());
        BDD.enregistrementAdherent("123456",id);
        ui->stackedWidget->setCurrentIndex(0);
    }
     else
    {
        ui->lineEdit_NumeroBadge->setText(ui->lineEdit_NumeroBadge->text() + currentButtonNumeroBadge->text());
    }

}

void GestionnaireCasier::connectButtonsDateDeNaissance()
{
    connect(ui->toolButton_AnneeMoins, &QPushButton::pressed, this, &GestionnaireCasier::RedirectPressEvent);
    connect(ui->toolButton_AnneeMoins, &QPushButton::released, this, &GestionnaireCasier::RedirectReleaseEvent);
    connect(ui->toolButton_AnneeMoins, &QPushButton::clicked, this, &GestionnaireCasier::on_TimerPressEventEnd);

    connect(ui->toolButton_AnneePlus, &QPushButton::pressed, this, &GestionnaireCasier::RedirectPressEvent);
    connect(ui->toolButton_AnneePlus, &QPushButton::released, this, &GestionnaireCasier::RedirectReleaseEvent);
    connect(ui->toolButton_AnneePlus, &QPushButton::clicked, this, &GestionnaireCasier::on_TimerPressEventEnd);

    connect(ui->toolButton_MoisMoins, &QPushButton::pressed, this, &GestionnaireCasier::RedirectPressEvent);
    connect(ui->toolButton_MoisMoins, &QPushButton::released, this, &GestionnaireCasier::RedirectReleaseEvent);
    connect(ui->toolButton_MoisMoins, &QPushButton::clicked, this, &GestionnaireCasier::on_TimerPressEventEnd);

    connect(ui->toolButton_MoisPlus, &QPushButton::pressed, this, &GestionnaireCasier::RedirectPressEvent);
    connect(ui->toolButton_MoisPlus, &QPushButton::released, this, &GestionnaireCasier::RedirectReleaseEvent);
    connect(ui->toolButton_MoisPlus, &QPushButton::clicked, this, &GestionnaireCasier::on_TimerPressEventEnd);

    connect(ui->toolButton_JourMoins, &QPushButton::pressed, this, &GestionnaireCasier::RedirectPressEvent);
    connect(ui->toolButton_JourMoins, &QPushButton::released, this, &GestionnaireCasier::RedirectReleaseEvent);
    connect(ui->toolButton_JourMoins, &QPushButton::clicked, this, &GestionnaireCasier::on_TimerPressEventEnd);

    connect(ui->toolButton_JourPlus, &QPushButton::pressed, this, &GestionnaireCasier::RedirectPressEvent);
    connect(ui->toolButton_JourPlus, &QPushButton::released, this, &GestionnaireCasier::RedirectReleaseEvent);
    connect(ui->toolButton_JourPlus, &QPushButton::clicked, this, &GestionnaireCasier::on_TimerPressEventEnd);
}

void GestionnaireCasier::RedirectPressEvent()
{
    currentButtonDateNaissance = qobject_cast<QToolButton*>(sender());
    pressEvent = new QTimer(this);
    connect(pressEvent, &QTimer::timeout, this, &GestionnaireCasier::on_TimerPressEventEnd);
    pressEvent->start(150);
}

void GestionnaireCasier::RedirectReleaseEvent()
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
        if (!currentButtonRemplirCasier || !currentButtonRemplirMateriel) {
            // Afficher un message si un seul bouton est sélectionné ou aucun bouton n'est sélectionné
            if (pushButton->text() == "Valider") {
                if(currentButtonRemplirCasier){
                    currentButtonRemplirCasier->setEnabled(true);
                    currentButtonRemplirCasier->setStyleSheet("background-color: orange");
                    currentButtonRemplirCasier = NULL;
                }
                if(currentButtonRemplirMateriel){
                    currentButtonRemplirMateriel->setEnabled(true);
                    currentButtonRemplirMateriel->setStyleSheet("background-color: orange");
                    currentButtonRemplirMateriel = NULL;
                }
                QMessageBox::information(this, "Information", "Veuillez sélectionner un bouton casier et un bouton matériel.");
            }
            if (pushButton->text() == "Annuler") {
                if(currentButtonRemplirCasier){
                    currentButtonRemplirCasier->setEnabled(true);
                    currentButtonRemplirCasier->setStyleSheet("background-color: orange");
                    currentButtonRemplirCasier = NULL;
                }
                if(currentButtonRemplirMateriel){
                    currentButtonRemplirMateriel->setEnabled(true);
                    currentButtonRemplirMateriel->setStyleSheet("background-color: orange");
                    currentButtonRemplirMateriel = NULL;
                }
            }
        } else {
            if (currentButtonRemplirCasier) {
                currentButtonRemplirCasier->setEnabled(true);
                currentButtonRemplirCasier->setStyleSheet("background-color: orange");
                currentButtonRemplirCasier = NULL;
            }
            if (currentButtonRemplirMateriel) {
                currentButtonRemplirMateriel->setEnabled(true);
                currentButtonRemplirMateriel->setStyleSheet("background-color: orange");
                currentButtonRemplirMateriel = NULL;
            }
        }
    }
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
    connect(ui->toolbutton_Choix_Casier9, &QToolButton::clicked, this, &GestionnaireCasier::ChoixEmprunt);
    connect(ui->toolbutton_Choix_Casier10, &QToolButton::clicked, this, &GestionnaireCasier::ChoixEmprunt);
    connect(ui->toolbutton_Choix_Casier11, &QToolButton::clicked, this, &GestionnaireCasier::ChoixEmprunt);
}

void GestionnaireCasier::ChoixEmprunt()
{
    currentButtonChoixEmprunt = qobject_cast<QToolButton*>(sender());
    if(currentButtonChoixEmprunt->text() == ""){
        QMessageBox::information(this, "Information", "Veuillez sélectionner un matériel valide !");
    }
    else
    {
        QMessageBox msgBox(QMessageBox::Question, "Confirmation", "Etes-vous sûr de vouloir emprunter "+currentButtonChoixEmprunt->text()+" ?", QMessageBox::No | QMessageBox::Yes,this);
        QAbstractButton *yesButton = msgBox.button(QMessageBox::Yes);
        yesButton->setText("Valider");

        QAbstractButton *noButton = msgBox.button(QMessageBox::No);
        noButton->setText("Annuler");
        msgBox.exec();
        connect(yesButton, &QAbstractButton::clicked,this,&GestionnaireCasier::RedirectEmprunt);
    }
}

void GestionnaireCasier::RedirectEmprunt()
{

}

void GestionnaireCasier::connectButtonsRestitution()
{
    connect(ui->toolButton_RestitutionOui, &QToolButton::clicked, this, &GestionnaireCasier::RedirectRestitution);
    connect(ui->toolButton_RestitutionNon, &QToolButton::clicked, this, &GestionnaireCasier::RedirectRestitution);
}

void GestionnaireCasier::RedirectRestitution()
{
    currentButtonRestitution = qobject_cast<QToolButton*>(sender());
    if(currentButtonRestitution == ui->toolButton_RestitutionNon){
        ui->stackedWidget->setCurrentIndex(0);
    }
    else
    {

    }
}
