#ifndef WAREHOUSE_H
#define WAREHOUSE_H

#include "../models/Product.h"
#include <vector>
#include <string>
#include <map>
#include <memory>

class Warehouse {
private:
    std::string departmentsFile;
    std::string logsFile;
    std::string transFile;

    std::map<std::string, std::string> departmentPaths;
    std::map<std::string, std::vector<std::unique_ptr<Product>>> products;

    void sortProductsByID(std::string department);

public:
    Warehouse(std::string dFile, std::string lFile, std::string tFile);
    
    std::vector<Product*> getProducts(std::string department) const;
    std::vector<std::string> getDepartments() const;
    void createDepartment(std::string name);

    bool addProduct(std::unique_ptr<Product> p);
    bool deleteProduct(std::string id, std::string department = "");
    bool editProduct(std::string id, double newPrice, std::string department = "");
    bool depositStock(std::string id, int amount, std::string username, std::string department = "");
    bool withdrawStock(std::string id, int amount, std::string username, std::string department = "");

    Product* linearSearchByName(std::string name, std::string department = "");
    Product* binarySearchByID(std::string id, std::string department = "");
    
    void save();
};

#endif
