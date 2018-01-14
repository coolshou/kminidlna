/*
    KMiniDLNA
    https://sourceforge.net/projects/kminidlna/
    
    Copyright (C) 2012  Tomáš Poledný saljacky(at)gmail(dot)com

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


#include "certificategenerator.h"
#include <openssl/x509v3.h>
#include <openssl/x509.h>
#include <openssl/pem.h>
#include <openssl/conf.h>

#include <openssl/crypto.h>

#ifndef OPENSSL_NO_ENGINE
#include <openssl/engine.h>
#endif

#include <stdio.h>
#include <stdlib.h>

#include <QString>

#include <QFile>
#include <QDebug>

using namespace std;

const QString CertificateGenerator::PKEY_NAME = "privatekey.key";
const QString CertificateGenerator::CERTIFICATE_NAME = "cert.crt";
/**
 * @param value value of certificate
 * @param path path where certificate and private key will be saved default is "", must end with / for example: /home/xxxx/
 */
CertificateGenerator::CreatedCertificteError CertificateGenerator::createCertificate(X509Value& value, const QString path, unsigned int bits) {
    CreatedCertificteError error = NoError;
    BIO *bio_err;
    X509 *x509 = 0;
    EVP_PKEY *pkey = 0;

    CRYPTO_mem_ctrl(CRYPTO_MEM_CHECK_ON);
    bio_err = BIO_new_fp(stderr, BIO_NOCLOSE);
    int serial = qrand();

    int c = mkcert(&x509, &pkey, bits, serial, 365, value);
    if(c == 1) {
// 	RSA_print_fp(stdout,pkey->pkey.rsa,0);
//     X509_print_fp(stdout, x509);

        QFile pkeyF(path + PKEY_NAME);
        FILE* pkeyFILE = 0;
        if(pkeyF.open(QIODevice::ReadWrite)) {
            int fileHandle = pkeyF.handle();
            if(fileHandle != -1) {
                pkeyFILE = fdopen(fileHandle, "w");
            }
        } else {
            qDebug() << "Cannot open file: " << pkeyF.fileName();
        }


        QFile certF(path + CERTIFICATE_NAME);
        FILE* certFILE = 0;
        if(certF.open(QIODevice::ReadWrite)) {
            int fileHandle = certF.handle();
            if(fileHandle != -1) {
                certFILE = fdopen(fileHandle, "w");
            }
        } else {
            qDebug() << "Cannot open file: " << certF.fileName();
        }



        if(certFILE != 0 && pkeyFILE != 0) {
            PEM_write_PrivateKey(pkeyFILE, pkey, 0, 0, 0, 0, 0);
            PEM_write_X509(certFILE, x509);
            qDebug() << "Certificate and private key was created.";
        } else {
            qDebug() << "Certificate and private key was not created. Check persmission";
        }

        fclose(certFILE);
        fclose(pkeyFILE);
        certF.close();
        pkeyF.close();
    } else {
        qDebug() << "Certificate and private key was not generate. Wrong ";
        error = GenerateError;
    }

    X509_free(x509);
    EVP_PKEY_free(pkey);

#ifndef OPENSSL_NO_ENGINE
    ENGINE_cleanup();
#endif
    CRYPTO_cleanup_all_ex_data();

    CRYPTO_mem_leaks(bio_err);
    BIO_free(bio_err);
    return error;
}

void CertificateGenerator::callback(int p, int n, void* arg) {
    char c = 'B';

    if(p == 0) c = '.';
    if(p == 1) c = '+';
    if(p == 2) c = '*';
    if(p == 3) c = '\n';
    fputc(c, stderr);
}

/**
 *
 * @param x509p certificate
 * @param pkeyp private key
 * @param bits length
 * @param serial
 * @param days not after num of days
 * @param value value of X509 (C, CN ...)
 */
int CertificateGenerator::mkcert(X509** x509p, EVP_PKEY** pkeyp, int bits, int serial, int days, const X509Value& value) {
    X509 *x;
    EVP_PKEY *pk;
    RSA *rsa;
    X509_NAME *name = 0;

    if((pkeyp == 0) || (*pkeyp == 0)) {
        if((pk = EVP_PKEY_new()) == 0) {
            abort();
            return 0;
        }
    } else {
        pk = *pkeyp;
    }

    if((x509p == 0) || (*x509p == 0)) {
        if((x = X509_new()) == 0)
            goto err;
    } else {
        x = *x509p;
    }

    rsa = RSA_generate_key(bits, RSA_F4, 0, 0);
    if(!EVP_PKEY_assign_RSA(pk, rsa)) {
        abort();
        goto err;
    }
    rsa = 0;

    X509_set_version(x, 2);
    ASN1_INTEGER_set(X509_get_serialNumber(x), serial);
    X509_gmtime_adj(X509_get_notBefore(x), 0);
    X509_gmtime_adj(X509_get_notAfter(x), (long)60 * 60 * 24 * days);
    X509_set_pubkey(x, pk);

    name = X509_get_subject_name(x);

    /* This function creates and adds the entry, working out the
     * correct string type and performing checks on its length.
     * Normally we'd check the return value for errors...
     */
    X509_NAME_add_entry_by_txt(name, "C",
                               MBSTRING_ASC, value.countryName, -1, -1, 0);
    X509_NAME_add_entry_by_txt(name, "O",
                               MBSTRING_ASC, (unsigned char*) "KMiniDLNA", -1, -1, 0);
//     X509_NAME_add_entry_by_txt(name, "OU",
//                                MBSTRING_ASC, (unsigned char*) "KMiniDLNA", -1, -1, 0);
    X509_NAME_add_entry_by_txt(name, "CN",
                               MBSTRING_ASC, value.commonName, -1, -1, 0);

    /* Its self signed so set the issuer name to be the same as the
     * subject.
     */
    X509_set_issuer_name(x, name);

    /* Add various extensions: standard extensions */
//     add_ext(x, NID_basic_constraints, (char*)"critical,CA:TRUE");
    add_ext(x, NID_basic_constraints, (char*)"CA:TRUE");
//     add_ext(x, NID_key_usage, (char*) "keyCertSign,cRLSign");//remove critical

    add_ext(x, NID_subject_key_identifier, (char*)"hash");

    /* Some Netscape specific extensions */
    add_ext(x, NID_netscape_cert_type, (char*)"sslCA");

    add_ext(x, NID_netscape_comment, (char*) "created by KMiniDLNA");


// #ifdef CUSTOM_EXT
//     /* Maybe even add our own extension based on existing */
//     {
//         int nid;
//         nid = OBJ_create("1.2.3.4", "MyAlias", "My Test Alias Extension");
//         X509V3_EXT_add_alias(nid, NID_netscape_comment);
//         add_ext(x, nid, "example comment alias");
//     }
// #endif

    if(!X509_sign(x, pk, EVP_sha1())) {
        goto err;
    }

    *x509p = x;
    *pkeyp = pk;
    return 1;
err:
    return 0;
}

/* Add extension using V3 code: we can set the config file as 0
 * because we wont reference any other sections.
 */

int CertificateGenerator::add_ext(X509* cert, int nid, char* value) {
    X509_EXTENSION *ex;
    X509V3_CTX ctx;
    /* This sets the 'context' of the extensions. */
    /* No configuration database */
    X509V3_set_ctx_nodb(&ctx);
    /* Issuer and subject certs: both the target since it is self signed,
     * no request and no CRL
     */
    X509V3_set_ctx(&ctx, cert, cert, 0, 0, 0);
    ex = X509V3_EXT_conf_nid(0, &ctx, nid, value);
    if(!ex)
        return 0;

    X509_add_ext(cert, ex, -1);
    X509_EXTENSION_free(ex);
    return 1;
}
