#include "chatpage.h"

ChatPage::ChatPage(QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);
    connect(sendButton, &QPushButton::clicked, [this]() {
        emit onSendClicked();
    });
}

QString ChatPage::getMessage()
{
    return sendLineEdit->text();
}

void ChatPage::clearChatLineEdit()
{
    // clear out the input box so they can type something else:
    sendLineEdit->clear();
    // put the focus back into the input box so they can type again:
    sendLineEdit->setFocus();
}

void ChatPage::addUser(const QString &username)
{
    userListWidget->addItem(new QListWidgetItem(username, userListWidget));
}

void ChatPage::clearUserList()
{
    userListWidget->clear();
}

void ChatPage::appendChatText(const QString &user, const QString &message)
{
    chatTextEdit->append("<b>" + user + "</b>: " + message);
}

