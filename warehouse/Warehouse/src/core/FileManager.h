#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include "../models/Product.h"
#include "../models/User.h"
#include <vector>
#include <string>
#include <map>
#include <memory>

class FileManager {
public:
    static std::vector<std::unique_ptr<User>> loadUsers(std::string filename);
    static void saveUsers(std::string filename, const std::vector<std::unique_ptr<User>>& users);

    static std::map<std::string, std::string> loadDepartments(std::string filename);
    static void saveDepartments(std::string filename, const std::map<std::string, std::string>& departments);

    static std::vector<std::unique_ptr<Product>> loadProducts(std::string filename, std::string department);
    static void saveProducts(std::string filename, const std::vector<std::unique_ptr<Product>>& products);

    static void logAction(std::string filename, std::string message);
    static void saveTransaction(std::string filename, std::string action, std::string user, std::string productId, int quantity);
};

#endif
