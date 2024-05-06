#ifndef ACCESGACHE_H
#define ACCESGACHE_H

#define BUS_SORTIE 0x21
#define ALLUME 0
#define ETEINT 1
#define SORTIE 0
#define ENTREE 1
#define DUREE_IMPULSION_GACHE 200

#include <QObject>
#include <QTimer>
#include "ABE_IoPi.h"
using namespace ABElectronics_CPP_Libraries;

class AccesGache : public QObject
{
    Q_OBJECT
public:
    explicit AccesGache(QObject *parent = nullptr);
    void ouvertureGache(int _numGache);
    void onTimerTimeout();
    void fermetureComplete();

private:
    int numGache;
    IoPi controlBusSortie;
    QTimer tempo;
signals:

};

#endif // ACCESGACHE_H
