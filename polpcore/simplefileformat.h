#ifndef SIMPLEFILEFORMAT_H
#define SIMPLEFILEFORMAT_H

#include "plugins.h"

#include <QObject>

class SimpleFileFormat : public QObject ,public FileFormat
{
    Q_OBJECT
    Q_INTERFACES(FileFormat)
public:
    explicit SimpleFileFormat(QObject *parent = 0);
    virtual QString extension();
    virtual QString description();

    // Returns non zero value if errors occur(For example: Faild to open file)
    virtual int loadData(Data* data);
    virtual int saveData(Data* data);

    //Returns error message
    virtual QString error();

    virtual QString preferedView();
signals:

public slots:

};

#endif // SIMPLEFILEFORMAT_H
