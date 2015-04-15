#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindow.h"
#include <QMainWindow>
#include <QTcpSocket>

class LoginPage;
class ChatPage;

class MainWindow : public QMainWindow, private Ui::MainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);

private slots:
    // Called when the connect button in LoginPage is clicked
    void onConnectClicked();

    // This is a function we'll connect to a socket's readyRead()
    // signal, which tells us there's text to be read from the chat
    // server.
    void readyRead();

    // This function gets called when the socket tells us it's connected.
    void connected();

    // called when an error with the socket occurs
    void handleSocketError(QAbstractSocket::SocketError socketError);

    // called when the send button in the chat page is clicked
    void sendMessage();

private:
    // connect signals to slots
    void addHandlers();

    // UI
    LoginPage *loginPage;
    ChatPage *chatPage;

    // Networking
    QTcpSocket *socket;
};

#endif // MAINWINDOW_H
