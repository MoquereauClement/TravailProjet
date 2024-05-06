#include "accesgache.h"
#include <QDebug>

AccesGache::AccesGache(QObject *parent) : QObject(parent)
  , controlBusSortie(BUS_SORTIE,true)
{
    tempo.setSingleShot(true);
    connect(&tempo, &QTimer::timeout, this, &AccesGache::onTimerTimeout);
}

void AccesGache::ouvertureGache(int _numGache)
{
    numGache = _numGache;
    controlBusSortie.set_pin_direction(_numGache,SORTIE);
    controlBusSortie.write_pin(_numGache,ALLUME);
    tempo.start(DUREE_IMPULSION_GACHE);
}

void AccesGache::onTimerTimeout()
{
    controlBusSortie.write_pin(numGache,ETEINT);
}

void AccesGache::fermetureComplete()
{
    for(int i = 1; i < 16; i++){
        controlBusSortie.write_pin(i,ETEINT);
    }
}
