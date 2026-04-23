#include <QApplication>
#include "core/Warehouse.h"
#include "core/FileManager.h"
#include "gui/LoginWindow.h"
#include "models/User.h"
#include <vector>
#include <memory>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    auto users = FileManager::loadUsers("../data/users.csv");
    Warehouse warehouse("../data/departments.csv", "../data/logs.csv", "../data/transactions.csv");

    LoginWindow* loginWin = new LoginWindow(&warehouse, users);
    loginWin->show();

    return app.exec();
}
