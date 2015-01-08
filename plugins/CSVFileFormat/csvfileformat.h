#ifndef CSVFILEFORMAT_H
#define CSVFILEFORMAT_H

#include "csvfileformat_global.h"
#include "plugins.h"

#include <QTextStream>


class CSVFILEFORMATSHARED_EXPORT CSVFileFormat: public QObject , public FileFormat{

    Q_OBJECT
    Q_INTERFACES(FileFormat)
    Q_PLUGIN_METADATA(IID "ISEFileFormat.Plugin")
public:
    explicit CSVFileFormat(QObject *parent = 0);
    virtual QString extension();
    virtual QString description();
    virtual int loadData(Data* data);
    virtual int saveData(Data* data);
    virtual QString error();
    virtual QString preferedView();
protected:
    QTextStream getInputDataStream(QString filename);
    int validateFilename(QString filename);
    bool hasError();
private:
    QString error_message;
};

#endif // CSVFILEFORMAT_H
