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
#include <QString>
#include <QMessageBox>
#include "imagedownloader.h"
#include "accessbdd.h"
#include "wiegand.h"
#include "accesgache.h"

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
// Indentification
    void on_NouvelleTrame(QString &tag_RFID);
    void viderToolButton();
    void connectButtonsRedirection();
    void RedirectionAccueil();
//Styles
    void Style();
    void downloadImage(QString link, QToolButton *toolButton);
    void onImageDownloaded(const QPixmap &pixmap, QToolButton *toolButton);
//Premiere Identification
    //Slots Calculatrice
    void connectButtonsNumeroBadge();
    void AjoutChiffre();

    //Slots Date De Naissance
    void connectButtonsDateDeNaissance();
    void on_ToolButtonPressEvent();
    void on_ToolButtonReleaseEvent();
    void on_TimerPressEventEnd();
    void checkAndUpdateDays();
//Remplir
    void InitialisationRemplir();
    void connectButtonsRemplissage();
    void ChoixRemplissage();
//Retirer
    void InitialisationRetirer();
    void connectButtonsRetirer();
    void ChoixRetirer();
    void RedirectionRetirer();
//Emprunt
    void connectButtonsEmprunt();
    void ChoixEmprunt();
    void RedirectionEmprunt();
//Restitution
    void connectButtonsRestitution();
    void RedirectionRestitution();


private:
    QTimer *pressEvent;
    QTimer timerIDCarte;
    int idUser;
    int idMateriel;
    int idCasier;
    AccesGache *accessGache;
    ImageDownloader *imageDownloader;
    accessBDD BDD;
    Wiegand lecteur;
    QString tagRFID;
    QToolButton *currentButtonRetirerObjet;
    QToolButton *currentButtonChoixEmprunt;
    QToolButton *currentButtonRestitution;
    QToolButton *currentButtonNumeroBadge;
    QToolButton *currentButtonDateNaissance;
    QToolButton *currentButtonRemplirCasier = NULL;
    QPushButton *currentButtonRemplirMateriel = NULL;
    Ui::GestionnaireCasier *ui;
};
#endif // GESTIONNAIRECASIER_H
