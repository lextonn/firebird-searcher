#ifndef SEARCHWORK_H
#define SEARCHWORK_H

#include <QObject>
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

class SearchWork : public QObject
{
    Q_OBJECT
public:
    SearchWork(QSqlDatabase &baza, QString szukany, QString filename, QStringList tables, QWidget *parent = 0);
    virtual ~SearchWork(){}
    QSqlDatabase &baza;
    QString szukany;
    QString filename;
    QStringList tables;
    bool canWork;
public slots:
    //searchInDb(QSqlDatabase &baza, QString &szukany, QString &filename, QStringList &tables);
    searchInDb();
signals:
    void loggingTableName(QString tableName);
    void loggingColumnName(QString columnName);
    void progressTables(int i, int size);
    void progressColumns(int i, int size);
    void foundLog(QString rekord);
    void finished();
};

#endif // SEARCHWORK_H
