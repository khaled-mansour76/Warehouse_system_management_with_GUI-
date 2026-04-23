#include "DashboardWindow.h"
#include "../core/FileManager.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QLabel>
#include <QHeaderView>
#include <QInputDialog>
#include <QFile>
#include <QTextStream>
#include "LoginWindow.h"

DashboardWindow::DashboardWindow(Warehouse* w, User* u, std::vector<std::unique_ptr<User>>& au, QWidget *parent)
    : QWidget(parent), warehouse(w), currentUser(u), allUsers(au) {
    setAttribute(Qt::WA_DeleteOnClose);
    setupUI();
    refreshTable();
    resize(850, 650);
}

void DashboardWindow::setupUI() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    QHBoxLayout* topLayout = new QHBoxLayout();
    QLabel* welcomeLabel = new QLabel(QString("Welcome, %1 (%2)").arg(QString::fromStdString(currentUser->getUsername())).arg(QString::fromStdString(currentUser->getRole())), this);
    topLayout->addWidget(welcomeLabel);

    topLayout->addStretch();
    
    QLabel* deptLabel = new QLabel("Department:", this);
    topLayout->addWidget(deptLabel);

    deptSelector = new QComboBox(this);
    deptSelector->addItem("All");
    for (const auto& d : warehouse->getDepartments()) {
        deptSelector->addItem(QString::fromStdString(d));
    }
    topLayout->addWidget(deptSelector);
    connect(deptSelector, &QComboBox::currentTextChanged, this, &DashboardWindow::refreshTable);

    if (currentUser->getRole() == "admin") {
        QPushButton* createDeptBtn = new QPushButton("Create Dept");
        topLayout->addWidget(createDeptBtn);
        connect(createDeptBtn, &QPushButton::clicked, this, &DashboardWindow::handleCreateDepartment);
    }

    btnLogout = new QPushButton("Logout", this);
    topLayout->addWidget(btnLogout);
    connect(btnLogout, &QPushButton::clicked, this, &DashboardWindow::on_btnLogout_clicked);

    mainLayout->addLayout(topLayout);

    productTable = new QTableWidget(this);
    productTable->setColumnCount(7);
    productTable->setHorizontalHeaderLabels({"ID", "Name", "Quantity", "Price", "X", "Y", "Dept"});
    productTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    mainLayout->addWidget(productTable);

    QHBoxLayout* inputLayout = new QHBoxLayout();
    idInput = new QLineEdit(); idInput->setPlaceholderText("ID");
    nameInput = new QLineEdit(); nameInput->setPlaceholderText("Name");
    qtyInput = new QLineEdit(); qtyInput->setPlaceholderText("Quantity");
    priceInput = new QLineEdit(); priceInput->setPlaceholderText("Price");
    xInput = new QLineEdit(); xInput->setPlaceholderText("X");
    yInput = new QLineEdit(); yInput->setPlaceholderText("Y");

    inputLayout->addWidget(idInput);
    inputLayout->addWidget(nameInput);
    inputLayout->addWidget(qtyInput);
    inputLayout->addWidget(priceInput);
    inputLayout->addWidget(xInput);
    inputLayout->addWidget(yInput);

    mainLayout->addLayout(inputLayout);

    QHBoxLayout* buttonLayout = new QHBoxLayout();

    if (currentUser->getRole() == "admin") {
        QPushButton* addButton = new QPushButton("Add Product");
        QPushButton* delButton = new QPushButton("Delete Product");
        QPushButton* editButton = new QPushButton("Edit Product");
        QPushButton* reportButton = new QPushButton("View Reports");
        buttonLayout->addWidget(addButton);
        buttonLayout->addWidget(delButton);
        buttonLayout->addWidget(editButton);
        buttonLayout->addWidget(reportButton);
        connect(addButton, &QPushButton::clicked, this, &DashboardWindow::handleAddProduct);
        connect(delButton, &QPushButton::clicked, this, &DashboardWindow::handleDeleteProduct);
        connect(editButton, &QPushButton::clicked, this, &DashboardWindow::handleEditProduct);
        connect(reportButton, &QPushButton::clicked, this, &DashboardWindow::handleViewReports);
    } else {
        QPushButton* depButton = new QPushButton("Deposit Stock");
        QPushButton* witButton = new QPushButton("Withdraw Stock");
        buttonLayout->addWidget(depButton);
        buttonLayout->addWidget(witButton);
        connect(depButton, &QPushButton::clicked, this, &DashboardWindow::handleDeposit);
        connect(witButton, &QPushButton::clicked, this, &DashboardWindow::handleWithdraw);
    }

    QPushButton* searchButton = new QPushButton("Search (by ID or Name)");
    QPushButton* passButton = new QPushButton("Change Password");
    buttonLayout->addWidget(searchButton);
    buttonLayout->addWidget(passButton);
    connect(searchButton, &QPushButton::clicked, this, &DashboardWindow::handleSearch);
    connect(passButton, &QPushButton::clicked, this, &DashboardWindow::handleChangePassword);

    mainLayout->addLayout(buttonLayout);
}

void DashboardWindow::refreshTable() {
    std::string dept = deptSelector->currentText().toStdString();
    auto products = warehouse->getProducts(dept);
    
    productTable->setRowCount(products.size());
    for (size_t i = 0; i < products.size(); ++i) {
        productTable->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(products[i]->getId())));
        productTable->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(products[i]->getName())));
        productTable->setItem(i, 2, new QTableWidgetItem(QString::number(products[i]->getQuantity())));
        productTable->setItem(i, 3, new QTableWidgetItem(QString::number(products[i]->getPrice())));
        productTable->setItem(i, 4, new QTableWidgetItem(QString::number(products[i]->getLocationX())));
        productTable->setItem(i, 5, new QTableWidgetItem(QString::number(products[i]->getLocationY())));
        productTable->setItem(i, 6, new QTableWidgetItem(QString::fromStdString(products[i]->getDepartment())));
    }
}

void DashboardWindow::handleCreateDepartment() {
    QString name = QInputDialog::getText(this, "New Department", "Enter department name:");
    if (!name.isEmpty()) {
        warehouse->createDepartment(name.toStdString());
        deptSelector->blockSignals(true);
        deptSelector->clear();
        deptSelector->addItem("All");
        for (const auto& d : warehouse->getDepartments()) {
            deptSelector->addItem(QString::fromStdString(d));
        }
        deptSelector->setCurrentText(name);
        deptSelector->blockSignals(false);
        refreshTable();
        QMessageBox::information(this, "Success", "Department created successfully!");
    }
}

void DashboardWindow::handleAddProduct() {
    std::string id = idInput->text().toStdString();
    std::string name = nameInput->text().toStdString();
    int qty = qtyInput->text().toInt();
    double price = priceInput->text().toDouble();
    int x = xInput->text().toInt();
    int y = yInput->text().toInt();
    
    std::string dept = deptSelector->currentText().toStdString();
    if (dept == "All") {
        QMessageBox::warning(this, "Error", "Please select a specific department to add to!");
        return;
    }

    if (id.empty() || name.empty()) {
        QMessageBox::warning(this, "Error", "Invalid inputs!");
        return;
    }

    auto p = std::make_unique<Product>(id, name, qty, price, x, y, dept);
    if (warehouse->addProduct(std::move(p))) {
        FileManager::logAction("../data/logs.csv", "ADD_PRODUCT: " + id);
        warehouse->save();
        refreshTable();
    } else {
        QMessageBox::warning(this, "Error", "Duplicate ID!");
    }
}

void DashboardWindow::handleDeleteProduct() {
    std::string id = idInput->text().toStdString();
    std::string dept = deptSelector->currentText().toStdString();
    
    if (warehouse->deleteProduct(id, dept)) {
        FileManager::logAction("../data/logs.csv", "DELETE_PRODUCT: " + id);
        warehouse->save();
        refreshTable();
    } else {
        QMessageBox::warning(this, "Error", "Product not found!");
    }
}

void DashboardWindow::handleEditProduct() {
    std::string id = idInput->text().toStdString();
    double newPrice = priceInput->text().toDouble();
    std::string dept = deptSelector->currentText().toStdString();

    if (id.empty() || newPrice <= 0) {
        QMessageBox::warning(this, "Error", "Invalid ID or Price!");
        return;
    }

    if (warehouse->editProduct(id, newPrice, dept)) {
        FileManager::logAction("../data/logs.csv", "EDIT_PRODUCT: " + id);
        warehouse->save();
        refreshTable();
    } else {
        QMessageBox::warning(this, "Error", "Product not found!");
    }
}

void DashboardWindow::handleViewReports() {
    QFile file("../data/transactions.csv");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        QString data = in.readAll();
        QMessageBox::information(this, "Transactions Report", data);
        file.close();
    } else {
        QMessageBox::warning(this, "Error", "Could not load reports!");
    }
}

void DashboardWindow::handleDeposit() {
    std::string id = idInput->text().toStdString();
    int qty = qtyInput->text().toInt();
    std::string dept = deptSelector->currentText().toStdString();

    if (warehouse->depositStock(id, qty, currentUser->getUsername(), dept)) {
        FileManager::logAction("../data/logs.csv", "DEPOSIT: " + id);
        warehouse->save();
        refreshTable();
    } else {
        QMessageBox::warning(this, "Error", "Product not found or invalid quantity!");
    }
}

void DashboardWindow::handleWithdraw() {
    std::string id = idInput->text().toStdString();
    int qty = qtyInput->text().toInt();
    std::string dept = deptSelector->currentText().toStdString();

    if (warehouse->withdrawStock(id, qty, currentUser->getUsername(), dept)) {
        FileManager::logAction("../data/logs.csv", "WITHDRAW: " + id);
        warehouse->save();
        refreshTable();
    } else {
        QMessageBox::warning(this, "Error", "Not enough stock or not found!");
    }
}

void DashboardWindow::handleSearch() {
    std::string id = idInput->text().toStdString();
    std::string name = nameInput->text().toStdString();
    std::string dept = deptSelector->currentText().toStdString();
    
    Product* p = nullptr;
    if (!id.empty()) {
        p = warehouse->binarySearchByID(id, dept);
    } else if (!name.empty()) {
        p = warehouse->linearSearchByName(name, dept);
    }

    if (p) {
        QString msg = QString("Found in %1\nID: %2 - %3\nQty: %4\nLoc: (%5, %6)").arg(QString::fromStdString(p->getDepartment()), QString::fromStdString(p->getId()), QString::fromStdString(p->getName())).arg(p->getQuantity()).arg(p->getLocationX()).arg(p->getLocationY());
        QMessageBox::information(this, "Search Result", msg);
        FileManager::logAction("../data/logs.csv", "SEARCH: " + p->getId());
    } else {
        QMessageBox::warning(this, "Search Result", "Product not found!");
    }
}

void DashboardWindow::handleChangePassword() {
    QString newPass = QInputDialog::getText(this, "Change Password", "Enter new password:", QLineEdit::Password);
    if (!newPass.isEmpty()) {
        currentUser->setPassword(newPass.toStdString());
        FileManager::saveUsers("../data/users.csv", allUsers);
        QMessageBox::information(this, "Success", "Password changed successfully!");
        FileManager::logAction("../data/logs.csv", "PASSWORD_CHANGE: " + currentUser->getUsername());
    }
}

void DashboardWindow::on_btnLogout_clicked() {
    warehouse->save();
    
    this->close();
    
    LoginWindow* loginWin = new LoginWindow(warehouse, allUsers);
    loginWin->show();
}
