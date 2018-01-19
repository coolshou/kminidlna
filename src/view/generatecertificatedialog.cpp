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


#include "generatecertificatedialog.h"
#include "ui_generatecertificatedialog.h"

#include <QVBoxLayout>
#include <QLabel>
#include <QLocale>
//#include <KLocale>

GenerateCertificateDialog::GenerateCertificateDialog(QWidget* parent, Qt::WindowFlags flags):
    QDialog(parent, flags),
    ui(new Ui::GenerateCertificateDialog)
{
    ui->setupUi(this);
    initGUI();
}

GenerateCertificateDialog::~GenerateCertificateDialog() {

}

void GenerateCertificateDialog::initGUI() {
    m_txlCommonName = ui->m_txlCommonName;
    m_txlCountryName = ui->m_txlCountryName;
}

QString GenerateCertificateDialog::commonName() {
    return m_txlCommonName->text();
}

QString GenerateCertificateDialog::country() {
    return m_txlCountryName->text();
}


