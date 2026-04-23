#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include "../core/Warehouse.h"
#include "../models/User.h"
#include <vector>
#include <memory>

class LoginWindow : public QWidget {
    Q_OBJECT

public:
    LoginWindow(Warehouse* warehouse, std::vector<std::unique_ptr<User>>& users, QWidget *parent = nullptr);

private slots:
    void handleLogin();

private:
    Warehouse* warehouse;
    std::vector<std::unique_ptr<User>>& users;
    QLineEdit* usernameInput;
    QLineEdit* passwordInput;
    QPushButton* loginButton;
};

#endif
