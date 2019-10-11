#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    tablesModel = new QStringListModel();
    tablesCBModel = new QStringListModel();
    this->ui->tablesCB->setModel(tablesCBModel);
}

MainWindow::~MainWindow()
{
    if(baza.isOpen()) baza.close();
    delete ui;
}

void MainWindow::showEvent(QShowEvent *e)
{
    QMainWindow::showEvent(e);
}

void MainWindow::on_searchPB_clicked()
{
    qDebug() << "click";
    if(thread!=nullptr && thread->isRunning())
    {
        qDebug() << "stop";
        if(searchWork!=nullptr) searchWork->canWork = false;
        thread->terminate();
        return;
    }
    this->ui->searchLW->clear();
    QString szukany= this->ui->searchTextLE->text();
    QString filename = this->ui->outQueryLE->text().length() == 0 ? "znalezione.txt" : this->ui->outQueryLE->text();
    //qDebug()<<searchWork;
    searchWork = new SearchWork(baza, szukany, filename, tables);
    searchWork->canWork = true;
    if(thread==nullptr)
    {
        thread = new QThread(this);
        searchWork->moveToThread(thread);
    }
    //SearchWork *searchWork = new SearchWork(baza, szukany, filename, tables);
    //searchWork->moveToThread(thread);
    connect(thread, SIGNAL(finished()), this, SLOT(threadSearchFinished()));
    connect(thread, SIGNAL(started()), this, SLOT(threadSearchStarted()));
    connect(thread, SIGNAL(finished()), searchWork, SLOT(deleteLater()));
    connect(thread, SIGNAL(started()), searchWork, SLOT(searchInDb()));
    connect(searchWork, SIGNAL(loggingTableName(QString)), this, SLOT(searchLoggingTableName(QString)));
    connect(searchWork, SIGNAL(finished()), this, SLOT(searchFinished()));
    connect(searchWork, SIGNAL(progressTables(int,int)), this, SLOT(searchProgressTables(int,int)));
    connect(searchWork, SIGNAL(loggingColumnName(QString)), this, SLOT(searchLoggingColumnName(QString)));
    connect(searchWork, SIGNAL(progressColumns(int,int)), this, SLOT(searchProgressColumnName(int,int)));
    connect(searchWork, SIGNAL(foundLog(QString)), this, SLOT(searchFoundRekord(QString)));

    thread->start();
}

void MainWindow::on_queryExecPB_clicked()
{
    this->ui->searchLW->clear();
    QString queryStr= this->ui->queryLE->text();

    QSqlQuery query(this->baza);
    query.exec(queryStr);
    while (query.next()) {
        QString rekord="";
        for(int j=0;j<query.record().count();++j)
        {
            //qDebug() << "a: " + query.record().fieldName(j);
            //qDebug() << query.value(j);
            rekord = rekord + query.record().fieldName(j) + ":[" + query.value(j).toString() + "] ";
        }
        qDebug() << rekord ;
        this->ui->searchLW->addItem(rekord);
    }
    //    QSqlTableModel *model = new QSqlTableModel(parentObject, database);
    //    model->setTable("employee");
    //    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    //    model->select();
    //    model->setHeaderData(0, Qt::Horizontal, tr("Name"));
    //    model->setHeaderData(1, Qt::Horizontal, tr("Salary"));

    //    QTableView *view = new QTableView;
    //    view->setModel(model);
    //    view->hideColumn(0); // don't show the ID
    //    view->show();
}

void MainWindow::on_dbFilePB_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
          tr("Open database"), MySettings.value(DEFAULT_DIR_KEY).toString(), tr("Db Files (*.FDB);;All files (*.*)"));
    if(fileName.length()>0)
    {
        QDir CurrentDir;
        MySettings.setValue(DEFAULT_DIR_KEY,
                            CurrentDir.absoluteFilePath(fileName));
        this->ui->dbFileLE->setText(fileName);
        this->ui->connectPB->setEnabled(true);
    }
    else
        this->ui->connectPB->setEnabled(false);
}

void MainWindow::on_connectPB_clicked()
{
    if(baza.isOpen())
    {
        ui->connectPB->setText(tr("Połącz"));
        this->ui->searchPB->setEnabled(false);
        this->ui->queryExecPB->setEnabled(false);
        this->ui->tablesCB->clear();
        this->tablesCB.clear();
        this->tables.clear();
        tablesModel->setStringList(tables);
        this->ui->tablesLV->setModel(tablesModel);
        baza.close();
        baza.removeDatabase("con1");
        return;
    }

    baza = QSqlDatabase::addDatabase("QIBASE","con1");
    foreach (QString driver, baza.drivers()) {
        std::cout << driver.toStdString() << std::endl;
    }
    baza.setHostName(this->ui->hostLE->text());
    baza.setDatabaseName(this->ui->dbFileLE->text());
  baza.setUserName(this->ui->userLE->text());
  baza.setPassword(this->ui->passwordLE->text());
  baza.setPort(this->ui->portLE->text().toInt());
  if(baza.open())
  {
      qDebug() << "OK";
      std::cout << "DB connect" << std::endl;
      this->ui->searchPB->setEnabled(true);
      this->ui->queryExecPB->setEnabled(true);
      tablesCB = baza.tables(QSql::Tables);
      tablesCB.sort();
      tablesCBModel->setStringList(tablesCB);
      ui->connectPB->setText(tr("Rozłącz"));
  }
  else
  {
      std::cout << "DB error: " << baza.lastError().text().toStdString() << std::endl;
      ui->connectPB->setText(tr("Połącz"));
      qDebug() << "Error";
      qDebug() << baza.lastError();
      QMessageBox msgBox;
    msgBox.setText("Error.");
    msgBox.setInformativeText(baza.lastError().text());
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);
    msgBox.exec();
    this->ui->searchPB->setEnabled(false);
    this->ui->queryExecPB->setEnabled(false);
    //if(msgBox.exec() == QMessageBox::Ok)
    //    QApplication::quit();
  }
}

void MainWindow::on_outQueryPB_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this,
          tr("Open database"), "", tr("Txt Files (*.txt);;All files (*.*)"));
    if(fileName.length()>0)
    {
        this->ui->outQueryLE->setText(fileName);
        this->filename = fileName;
    }
}

void MainWindow::on_tablesCB_activated(const QString &arg1)
{
    this->ui->addTB->setEnabled(true);
}

void MainWindow::on_addTB_clicked()
{
    if(!this->tables.contains(this->ui->tablesCB->currentText()))
    {
        this->tables.append(this->ui->tablesCB->currentText());
        tablesModel->setStringList(tables);
        this->ui->tablesLV->setModel(tablesModel);
        qDebug() << ui->tablesCB->count() << " : " << ui->tablesCB->currentIndex();
        ui->tablesCB->setCurrentIndex(ui->tablesCB->count() > ui->tablesCB->currentIndex()+1 ? ui->tablesCB->currentIndex()+1 : ui->tablesCB->currentIndex());
    }
}

void MainWindow::on_removeTB_clicked()
{
    QModelIndexList indexes = ui->tablesLV->selectionModel()->selectedIndexes();
    qDebug() << "S: " << ui->tablesLV->selectionModel()->selectedIndexes().size();
    while(indexes.size()) {
        tables.removeAt(indexes.last().row());
        tablesModel->setStringList(tables);
        this->ui->tablesLV->setModel(tablesModel);
       //ui->tablesLV->model()->removeRow(indexes.last().row());
       indexes.removeLast();
    }
    ui->tablesLV->selectionModel()->selectedIndexes().size() == 0 ? ui->removeTB->setEnabled(false) : ui->removeTB->setEnabled(true);
    qDebug() << tables.size();
}

void MainWindow::on_tablesLV_clicked(const QModelIndex &index)
{
    ui->removeTB->setEnabled(true);
}

void MainWindow::searchLoggingTableName(QString tableName)
{
    ui->tableProgressL->setText(tableName);
}

void MainWindow::searchProgressTables(int i, int size)
{
    ui->tablesPB->setMaximum(size);
    ui->tablesPB->setValue(i);
}

void MainWindow::searchLoggingColumnName(QString columnName)
{
    ui->columnProgressL->setText(columnName);
}

void MainWindow::searchProgressColumnName(int i, int size)
{
    ui->columnsPB->setMaximum(size);
    ui->columnsPB->setValue(i);
}

void MainWindow::searchFoundRekord(QString rekord)
{
    ui->searchLW->addItem(rekord);
}

void MainWindow::threadSearchStarted()
{
    qDebug() << "Start thr";
    ui->searchPB->setText(tr("Zatrzymaj szukanie"));
    ui->tabWidget->setDisabled(true);
    ui->connectPB->setDisabled(true);
    ui->queryExecPB->setDisabled(true);
}

void MainWindow::threadSearchFinished()
{
    qDebug() << "Finished thr";
    ui->searchPB->setText(tr("Szukaj"));
    ui->tabWidget->setDisabled(false);
    ui->connectPB->setDisabled(false);
    ui->queryExecPB->setDisabled(false);
    disconnect(thread, SIGNAL(finished()), this, SLOT(threadSearchFinished()));
    disconnect(thread, SIGNAL(started()), this, SLOT(threadSearchStarted()));
    //if(thread!=nullptr) delete thread;
}

void MainWindow::searchFinished()
{
    thread->terminate();
}
