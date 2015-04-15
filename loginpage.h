#ifndef LOGINPAGE_H
#define LOGINPAGE_H

#include "ui_loginpage.h"
#include <QWidget>

class LoginPage : public QWidget, private Ui::LoginPage
{
    Q_OBJECT

public:
    explicit LoginPage(QWidget *parent = 0);

    bool validateInput();
    bool validateHost();
    bool validatePort();
    QString getHost();
    QString getPort();
    QString getUsername();

signals:
    void connectToServerClicked();
};

#endif // LOGINPAGE_H
