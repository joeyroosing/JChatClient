#ifndef CHATPAGE_H
#define CHATPAGE_H

#include "ui_chatpage.h"
#include <QWidget>

class ChatPage : public QWidget, private Ui::ChatPage
{
    Q_OBJECT

public:
    explicit ChatPage(QWidget *parent = 0);

    QString getMessage();
    void clearChatLineEdit();
    void clearUserList();
    void addUser(const QString &username);
    void appendChatText(const QString &user, const QString &message);

signals:
    void onSendClicked();

private:
};

#endif // CHATPAGE_H
