/*
 * Author: Roger Morton (ttguy1@gmail.com
 * Date: 13 Dec 2015
 * Purpose: Demo of how use the freesound.org ver2 API.
 * This program authenticates at freesound, obtains an authorisation
 * code from freesound - if the user has authorised the app to use freesound.
 * It then obtains an access_token and finally uses that access token to download a file
 *
 * Based on code at  https://code.google.com/p/qt-oauth-lib/
 * Which is Qt Library created by Integrated Computer Solutions, Inc. (ICS)
 * to provide OAuth2.0 for the Google API.
 *
 * Licence: GNU Lesser General Public License
 * http://www.gnu.org/licenses/lgpl.html
 *
 *  Disclaimer of Warranty.
THERE IS NO WARRANTY FOR THE PROGRAM, TO THE EXTENT PERMITTED BY APPLICABLE LAW.
 EXCEPT WHEN OTHERWISE STATED IN WRITING THE COPYRIGHT HOLDERS AND/OR OTHER PARTIES PROVIDE
THE PROGRAM “AS IS” WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING,
 BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 PURPOSE. THE ENTIRE RISK AS TO THE QUALITY AND PERFORMANCE OF THE PROGRAM IS WITH YOU.
SHOULD THE PROGRAM PROVE DEFECTIVE, YOU ASSUME THE COST OF ALL NECESSARY SERVICING, REPAIR
OR CORRECTION.

 Limitation of Liability.
IN NO EVENT UNLESS REQUIRED BY APPLICABLE LAW OR AGREED TO IN WRITING WILL ANY COPYRIGHT HOLDER,
 OR ANY OTHER PARTY WHO MODIFIES AND/OR CONVEYS THE PROGRAM AS PERMITTED ABOVE, BE LIABLE TO
 YOU FOR DAMAGES, INCLUDING ANY GENERAL, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES ARISING
OUT OF THE USE OR INABILITY TO USE THE PROGRAM (INCLUDING BUT NOT LIMITED TO LOSS OF DATA OR
 DATA BEING RENDERED INACCURATE OR LOSSES SUSTAINED BY YOU OR THIRD PARTIES OR A FAILURE OF
THE PROGRAM TO OPERATE WITH ANY OTHER PROGRAMS), EVEN IF SUCH HOLDER OR OTHER PARTY HAS BEEN
 ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.


*/

#include "oauth2.h"
#include <QDebug>
#include <QApplication>
#include "logindialog.h"
#include <QSettings>
#include <QMessageBox>

OAuth2::OAuth2(QWidget* parent)

{
   //  m_strEndPoint = "https://www.freesound.org/apiv2/oauth2/logout_and_authorize/";
    m_strEndPoint = "https://www.freesound.org/apiv2/oauth2/authorize/";

    m_strClientID = "YOUR_CLIENT_ID_HERE";//as you will have obtained when you registered  your app
              //and can be found here http://www.freesound.org/apiv2/apply/



    m_strClientSecret ="YOUR_CLIENT_SECRET_HERE";//as you will have obtained when you registered  your app
            //and can be found here http://www.freesound.org/apiv2/apply/



    m_strRedirectURI == "YOUR_REDIRECT_URI_HERE"; //as you will have set up when you registered  your app
            //and can be found here http://www.freesound.org/apiv2/apply/
//    likely "https://www.freesound.org/home/app_permissions/permission_granted/";




    m_strResponseType = "code";

    m_strCompanyName = "company.org"; //You company here
    m_strAppName = "QT Free sound"; //Your application name here

    m_pLoginDialog = new LoginDialog(parent);
    m_pParent = parent;
    connect(m_pLoginDialog, SIGNAL(accessTokenObtained()), this, SLOT(accessTokenObtained()));
    connect(m_pLoginDialog, SIGNAL(accessDenied()), this, SLOT(SlotAccessDenied()));
}

void OAuth2::setClientID(const QString& clientID)
{
    m_strClientID = clientID;
}

 QString OAuth2::getClientID()
 {
     return m_strClientID;
 }
  QString OAuth2::getClientSecret()
  {
      return m_strClientSecret;
  }

void OAuth2::setRedirectURI(const QString& redirectURI)
{
    m_strRedirectURI = redirectURI;
}

void OAuth2::setCompanyName(const QString& companyName)
{
    m_strCompanyName = companyName;
}

void OAuth2::setAppName(const QString& appName)
{
    m_strAppName = appName;
}



QString OAuth2::loginUrl()
{

    QString str = QString("%1?client_id=%2&redirect_uri=%3&response_type=%4").arg(m_strEndPoint).arg(m_strClientID).arg(m_strRedirectURI).arg(m_strResponseType);
    qDebug() << "Login URL" << str;
    return str;
}

QString OAuth2::accessToken()
{
    return m_strAccessToken;
}

bool OAuth2::isAuthorized()
{
    return m_strAccessToken != "";
}

void OAuth2::startLogin(bool bForce)
{
    QSettings settings(m_strCompanyName, m_strAppName);
    QString str = settings.value("access_token", "").toString();

    qDebug() << "OAuth2::startLogin, token from Settings" << str;
    if(m_strClientID == "YOUR_CLIENT_ID_HERE" || m_strRedirectURI == "YOUR_REDIRECT_URI_HERE" || m_strClientSecret =="YOUR_CLIENT_SECRET_HERE")
    {
        QMessageBox::warning(m_pParent, "Warning",
                             "To work with application you need to register your own application in <b>Freesound</b> <a href='http://www.freesound.org/apiv2/apply/'>here</a>.\n"
                             "Learn more from <a href='http://www.freesound.org/docs/api/'>here</a> <br> Once you have registered you need to edit oauth2.cpp source file and add your clientID, redirect URI and client secret ");
        return;
    }


    if(str.isEmpty() || bForce)//  if we have not got an access token saved in settings or we have been asked to force the obtaining of a new access token
    {                             // then login to free sound via our login dialog
        m_pLoginDialog->setLoginUrl(loginUrl());
        m_pLoginDialog->show();
    }
    else// use the access token previously saved in settings
    {
        m_strAccessToken = str;
        emit loginDone();
    }
}

void OAuth2::accessTokenObtained()
{
    QSettings settings(m_strCompanyName, m_strAppName); // create a new settings object
    m_strAccessToken = m_pLoginDialog->accessToken(); // get the access token we have obtained
    settings.setValue("access_token", m_strAccessToken); // save the access token in our settings object
    emit loginDone(); // notify something. In this case MainWindow is notified

}

void OAuth2::SlotAccessDenied()
{
    qDebug() << "access denied";
    emit accessDenied();
}
