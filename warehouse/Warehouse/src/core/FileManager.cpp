#include "FileManager.h"
#include "../models/Admin.h"
#include "../models/Staff.h"
#include <fstream>
#include <sstream>
#include <ctime>

std::vector<std::unique_ptr<User>> FileManager::loadUsers(std::string filename) {
    std::vector<std::unique_ptr<User>> users;
    std::ifstream file(filename);
    std::string line;
    if (file.is_open()) {
        std::getline(file, line);
        while (std::getline(file, line)) {
            std::stringstream ss(line);
            std::string u, p, r;
            std::getline(ss, u, ',');
            std::getline(ss, p, ',');
            std::getline(ss, r, ',');
            if (!u.empty()) {
                if (r == "admin") {
                    users.push_back(std::make_unique<Admin>(u, p));
                } else {
                    users.push_back(std::make_unique<Staff>(u, p));
                }
            }
        }
        file.close();
    }
    return users;
}

void FileManager::saveUsers(std::string filename, const std::vector<std::unique_ptr<User>>& users) {
    std::ofstream file(filename, std::ios::trunc);
    if (file.is_open()) {
        file << "username,password,role\n";
        for (const auto& u : users) {
            file << u->getUsername() << "," << u->getPassword() << "," << u->getRole() << "\n";
        }
        file.close();
    }
}

std::map<std::string, std::string> FileManager::loadDepartments(std::string filename) {
    std::map<std::string, std::string> depts;
    std::ifstream file(filename);
    std::string line;
    if (file.is_open()) {
        std::getline(file, line);
        while (std::getline(file, line)) {
            std::stringstream ss(line);
            std::string name, path;
            std::getline(ss, name, ',');
            std::getline(ss, path, ',');
            if (!name.empty()) {
                depts[name] = path;
            }
        }
        file.close();
    }
    return depts;
}

void FileManager::saveDepartments(std::string filename, const std::map<std::string, std::string>& departments) {
    std::ofstream file(filename, std::ios::trunc);
    if (file.is_open()) {
        file << "department_name,file_name\n";
        for (const auto& pair : departments) {
            file << pair.first << "," << pair.second << "\n";
        }
        file.close();
    }
}

std::vector<std::unique_ptr<Product>> FileManager::loadProducts(std::string filename, std::string department) {
    std::vector<std::unique_ptr<Product>> products;
    std::ifstream file(filename);
    std::string line;
    if (file.is_open()) {
        std::getline(file, line);
        while (std::getline(file, line)) {
            std::stringstream ss(line);
            std::string id, name, qStr, pStr, xStr, yStr;
            std::getline(ss, id, ',');
            std::getline(ss, name, ',');
            std::getline(ss, qStr, ',');
            std::getline(ss, pStr, ',');
            std::getline(ss, xStr, ',');
            std::getline(ss, yStr, ',');
            if (!id.empty()) {
                products.push_back(std::make_unique<Product>(id, name, std::stoi(qStr), std::stod(pStr), std::stoi(xStr), std::stoi(yStr), department));
            }
        }
        file.close();
    }
    return products;
}

void FileManager::saveProducts(std::string filename, const std::vector<std::unique_ptr<Product>>& products) {
    std::ofstream file(filename, std::ios::trunc);
    if (file.is_open()) {
        file << "id,name,quantity,price,locationX,locationY\n";
        for (const auto& p : products) {
            file << p->getId() << "," << p->getName() << "," << p->getQuantity() << "," << p->getPrice() << "," << p->getLocationX() << "," << p->getLocationY() << "\n";
        }
        file.close();
    }
}

void FileManager::logAction(std::string filename, std::string message) {
    std::ofstream file(filename, std::ios::app);
    if (file.is_open()) {
        std::time_t now = std::time(nullptr);
        char buf[100];
        std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", std::localtime(&now));
        file << buf << "," << message << "\n";
        file.close();
    }
}

void FileManager::saveTransaction(std::string filename, std::string action, std::string user, std::string productId, int quantity) {
    std::ofstream file(filename, std::ios::app);
    if (file.is_open()) {
        std::time_t now = std::time(nullptr);
        char buf[100];
        std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", std::localtime(&now));
        file << buf << "," << action << "," << user << "," << productId << "," << quantity << "\n";
        file.close();
    }
}
