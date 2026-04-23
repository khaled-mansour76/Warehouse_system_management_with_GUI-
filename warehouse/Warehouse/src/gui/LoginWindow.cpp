#include "LoginWindow.h"
#include "DashboardWindow.h"
#include "../core/FileManager.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QMessageBox>

LoginWindow::LoginWindow(Warehouse* w, std::vector<std::unique_ptr<User>>& u, QWidget *parent)
    : QWidget(parent), warehouse(w), users(u) {
    
    setAttribute(Qt::WA_DeleteOnClose);
    
    QVBoxLayout* layout = new QVBoxLayout(this);

    QLabel* title = new QLabel("Warehouse Login", this);
    layout->addWidget(title);

    usernameInput = new QLineEdit(this);
    usernameInput->setPlaceholderText("Username");
    layout->addWidget(usernameInput);

    passwordInput = new QLineEdit(this);
    passwordInput->setPlaceholderText("Password");
    passwordInput->setEchoMode(QLineEdit::Password);
    layout->addWidget(passwordInput);

    loginButton = new QPushButton("Login", this);
    layout->addWidget(loginButton);

    connect(loginButton, &QPushButton::clicked, this, &LoginWindow::handleLogin);
}

void LoginWindow::handleLogin() {
    QString uName = usernameInput->text();
    QString pWord = passwordInput->text();

    User* loggedInUser = nullptr;
    for(const auto& user : users) {
        if(user->getUsername() == uName.toStdString() && user->getPassword() == pWord.toStdString()) {
            loggedInUser = user.get();
            break;
        }
    }

    if(loggedInUser) {
        FileManager::logAction("../data/logs.csv", "LOGIN: " + loggedInUser->getUsername());
        DashboardWindow* dashboard = new DashboardWindow(warehouse, loggedInUser, users);
        dashboard->show();
        this->close();
    } else {
        QMessageBox::critical(this, "Error", "Wrong username or password!");
    }
}
