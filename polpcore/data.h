#ifndef DATA_H
#define DATA_H

#include <QObject>
#include <QMap>
#include <QString>
#include <QVariant>
#include <QList>
#include <QVector>
/**
 * @brief The Data class
 * Parametres:
 *          title
 *          filename
 *          deviceID
 */
class Data : public QObject{
    Q_OBJECT
public:
    explicit Data(QObject *parent = 0);
    virtual void startEdit() =0;
    virtual void stopEdit() =0;
    QVariant parameter(QString key);
    void setParameter(QString key, QVariant value);
    QList<QString> parameterList();

signals:
    void dataChanged();
public slots:

private:
    QMap<QString,QVariant> parameters;
};

class Data2D: public Data{
    Q_OBJECT
public:
    explicit Data2D(QObject *parent = 0);
    void addPoint(QString curve, double x, double y);
    QVector<double> *x(QString curve);
    QVector<double> *y(QString curve);
    QList<QString> curvers();
    virtual void startEdit();
    virtual void stopEdit();
    void addCurve(QString curve,QVector<double> *x,QVector<double> *y );
    void removeCurve(QString curve);
private:
    QMap<QString,QVector<double>*> xdata;
    QMap<QString,QVector<double>*> ydata;
};

class DataBin : public Data{
    Q_OBJECT
public:
    explicit DataBin(QObject *parent = 0);
    ulong size();
    char *data();
    void setData(ulong size, char* data);
    virtual void startEdit();
    virtual void stopEdit();
private:
    char* _data;
    ulong _size;
};
#endif // DATA_H
