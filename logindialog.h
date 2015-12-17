#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QString>
#include <QUrl>

namespace Ui {
    class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = 0);
    ~LoginDialog();
    void setLoginUrl(const QString& url);
    QString accessToken();

signals:
    void accessTokenObtained();
    void accessDenied();

private slots:
    void urlChanged(const QUrl& url);


private:
    Ui::LoginDialog *ui;
    QString m_strAuthCode;
};

#endif // LOGINDIALOG_H
