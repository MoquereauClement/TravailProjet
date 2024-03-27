#ifndef GESTIONNAIRECASIER_H
#define GESTIONNAIRECASIER_H


#include <QWidget>
#include <QAbstractButton>
#include <QAbstractItemView>
#include <QTimer>
#include <QToolButton>
#include <QPushButton>
#include <QMessageBox>

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
//Premiere Identification
    //Slots Calculatrice
    void connectButtonsNumeroBadge();
    void on_toolButton_Valider_clicked();
    void on_toolButton_Supprimer_clicked();
    void AddNumber();

    //Slots Date De Naissance
    void connectButtonsDateDeNaissance();
    void RedirectPressEvent();
    void RedirectReleaseEvent();
    void on_TimerPressEventEnd();
    void checkAndUpdateDays();
//Remplir
    void setButton();
    void connectButtonsRemplissage();
    void ChoixRemplissage();


private:
    QTimer *pressEvent;
    QToolButton *currentButtonNumeroBadge;
    QToolButton *currentButtonDateNaissance;
    QToolButton *currentButtonRemplirCasier = NULL;
    QPushButton *currentButtonRemplirMateriel = NULL;
    Ui::GestionnaireCasier *ui;
};
#endif // GESTIONNAIRECASIER_H
