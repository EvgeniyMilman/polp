#ifndef GPIBBUS_H
#define GPIBBUS_H

#include <QString>
#include <QVariant>
#include <QList>

#define BUFFFER_SIZE 0x1000

#define DEFAULT_TIMEOUT 3

class GPIBBus{
        GPIBBus();
        ~GPIBBus();
public:
    static GPIBBus* instance();

    int busTimeout;

    unsigned int openDev(QVariant dev);
    void closeDev(QVariant dev);

    QStringList findDev(QString str);

    int write(QVariant dev, void* data, int count, long unsigned int* returnCount);
    int readByte(QVariant dev, char* data);
    int readBlock(QVariant dev, void* data, int count,long unsigned int* returnCount);

    int gpib_command(QVariant dev,QString command);
    QString gpib_query(QVariant dev,QString command);
    int queryBlock(QVariant dev,QString command,void* data, int count , long unsigned int* returnCount);

private:
    char* databuffer;
};

#endif // GPIBBUS_H
