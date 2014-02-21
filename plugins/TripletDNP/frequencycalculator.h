#ifndef FREQUENCYCALCULATOR_H
#define FREQUENCYCALCULATOR_H

#include <QWidget>
#include <plugins.h>

class FrequencyCalculator : public QWidget, public Tool
{
    Q_OBJECT
    Q_INTERFACES(Tool)
public:
    explicit FrequencyCalculator(QWidget *parent = 0);
    virtual QString title();
    virtual QWidget *toolPane();
signals:

public slots:

};

#endif // FREQUENCYCALCULATOR_H
