/*
http://github.com/coolshou/qminidlna

Copyright (C) 2018 jimmy

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


#ifndef GENERATECERTIFICATEDIALOG_H
#define GENERATECERTIFICATEDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QString>
// #include "../server/cert/certificategenerator.h"
namespace Ui {
    class GenerateCertificateDialog;
} // namespace Ui


class GenerateCertificateDialog : public QDialog
{
Q_OBJECT
    void initGUI();
    public:
        GenerateCertificateDialog(QWidget* parent, Qt::WindowFlags flags = 0);
        virtual ~GenerateCertificateDialog();
        QString commonName();
        QString country();
    private:
        Ui::GenerateCertificateDialog *ui;
        QLineEdit* m_txlCommonName;
        QLineEdit* m_txlCountryName;
};

#endif // GENERATECERTIFICATEDIALOG_H
