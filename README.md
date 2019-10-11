Hello
I wrote this application in one day because need for one use and I didn't finish. If you want develop on this, yes sure you can and I'll be very happy.
This program can search any data in all tables and in this tablle's in all fields. In field befor "Szukaj" button you can put some string. This string is convert to LIKE query.. If you use * char it will be replace to % for LIKE syntax. First you need to chose firebird database file, ofcourse earlier properly fill connection fields... Then you can click "Połącz" button. In Settings 2 tab you can exclude some table from searching. In "Zapytanie sql: query" field you can create your own query but it may not work properly. If I want to search click "Szukaj" button.

Sorry for my English.

How to compile QIBASE (firebird) driver for qt sqldriver. Sorry but only in the Polish language now. I don't have a lot of time.

Musimy mieć pliki źródłowe Qt (Sources) jeżeli nie mamy to pobieramy. Najczęściej będą w katalogu Qt np. E:\Qt\Sources albo E:\Qt\Qt5.11.0\Sources
Musimy pobrać pliki firebirda może być zwykła instalka lub plik zip ja plecam plik zip: Firebird-2.5.9.27139-0_Win32.zip (https://firebirdsql.org/en/firebird-2-5/#Win32)
Uwaga ważne jest jaką wersję pobieramy czy 32 czy 64 bitową. Ja pobrałem 32 bo na 64 nie działało.
Archiwum firebirda rozpakowałem do C:\interbase chyba nie musi być akurat nazwa interbase...
Do zmiennych środowiskowych do zmiennej PATH dodałem: c:\interbase\lib;c:\interbase\include;c:\interbase\bin; nie wiem czy konieczne ale piszą, że tak
Do zmiennych środowiskowych dodałem jeszcze katalog E:\Qt\src jako zmienna środowiskowa qtdir nie dodawałem do path ale nie wiem czy jest to potrzebne chyba nie.
Teraz edytujemy plik: E:\Qt\Src\qtbase\src\plugins\sqldrivers\ibase\ibase.pro
I zakomentujemy: #QMAKE_USE += ibase
powinno wyglądać tak:

TARGET = qsqlibase

HEADERS += $$PWD/qsql_ibase_p.h
SOURCES += $$PWD/qsql_ibase.cpp $$PWD/main.cpp

# FIXME: ignores libfb (unix)/fbclient (win32) - but that's for the test anyway
#QMAKE_USE += ibase

OTHER_FILES += ibase.json

PLUGIN_CLASS_NAME = QIBaseDriverPlugin
include(../qsqldriverbase.pri)

odpalamy konsolę i przechodzimy do: e:\Qt\Src\qtbase\src\plugins\sqldrivers\ibase
uruchamiamy polecenie: 
qmake "INCLUDEPATH+=C:/interbase/include" "LIBS+=-LC:/interbase/lib" "LIBS+=-lfbclient_ms" ibase.pro
mingw32-make
mingw32-make install

mamy gotowe pliki dll w e:\Qt\Qt5.11.0\5.11.0\mingw53_32\plugins\sqldrivers i w e:\Qt\Src\qtbase\src\plugins\sqldrivers\plugins\sqldrivers\

To tyle pamiętaj aby skopiować także odpowiednie biblioteki (fbclient.dll wystarczy) dla firebirda tam gdzie plik wykonywalny albo dodać do zmiennych środowiskowych.

uwaga!!! Po kompliacji itp. Aby baza danych działała trzeba usunąć zmienne środowiskowe związane z firebirdem bo inaczej bedzie connection refused albo coś tam z messages...

https://forum.qt.io/topic/26620/how-to-build-plugin-qibase-for-firebird-qt-5-0-2/12?fbclid=IwAR2smvjPeir2VK7b81s1T1rTrviXIdczS9sjuOpBAkQla5lNjTt_RmWmmm8
https://forum.qt.io/topic/75419/ibase-sql-plugin-build-error
https://forum.qt.io/topic/92626/cant-connect-to-mysql-database-qmysql-driver-not-loaded/23
https://doc.qt.io/qt-5/sql-driver.html#qibase
https://www.elektroda.pl/rtvforum/topic1896422.html