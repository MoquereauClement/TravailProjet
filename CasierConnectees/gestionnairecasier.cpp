#include "gestionnairecasier.h"
#include "ui_gestionnairecasier.h"

GestionnaireCasier::GestionnaireCasier(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::GestionnaireCasier)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(4);
    showFullScreen();
    connectButtonsDateDeNaissance();
    connectButtonsNumeroBadge();
    connectButtonsRemplissage();
    setButton();
}


GestionnaireCasier::~GestionnaireCasier()
{
    delete ui;
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
}

void GestionnaireCasier::on_toolButton_Valider_clicked()
{

}


void GestionnaireCasier::on_toolButton_Supprimer_clicked()
{
    ui->lineEdit_NumeroBadge->backspace();
}

void GestionnaireCasier::AddNumber()
{
    currentButtonNumeroBadge = qobject_cast<QToolButton*>(sender());
    ui->lineEdit_NumeroBadge->setText(ui->lineEdit_NumeroBadge->text() + currentButtonNumeroBadge->text());
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

void GestionnaireCasier::setButton()
{
    QWidget *widget = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout(widget);
    for(int i=0;i<50;i++){
        QPushButton *button = new QPushButton("Button 1");
        layout->addWidget(button);
        connect(button, &QPushButton::clicked, this, &GestionnaireCasier::ChoixRemplissage);
    }
    ui->scrollArea->setWidget(widget);
}

void GestionnaireCasier::connectButtonsRemplissage()
{
    connect(ui->toolbutton_Remplir_Casier1, &QPushButton::clicked, this, &GestionnaireCasier::ChoixRemplissage);
    connect(ui->toolbutton_Remplir_Casier2, &QPushButton::clicked, this, &GestionnaireCasier::ChoixRemplissage);
    connect(ui->toolbutton_Remplir_Casier3, &QPushButton::clicked, this, &GestionnaireCasier::ChoixRemplissage);
    connect(ui->toolbutton_Remplir_Casier4, &QPushButton::clicked, this, &GestionnaireCasier::ChoixRemplissage);
    connect(ui->toolbutton_Remplir_Casier5, &QPushButton::clicked, this, &GestionnaireCasier::ChoixRemplissage);
    connect(ui->toolbutton_Remplir_Casier6, &QPushButton::clicked, this, &GestionnaireCasier::ChoixRemplissage);
    connect(ui->toolbutton_Remplir_Casier7, &QPushButton::clicked, this, &GestionnaireCasier::ChoixRemplissage);
    connect(ui->toolbutton_Remplir_Casier8, &QPushButton::clicked, this, &GestionnaireCasier::ChoixRemplissage);
    connect(ui->toolbutton_Remplir_Casier9, &QPushButton::clicked, this, &GestionnaireCasier::ChoixRemplissage);
    connect(ui->toolbutton_Remplir_Casier10, &QPushButton::clicked, this, &GestionnaireCasier::ChoixRemplissage);
    connect(ui->toolbutton_Remplir_Casier11, &QPushButton::clicked, this, &GestionnaireCasier::ChoixRemplissage);
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
