#ifndef OAUTH2_H
#define OAUTH2_H

#include <QString>
#include <QObject>

class LoginDialog;

class OAuth2 : public QObject
{
    Q_OBJECT

public:
    OAuth2(QWidget* parent = 0);
    QString accessToken();
    bool isAuthorized();
    void startLogin(bool bForce);

    //Functions to set application's details.
    void setScope(const QString& scope);
    void setClientID(const QString& clientID);
    QString getClientID();
    QString getClientSecret();
    void setRedirectURI(const QString& redirectURI);
    void setCompanyName(const QString& companyName);
    void setAppName(const QString& appName);

    QString loginUrl();

signals:
    //Signal that is emitted when login is ended OK.
    void loginDone();
    // signal emited if the freesound has denied access to the application
    void accessDenied();

private slots:
    void accessTokenObtained();
    void SlotAccessDenied();

private:
    QString m_strAccessToken;

    QString m_strEndPoint;

    QString m_strClientID;
    QString m_strClientSecret;
    QString m_strRedirectURI;
    QString m_strResponseType;

    QString m_strCompanyName;
    QString m_strAppName;

    LoginDialog* m_pLoginDialog;
    QWidget* m_pParent;
};

#endif // OAUTH2_H
