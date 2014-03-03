#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QComboBox>
#include <QItemSelection>
#include <QMainWindow>
#include <plugins.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionNew_triggered();

private:
    Ui::MainWindow *ui;
    QComboBox * analyserBox;
    Analyser * currentAnalyser;
    View * currentView;
    Data * currentData;

    void loadViews();
    void loadAnalysers();
    void loadSimulations();
    void loadDevices();
    void loadTools();
    void displayData(Data* data);
    void analyse();
    void setCurrentView(QString viewtitle);
    void setCurrentView(View* view);

private slots:
    void view_selected(QString viewtitle);
    void simulation_add(QString simulationtitle);
    void device_add(QString devicetitle);
    void onProjectItemSelectionChanged(QItemSelection item);
    void onAnalyserBoxIndexChanged(int index);
    void tool_selected(QObject*);
   void showSettings();
    void on_actionFileNew_triggered();
    void on_actionFileOpen_triggered();
    void on_actionFileSave_triggered();
    void on_toolsWidget_tabCloseRequested(int index);
    void on_actionProjectSave_triggered();
    void on_actionProjectSave_As_triggered();
    void on_actionProjectOpen_triggered();
};

#endif // MAINWINDOW_H
