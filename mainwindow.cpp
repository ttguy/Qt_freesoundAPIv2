#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QVariant>
#include <QMap>
#include <QUrlQuery>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonParseError>
#include <QJsonDocument>

#include <QIODevice>
#include <QDir>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
   m_pOAuth2 = new OAuth2(this);
   demoFile="https://www.freesound.org/apiv2/sounds/39206/download/";//  Download url of a freesound file
   demoSaveLocation=QDir::homePath();
   demoSaveLocation.append("/Downloads/down2.wav");
   connect(m_pOAuth2, SIGNAL(loginDone()), this, SLOT(loginDone()));
   connect(m_pOAuth2, SIGNAL(accessDenied()), this, SLOT(SlotAccessDenied()));
   // textBrowser->setHTML("here");
    mDisplayText="Click Button to login to freesound and download a file using the oauth2 authentication. It will download ";
    mDisplayText.append( demoFile);
    mDisplayText.append(  " to location ");
    mDisplayText.append(  demoSaveLocation);
    ui->textBrowser->setPlainText( mDisplayText);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::startLogin(bool bForce)
{
   m_pOAuth2->startLogin(bForce);
}


void MainWindow::loginDone()
{
     QString sAuthorizationCode=m_pOAuth2->accessToken();

    qDebug() << sAuthorizationCode;
     mDisplayText="Auth Code Obtained";
     ui->textBrowser->setPlainText( mDisplayText);

 QNetworkAccessManager *networkManager = new QNetworkAccessManager(this);
 QUrl serviceUrl = QUrl("https://www.freesound.org/apiv2/oauth2/access_token/");

 QUrlQuery postData;

 postData.addQueryItem("client_id", m_pOAuth2->getClientID());
 postData.addQueryItem("client_secret", m_pOAuth2->getClientSecret());
 postData.addQueryItem("grant_type", "authorization_code");
 postData.addQueryItem("code", sAuthorizationCode);
  connect(networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(serviceRequestFinished(QNetworkReply*)));
 QNetworkRequest request(serviceUrl);
 request.setHeader(QNetworkRequest::ContentTypeHeader,     "application/x-www-form-urlencoded");
 mDisplayText.append("<br> getting Access Token");
 ui->textBrowser->setHtml("<html>" + mDisplayText + "</html>");
 networkManager->post(request, postData.toString(QUrl::FullyEncoded).toUtf8());


    /*

     curl -X POST -d "client_id=33e04fxxx2710yyc&client_secret=441d8837471a3503xxxx1e4780566f7313&grant_type=authorization_code&code=<sAuthorizationCode>" https://www.freesound.org/apiv2/oauth2/access_token/
    returns some JSON  with an accessToken
    {"access_token": "cf1533c01fb3cb1bb0a0ca6f3beb5c9d7f7d8fb7"   ...
    Access token can be used to download a file like this
    curl -o "save3.wav" -H "Authorization: Bearer cf1533c01fb3cb1bb0a0ca6f3beb5c9d7f7d8fb7" 'https://www.freesound.org/apiv2/sounds/39206/download/'

    curl -X  option  Specifies a custom request method
    -d option Sends the specified data in a POST request to the HTTP server, in the same way that a browser does when a user has filled in an HTML form and presses the submit button.
    This will cause curl to pass the data to the server using the content-type application/x-www-form-urlencoded.
    So I think the -X post option is not needed
*/

}

void MainWindow::on_pushButton_clicked()
{
    MainWindow::startLogin(true);
}

void MainWindow::serviceRequestFinished(QNetworkReply* reply)
{
    QString sAccessToken;
    QString sRefreshToken;
    QString sErrorText;
    bool bAccessTokenRec=false;
    if (reply->isFinished())
    {
        QByteArray	sReply = reply->readAll();
         qDebug() << sReply ;
         QJsonParseError jsonError;
         QJsonDocument doc = QJsonDocument::fromJson(sReply, &jsonError);
         if (jsonError.error != QJsonParseError::NoError) {
            qDebug() <<  jsonError.errorString();
            mDisplayText.append("<br>Error obtaining access token<br>");
            mDisplayText.append( jsonError.errorString());
            ui->textBrowser->setHtml("<html>" + mDisplayText + "</html>");
         }
         else
         {
             QVariant data = doc.toVariant();

             if (data.canConvert(QVariant::Map)) {
                 QMap <QString, QVariant> map = data.toMap();


                 if (map.contains("access_token")) {
                      sAccessToken = map.value("access_token").toString();
                      bAccessTokenRec = true;
                      mDisplayText.append("<br>Access Token obtained");
                      ui->textBrowser->setHtml("<html>" + mDisplayText + "</html>");
                      qDebug() << "Access Token:  "<<  sAccessToken;
                 }
                 if (map.contains("refresh_token")) {
                      sRefreshToken = map.value("refresh_token").toString();
                 }
                 if (map.contains("error")) {
                      sErrorText = map.value("error").toString();
                      qDebug() << "error:  "<<  sErrorText;
                 }

                 if (bAccessTokenRec)
                 {
                     QNetworkAccessManager *networkManager = new QNetworkAccessManager(this);

                     QNetworkRequest request;
                     request.setUrl(QUrl(demoFile));//  Download url of a freesound file
                     // eg https://www.freesound.org/apiv2/sounds/39206/download/
                     request.setRawHeader(QByteArray("Authorization"),QByteArray( "Bearer ").append( sAccessToken));
                     mDisplayText.append("<br>Starting File Download<br>");
                     ui->textBrowser->setHtml("<html>" + mDisplayText + "</html>");
                     QNetworkReply *reply2 = networkManager->get(request);
                     connect(reply2, SIGNAL(readyRead()), this, SLOT(slotReadyRead()));
                     connect(networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(DownloadRequestFinished(QNetworkReply*)));
                //    connect(reply2, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(slotError(QNetworkReply::NetworkError)));
               //     connect(reply2, SIGNAL(sslErrors(QList<QSslError>)), this, SLOT(slotSslErrors(QList<QSslError>)));

                //     Access token can be used to download a file like this
                //     curl -o "save3.wav" -H "Authorization: Bearer cf1533c01fb3cb1bb0a0ca6f3beb5c9d7f7d8fb7" 'https://www.freesound.org/apiv2/sounds/39206/download/'


                }
                 else
                 {
                     mDisplayText.append("<br>Failed to Get Access Token<br>");
                     mDisplayText.append(sErrorText);
                     ui->textBrowser->setHtml("<html>" + mDisplayText + "</html>");
                 }
             }
         }

    }
    reply->deleteLater();

}

void MainWindow::DownloadRequestFinished(QNetworkReply* reply)
{
    if (reply->isFinished())
    {
        mDisplayText.append("<br> Saving file to <br>");
        mDisplayText.append(demoSaveLocation);
        ui->textBrowser->setHtml("<html>" + mDisplayText + "</html>");
        QByteArray	aSoundData = reply->readAll();
        QFile file(demoSaveLocation);
        if (file.open(QIODevice::WriteOnly))
        {
            file.write(aSoundData );
            file.close();
            mDisplayText.append("<br>File Saved");
            ui->textBrowser->setHtml("<html>" + mDisplayText + "</html>");
        }
        else
        {
            mDisplayText.append("<br>Error Saving File");
            ui->textBrowser->setHtml("<html>" + mDisplayText + "</html>");
        }
    }

}

void MainWindow::slotReadyRead()
{
qDebug() <<"slotReadyRead()";
mDisplayText.append(".");
ui->textBrowser->setHtml("<html>" + mDisplayText + "</html>");

}

void MainWindow::SlotAccessDenied()
{
    mDisplayText.append("<br>Access Denied");
    ui->textBrowser->setHtml("<html>" + mDisplayText + "</html>");
}
