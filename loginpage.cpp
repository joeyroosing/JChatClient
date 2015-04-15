#include "loginpage.h"

#include <QRegExp>
#include <QMessageBox>
#include <QIntValidator>

LoginPage::LoginPage(QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);
    portLineEdit->setValidator(new QIntValidator(0, 65536, this));
    connect(connectButton, &QPushButton::clicked, [this]() {
        emit connectToServerClicked();
    });
}

bool LoginPage::validateInput()
{
    if (!validateHost()) {
        return false;
    }

    if (!validatePort()) {
        return false;
    }

    return true;
}

bool LoginPage::validateHost()
{
    QString host = hostLineEdit->text();
    if (host.isEmpty()) {
        QMessageBox::warning(this, tr("Invalid host"),
                             tr("Host is required.\n"
                                "Please enter a valid hostname."));
        return false;
    }
    // host is not empty, so start validating the string

    // set up some validation rules
    QRegExp ipAddressRegExp("^(([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])\\.){3}([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])$");

    if (!ipAddressRegExp.exactMatch(host)) {
        QMessageBox::warning(this, tr("Invalid host"),
                             tr("Host is an invalid ip address.\n"
                                "Please enter a valid host."));
                return false;
    }
    // host is a valid one
    return true;
}

bool LoginPage::validatePort()
{
    QString port = portLineEdit->text();
    // check if the port is a number
    if (port.isEmpty()) {
        QMessageBox::warning(this, tr("Invalid Port"),
                             tr("Port is a required field"));
        return false;
    }
    return true;
}

QString LoginPage::getHost()
{
    return hostLineEdit->text();
}

QString LoginPage::getPort()
{
    return portLineEdit->text();
}

QString LoginPage::getUsername()
{
    return usernameLineEdit->text();
}
