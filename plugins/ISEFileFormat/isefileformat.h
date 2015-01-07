#ifndef ISEFILEFORMAT_H
#define ISEFILEFORMAT_H
#include "isefileformat_global.h"
#include "plugins.h"

#include <QTextStream>

class ISEFILEFORMATSHARED_EXPORT ISEFileFormat: public QObject , public FileFormat{
    Q_OBJECT
    Q_INTERFACES(FileFormat)
    Q_PLUGIN_METADATA(IID "ISEFileFormat.Plugin")
public:
    explicit ISEFileFormat(QObject *parent = 0);
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

#endif // ISEFILEFORMAT_H
