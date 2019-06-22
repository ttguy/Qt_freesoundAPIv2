#include "logindialog.h"
#include "ui_logindialog.h"

#include <QDebug>
#include <QtWebEngine/QtWebEngine>

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
    connect(ui->webView, SIGNAL(urlChanged(QUrl)), this, SLOT(urlChanged(QUrl)));
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

void LoginDialog::urlChanged(const QUrl &url)
{
    qDebug() << "URL =" << url;
    QString str = url.toString();
    int posCode = str.indexOf("code=");// urls now do not have % in them
    int posErr = str.indexOf("error=");
    if(posCode != -1)
    {

        m_strAuthCode =str.mid(posCode+5) ;
        emit accessTokenObtained();
        QDialog::accept();

    }
    if(posErr != -1)
    {

        QString sError =str.mid(posErr+6) ;
        if (sError=="access_denied" )
        {
            emit accessDenied();
        }
        QDialog::accept();

    }
}

QString LoginDialog::accessToken()
{
    return m_strAuthCode;
}


void LoginDialog::setLoginUrl(const QString& url)
{
   ui->webView->setUrl(url);
}
