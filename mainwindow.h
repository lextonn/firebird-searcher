#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QDebug>
#include <QSqlError>
#include <QMessageBox>
#include <QFile>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlTableModel>
#include "QFileDialog"
#include <QString>
#include <QSettings>
#include <QStringListModel>
#include <QStringList>
#include <QThread>
#include <iostream>
#include "searchwork.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void showEvent(QShowEvent *e);

private slots:

    void on_searchPB_clicked();

    void on_queryExecPB_clicked();

    void on_dbFilePB_clicked();

    void on_connectPB_clicked();

    void on_outQueryPB_clicked();

    void on_tablesCB_activated(const QString &arg1);

    void on_addTB_clicked();

    void on_removeTB_clicked();

    void on_tablesLV_clicked(const QModelIndex &index);

private:
    Ui::MainWindow *ui;
    QSqlDatabase baza;
    const QString DEFAULT_DIR_KEY {"dbpath"};
    QSettings MySettings;
    QString filename;
    QStringList tables, tablesCB;
    QStringListModel *tablesModel, *tablesCBModel;
    QThread *thread = nullptr;
    SearchWork *searchWork = nullptr;
private slots:
    void searchLoggingTableName(QString tableName);
    void searchProgressTables(int i, int size);
    void searchLoggingColumnName(QString columnName);
    void searchProgressColumnName(int i, int size);
    void searchFoundRekord(QString rekord);
    void threadSearchStarted();
    void threadSearchFinished();
    void searchFinished();
};

#endif // MAINWINDOW_H
