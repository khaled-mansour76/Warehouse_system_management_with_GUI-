#ifndef DASHBOARDWINDOW_H
#define DASHBOARDWINDOW_H

#include <QWidget>
#include <QTableWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>
#include "../core/Warehouse.h"
#include "../models/User.h"
#include <vector>
#include <memory>

class DashboardWindow : public QWidget {
    Q_OBJECT

public:
    DashboardWindow(Warehouse* warehouse, User* user, std::vector<std::unique_ptr<User>>& allUsers, QWidget *parent = nullptr);

private slots:
    void on_btnLogout_clicked();
    void handleAddProduct();
    void handleDeleteProduct();
    void handleEditProduct();
    void handleViewReports();
    void handleDeposit();
    void handleWithdraw();
    void handleSearch();
    void handleChangePassword();
    void handleCreateDepartment();

private:
    Warehouse* warehouse;
    User* currentUser;
    std::vector<std::unique_ptr<User>>& allUsers;
    QTableWidget* productTable;
    QComboBox* deptSelector;
    QPushButton* btnLogout;
    
    QLineEdit* idInput;
    QLineEdit* nameInput;
    QLineEdit* qtyInput;
    QLineEdit* priceInput;
    QLineEdit* xInput;
    QLineEdit* yInput;

    void refreshTable();
    void setupUI();
};

#endif
