#ifndef SIMPLEVIEW_H
#define SIMPLEVIEW_H

#include <QObject>
#include <plugins.h>
#include <qcustomplot.h>

namespace Ui {
class SimpleViewForm;
}
/*
 * Properties:      Value
 * view_split       String  1/1  (col, row) Default
 * view.curve.plot  int     1               Default
 * view.curve.color QColor
 * view.curve.title String
 * view_plot_legend Bool    true Default
 * view_plot_title  String
 * view_plot_x
 * view_plot_y
 */


class SimpleView : public QWidget, public View
{
    Q_OBJECT
    Q_INTERFACES(View)
public:
    explicit SimpleView(QWidget *parent = 0);
    ~SimpleView();
    virtual QString title();
    virtual QWidget * viewPane();
    virtual int setData(Data* data);
    virtual QString error();
signals:

public slots:

private:
    QString error_message;
    Ui::SimpleViewForm* ui;
    QVector<QCPLegend*> legends;
    QVector<QCPAxisRect *> _preparePlotWidget(Data2D* data);
};

#endif // SIMPLEVIEW_H
