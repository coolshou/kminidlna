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


#ifndef CERTIFICATEGENERATOR_H
#define CERTIFICATEGENERATOR_H

#include <openssl/ossl_typ.h>
#include <QString>
struct X509Value;

class CertificateGenerator {
    public:
        enum CreatedCertificteError {
            NoError = 0,
            FileError,
            GenerateError

        };
        static CreatedCertificteError createCertificate(X509Value& value, const QString path = "", unsigned int bits = 512);
	static const QString PKEY_NAME;
	static const QString CERTIFICATE_NAME;
    private:
        static int mkcert(X509 **x509p, EVP_PKEY **pkeyp, int bits, int serial, int days, const X509Value& value);
        static int add_ext(X509 *cert, int nid, char *value);
        static void callback(int p, int n, void *arg);
};

struct X509Value {
    unsigned char* countryName;//C
    unsigned char* commonName;//CN
//     DC	domainComponent
//     OU	organizationalUnitName
//     O	organizationName
//     STREET	streetAddress
//     L	localityName
//     ST	stateOrProvinceName
//     UID	userid
};

#endif // CERTIFICATEGENERATOR_H
