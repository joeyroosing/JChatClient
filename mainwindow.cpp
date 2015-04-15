#include "mainwindow.h"
#include "loginpage.h"
#include "chatpage.h"

#include <QDebug>
#include <QRegExp>
#include <QMessageBox>
#include <QHostAddress>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    setupUi(this);

    loginPage = new LoginPage(this);
    chatPage = new ChatPage(this);

    // the stacked widget has no initial pages, so add them
    stackedWidget->addWidget(loginPage);
    stackedWidget->addWidget(chatPage);
    // set the login page as active
    stackedWidget->setCurrentWidget(loginPage);

    // prepare the socket, but don't connect yet
    socket = new QTcpSocket(this);

    addHandlers();
}

void MainWindow::addHandlers()
{
    // This is how we tell Qt to call our readyRead() and connected()
    // functions when the socket has text ready to be read, and is done
    // connecting to the server (respectively):
    connect(socket, &QTcpSocket::readyRead, this, &MainWindow::readyRead);
    connect(socket, &QTcpSocket::connected, this, &MainWindow::connected);
    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(handleSocketError(QAbstractSocket::SocketError)));
    connect(loginPage, &LoginPage::connectToServerClicked, this, &MainWindow::onConnectClicked);
    connect(chatPage, &ChatPage::onSendClicked, this, &MainWindow::sendMessage);
}

void MainWindow::sendMessage()
{
    QString message = chatPage->getMessage().trimmed();
    if (message.isEmpty()) {
        return;
    }

    socket->write(QString(message + "\n").toUtf8());
    chatPage->clearChatLineEdit();
}

void MainWindow::handleSocketError(QAbstractSocket::SocketError socketError)
{
    switch(socketError) {
    case QAbstractSocket::RemoteHostClosedError:
        break;
    case QAbstractSocket::HostNotFoundError:
        QMessageBox::information(this, tr("JChat"),
                                 tr("The host was not found. Please check the "
                                    "host name and port settings."));
        break;
    case QAbstractSocket::ConnectionRefusedError:
        QMessageBox::information(this, tr("JChat"),
                                 tr("The connection was refused by the peer. "
                                    "Make sure the server is running, "
                                    "and check that the host name and port "
                                    "settings are correct."));
        break;
    default:
        QMessageBox::information(this, tr("JChat"),
                                 tr("The following error occurred: %1.")
                                 .arg(socket->errorString()));
    }
}

void MainWindow::onConnectClicked()
{
    bool inputValid = loginPage->validateInput();
    QString host = loginPage->getHost();
    quint16 port = loginPage->getPort().toInt();

    if (!inputValid) {
        qDebug() << "Invalid input: " << host << ":" << port;
        return;
    }

    qDebug() << "Trying to connect to " << host << ":" << port;
    // try to connect to the host on given host and port
    // if for some reason the connection failed the
    // handleSocketError will be called
    socket->connectToHost(host, port);
}

void MainWindow::readyRead()
{
    // will return true as long as there is a new line to be read
    while (socket->canReadLine()) {
        // read the line in UTF-8 format
        QString line = QString::fromUtf8(socket->readLine()).trimmed();

        // These two regular expressions describe the kinds of messages
        // the server can send us:

        //  Normal messges look like this: "username:The message"
        QRegExp messageRegex("^([^:]+):(.*)$");

        // Any message that starts with "/users:" is the server sending us a
        // list of users so we can show that list in our GUI:
        QRegExp usersRegex("^/users:(.*)$");

        // Is this a users message:
        if(usersRegex.indexIn(line) != -1) {
            // If so, udpate our users list on the right:
            QStringList users = usersRegex.cap(1).split(",");
            chatPage->clearUserList();
            foreach(QString user, users) {
                chatPage->addUser(user);
            }
        } else if(messageRegex.indexIn(line) != -1) { // Is this a normal chat message:

            // If so, append this message to our chat box:
            QString user = messageRegex.cap(1);
            QString message = messageRegex.cap(2);
            chatPage->appendChatText(user, message);
        }
    }
}

void MainWindow::connected()
{
    // Flip over to the chat page:
    stackedWidget->setCurrentWidget(chatPage);

    QString username = loginPage->getUsername();
    if (username.isEmpty()) {
        username = socket->localAddress().toString();
    }

    // And send our username to the chat server.
    socket->write(QString("/me:" + username + "\n").toUtf8());
}
