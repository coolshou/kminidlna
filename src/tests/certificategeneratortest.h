/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) 2012  TomÃ¡Å¡ PolednÃ½ <email>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#ifndef CERTIFICATEGENERATORTEST_H
#define CERTIFICATEGENERATORTEST_H

#include <qt4/QtCore/QObject>
#include <QString>


class CertificateGeneratorTest : public QObject {
  Q_OBJECT
    private:
        static const QString SAVE_PATH;
    private slots:

        void initTestCase(); // will be called before the first testfunction is executed.
        void cleanupTestCase(); // will be called after the last testfunction was executed.
        void init(); // will be called before each testfunction is executed.
        void cleanup(); // will be called after every testfunction.
        void generateValidCertificateAndPKey();
};

#endif // CERTIFICATEGENERATORTEST_H
