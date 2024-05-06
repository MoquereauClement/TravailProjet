#ifndef WIEGAND_H
#define WIEGAND_H
#include <wiringPi.h>
#include <time.h>
#include <unistd.h>
#include <QObject>
#include <memory.h>
#include <QTimer>

class Wiegand : public QObject
{
    Q_OBJECT
public:

    Wiegand(int _d0pin = 0, int _d1pin = 1, unsigned int _wiegandbits = 48, unsigned int _packetGap = 15, QObject *parent = nullptr);
    void wiegandReset();
    int wiegandGetPendingBitCount();
    int wiegandReadData(void* data, int dataMaxLen);
     static void readD0();
     static void readD1();
     static unsigned char *wiegandData;
     static unsigned char wiegandBitCount;
     static struct timespec wiegandBitTime;
private:
    bool calculerChecksum();
    int d0pin;
    int d1pin;
     static unsigned int wiegandbits;
    unsigned int packetGap;
    QTimer timer;
    const long WIEGANDTIMEOUT = 3000000;
private slots:
    void on_timerTimeout();
signals:
    void NouvelleTrame(QString &tag_RFID);
};

#endif // WIEGAND_H
