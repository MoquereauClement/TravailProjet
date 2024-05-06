#include "wiegand.h"
unsigned char *Wiegand::wiegandData=nullptr;
unsigned char Wiegand::wiegandBitCount=0;
struct timespec Wiegand::wiegandBitTime={0,0};
unsigned int Wiegand::wiegandbits=0;

Wiegand::Wiegand(int _d0pin, int _d1pin, unsigned int _wiegandbits, unsigned int _packetGap, QObject *parent):
    QObject(parent),
    d0pin(_d0pin),
    d1pin(_d1pin),
    packetGap(_packetGap)
{
    wiegandbits=_wiegandbits;
    wiringPiSetup() ;
    pinMode(d0pin, INPUT);
    pinMode(d1pin, INPUT);

    wiegandData = new unsigned char [wiegandbits/8];
    wiegandBitCount = 0;

    wiringPiISR(d0pin, INT_EDGE_FALLING, readD0);
    wiringPiISR(d1pin, INT_EDGE_FALLING, readD1);

    timer.setSingleShot(true);
    connect(&timer, &QTimer::timeout, this, &Wiegand::on_timerTimeout);
    timer.start(500);
}

void Wiegand::readD0()
{
    if((wiegandBitCount / 8) < wiegandbits){
        wiegandData[wiegandBitCount/8] <<= 1;
        wiegandBitCount++;
    }
    clock_gettime(CLOCK_MONOTONIC, &wiegandBitTime);
}

void Wiegand::readD1()
{
    if((wiegandBitCount / 8) < wiegandbits){
        wiegandData[wiegandBitCount/8] <<= 1;
        wiegandData[wiegandBitCount/8] |= 1;
        wiegandBitCount++;
    }
    clock_gettime(CLOCK_MONOTONIC, &wiegandBitTime);
}

bool Wiegand::calculerChecksum()
{
    unsigned char sum = 0;
    int i;
    bool retour=false;
    for(i = 0; i < (wiegandbits/8)-1 ;i++){
        sum ^= (wiegandData[i] & 0xF0) >> 4 ^ (wiegandData[i] & 0x0F);
    }
    if(sum == (wiegandData[i] & 0x0F)){
        retour = true;
    }
    return retour;
}

void Wiegand::on_timerTimeout()
{
    QString strTrame;
    int bitLen = wiegandGetPendingBitCount();
    if (bitLen == 0)
        timer.start(500);
    else
    {
        char data[100];
        QByteArray trame;
        bitLen = wiegandReadData((void *)data, 100);
        int bytes = bitLen / 8 + 1;
        for (int i = 0; i < bytes; i++)
        {
            trame.push_back(data[i]);
        }
        timer.start(500);
        strTrame = trame.toHex().toUpper();
        if(calculerChecksum())
            emit NouvelleTrame(strTrame);
    }
}


void Wiegand::wiegandReset()
{
    memset((void *) wiegandData, 0, wiegandbits/8);
    wiegandBitCount = 0;
}

int Wiegand::wiegandGetPendingBitCount() {
    int retour = 0;
    struct timespec now, delta;
    clock_gettime(CLOCK_MONOTONIC, &now);
    delta.tv_sec = now.tv_sec - wiegandBitTime.tv_sec;
    delta.tv_nsec = now.tv_nsec - wiegandBitTime.tv_nsec;

    if ((delta.tv_sec > 1) || (delta.tv_nsec > WIEGANDTIMEOUT))
        retour = wiegandBitCount;

    return retour;
}

int Wiegand::wiegandReadData(void* data, int dataMaxLen) {
    int retour = 0;
    if (wiegandGetPendingBitCount() > 0) {
        int bitCount = wiegandBitCount;
        int byteCount = (wiegandBitCount / 8) + 1;
        memcpy(data, (void *)wiegandData, ((byteCount > dataMaxLen) ? dataMaxLen : byteCount));

        wiegandReset();
        retour = bitCount;
    }
    return retour;
}


