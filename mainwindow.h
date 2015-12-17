#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "oauth2.h"
#include <QNetworkAccessManager>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);

    ~MainWindow();

private:
    Ui::MainWindow *ui;
    OAuth2*  m_pOAuth2;
    QString demoFile;
    QString demoSaveLocation;
    QString  mDisplayText;
    void startLogin(bool bForce);
private slots:
    void loginDone();

    void on_pushButton_clicked();
    void serviceRequestFinished(QNetworkReply* reply);
    void slotReadyRead();
    void DownloadRequestFinished(QNetworkReply* reply);
    void SlotAccessDenied();
};

#endif // MAINWINDOW_H
