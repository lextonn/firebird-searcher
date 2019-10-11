#include "searchwork.h"

SearchWork::SearchWork(QSqlDatabase &baza, QString szukany, QString filename, QStringList tables, QWidget *parent) :
    QObject(parent),
    baza(baza),
    szukany(szukany),
    filename(filename),
    tables(tables),
    canWork(true)
{

}

SearchWork::searchInDb()
{
    QStringList lista_tabel;
        lista_tabel = baza.tables(QSql::Tables);
        lista_tabel.sort();

        //QString szukany= this->ui->searchTextLE->text();
        QString szukany = this->szukany;
        szukany.replace("*","%");

        QSqlQuery query(baza);
        QSqlQuery query2(baza);


        //cout << filename.toStdString() << endl;
        QFile plik;
        plik.setFileName(filename);
        plik.open( QIODevice::WriteOnly | QIODevice::Truncate );

        QTextStream data( &plik );
        data.setCodec("UTF-8");

        for(int i=0;i<lista_tabel.count() && canWork;++i)
            //int i = 3;
        {
            emit progressTables(i+1,lista_tabel.count());
            emit loggingTableName(lista_tabel.at(i));
            if(tables.contains(lista_tabel.at(i),Qt::CaseInsensitive))
            {
                qDebug() << "Omitted " << i << ": " << lista_tabel.at(i);
                continue;
            }
            query.exec("SELECT * From "+lista_tabel.at(i));
            for(int j=0;j<query.record().count() && canWork;++j)
            {
                qDebug() << i << ": " << lista_tabel.at(i) << " : " << query.record().fieldName(j);
                emit progressColumns(j+1,query2.record().count());
                emit loggingColumnName(query2.record().fieldName(j));
                QString zapytanie = "SELECT * From " + lista_tabel.at(i) + " where " + query.record().fieldName(j) + " like '" + szukany + "'";
                //qDebug() << zapytanie;
                query2.exec(zapytanie);
                while (query2.next() && canWork) {
                    QString rekord="";
                    for(int j=0;j<query2.record().count() && canWork;++j)
                    {
                        //qDebug() << "a: " + query.record().fieldName(j);
                        //qDebug() << query.value(j);
                        rekord = rekord + query2.record().fieldName(j) + ":[" + query2.value(j).toString() + "] ";
                    }
                    rekord = lista_tabel.at(i) + " # " + rekord;
                    qDebug() << "----------------" + lista_tabel.at(i) + "----------------";
                    qDebug() << rekord ;
                    data << rekord << "\n";
                    qDebug() << "_________________________________________________________";
                    emit foundLog(rekord);
                }
            }
        }

        //plik.close();


//        int licz=0;
//        if(1==0)
//        {
//            query2.exec("SELECT * From leki");
//            query2.next();
//            while (query2.next()) {
//                //if(licz==10) break;
//                ++licz;
//                QString rekord="";
//                for(int j=0;j<query2.record().count();++j)
//                {
//                    //qDebug() << query.record().fieldName(j);
//                    //qDebug() << query.value(j);
//                    rekord = rekord + query2.record().fieldName(j) + ":[" + query2.value(j).toString() + "] ";
//                }
//                rekord = QString::number(licz) + "; Leki # " + rekord;
//                qDebug() << rekord ;
//                data << rekord << "\n";
//            }
//        }

//        if(1==0)
//        {
//            for(licz=1;licz>11;++licz)
//            {
//                query2.exec("SELECT * From kzak where idtowr = "+QString::number(licz));
//                while (query2.next()) {
//                    QString rekord="";
//                    for(int j=0;j<query2.record().count();++j)
//                    {
//                        //qDebug() << query.record().fieldName(j);
//                        //rekord = query2.value(3).toString();
//                        rekord = rekord + query2.record().fieldName(j) + ":[" + query2.value(j).toString() + "] ";
//                    }
//                    rekord = QString::number(licz) + "; KZAK # " + rekord;
//                    qDebug() << rekord ;
//                    data << rekord << "\n";
//                }
//            }
//        }
//        if(1==0)
//        {
//            for(licz=1;licz>11;++licz)
//            {
//                query2.exec("SELECT * From sprz where idtowr = "+QString::number(licz));
//                while (query2.next()) {
//                    QString rekord="";
//                    for(int j=0;j<query2.record().count();++j)
//                    {
//                        //qDebug() << query.record().fieldName(j);
//                        //rekord = query2.value(3).toString();
//                        rekord = rekord + query2.record().fieldName(j) + ":[" + query2.value(j).toString() + "] ";
//                    }
//                    rekord = QString::number(licz) + "; sprz # " + rekord;
//                    qDebug() << rekord ;
//                    data << rekord << "\n";
//                }
//            }
//        }

        plik.close();

        /*for(int i=0;i<0;++i)
        {
            qDebug() << lista_tabel.at(i);
            query.exec("SELECT * From "+lista_tabel.at(i));
            while (query.next()) {
                QString rekord="";
                for(int j=0;j<query.record().count();++j)
                {
                    //qDebug() << query.record().fieldName(j);
                    //qDebug() << query.value(j);
                    rekord = rekord + "[" + query.value(j).toString() + "] ";
                    if(szukany.contains(query.value(j).toString()))
                    {

                    }
                }
                qDebug() << rekord;
            }
        }*/
        qDebug() << "OK";
        emit finished();

        /*QMessageBox msgBox;
        msgBox.setText(QString::number());
        msgBox.exec();*/
}
