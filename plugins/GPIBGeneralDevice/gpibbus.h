#ifndef GPIBBUS_H
#define GPIBBUS_H

#include <QString>
#include <QVariant>

#define BUFFFER_SIZE 0x1000

#define DEFAULT_TIMEOUT 3

class GPIBBus{
        GPIBBus();
public:
    static GPIBBus* instance();

    int busTimeout;

    int openDev(QVariant dev);
    void closeDev(QVariant dev);

    int write(QVariant dev, void* data, int count, int* returnCount);
    int readByte(QVariant dev, char* data);
    int readBlock(QVariant dev, void* data, int count,int* returnCount);

    int command(QVariant dev,QString command);
    QString query(QVariant dev,QString command);
    int queryBlock(QVariant dev,QString command,void* data, int count , int* returnCount);

private:
    char* databuffer;
};

#endif // GPIBBUS_H
