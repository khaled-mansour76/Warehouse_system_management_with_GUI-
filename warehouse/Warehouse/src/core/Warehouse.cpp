#include "Warehouse.h"
#include "FileManager.h"
#include <algorithm>

Warehouse::Warehouse(std::string dFile, std::string lFile, std::string tFile) 
    : departmentsFile(dFile), logsFile(lFile), transFile(tFile) {
    
    departmentPaths = FileManager::loadDepartments(departmentsFile);
    if(departmentPaths.empty()) {
        departmentPaths["Electronics"] = "electronics.csv";
    }

    for (const auto& pair : departmentPaths) {
        products[pair.first] = FileManager::loadProducts("../data/" + pair.second, pair.first);
        sortProductsByID(pair.first);
    }
}

void Warehouse::createDepartment(std::string name) {
    if (departmentPaths.find(name) == departmentPaths.end()) {
        std::string filename = name + ".csv";
        std::transform(filename.begin(), filename.end(), filename.begin(), ::tolower);
        departmentPaths[name] = filename;
        products[name] = std::vector<std::unique_ptr<Product>>();
        save();
    }
}

void Warehouse::sortProductsByID(std::string department) {
    if (products.find(department) != products.end()) {
        std::sort(products[department].begin(), products[department].end(), 
            [](const std::unique_ptr<Product>& a, const std::unique_ptr<Product>& b) {
                return a->getId() < b->getId();
            });
    }
}

std::vector<std::string> Warehouse::getDepartments() const {
    std::vector<std::string> keys;
    for(const auto& pair : departmentPaths) keys.push_back(pair.first);
    return keys;
}

std::vector<Product*> Warehouse::getProducts(std::string department) const {
    std::vector<Product*> result;
    if (department == "All") {
        for (const auto& pair : products) {
            for (const auto& p : pair.second) {
                result.push_back(p.get());
            }
        }
    } else if (products.find(department) != products.end()) {
        for (const auto& p : products.at(department)) {
            result.push_back(p.get());
        }
    }
    return result;
}

Product* Warehouse::linearSearchByName(std::string name, std::string department) {
    if (department.empty() || department == "All") {
        for (const auto& pair : products) {
            for (const auto& p : pair.second) {
                if (p->getName() == name) return p.get();
            }
        }
    } else {
        auto it = products.find(department);
        if (it != products.end()) {
            for (const auto& p : it->second) {
                if (p->getName() == name) return p.get();
            }
        }
    }
    return nullptr;
}

Product* Warehouse::binarySearchByID(std::string id, std::string department) {
    if (department.empty() || department == "All") {
        for (const auto& pair : products) {
            Product* res = binarySearchByID(id, pair.first);
            if (res) return res;
        }
        return nullptr;
    }

    auto it = products.find(department);
    if (it == products.end()) return nullptr;

    const auto& vec = it->second;
    int left = 0;
    int right = vec.size() - 1;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (vec[mid]->getId() == id) return vec[mid].get();
        if (vec[mid]->getId() < id) left = mid + 1;
        else right = mid - 1;
    }
    return nullptr;
}

bool Warehouse::addProduct(std::unique_ptr<Product> p) {
    if (binarySearchByID(p->getId()) != nullptr) {
        return false;
    }
    std::string dept = p->getDepartment();
    products[dept].push_back(std::move(p));
    sortProductsByID(dept);
    return true;
}

bool Warehouse::deleteProduct(std::string id, std::string department) {
    if (department.empty() || department == "All") {
        Product* p = binarySearchByID(id);
        if (p) department = p->getDepartment();
        else return false;
    }
    
    auto it = products.find(department);
    if (it != products.end()) {
        auto& vec = it->second;
        for (auto vit = vec.begin(); vit != vec.end(); ++vit) {
            if ((*vit)->getId() == id) {
                vec.erase(vit);
                return true;
            }
        }
    }
    return false;
}

bool Warehouse::editProduct(std::string id, double newPrice, std::string department) {
    Product* p = binarySearchByID(id, department);
    if (p) {
        p->setPrice(newPrice);
        return true;
    }
    return false;
}

bool Warehouse::depositStock(std::string id, int amount, std::string username, std::string department) {
    Product* p = binarySearchByID(id, department);
    if (p) {
        p->setQuantity(p->getQuantity() + amount);
        FileManager::saveTransaction(transFile, "DEPOSIT", username, id, amount);
        return true;
    }
    return false;
}

bool Warehouse::withdrawStock(std::string id, int amount, std::string username, std::string department) {
    Product* p = binarySearchByID(id, department);
    if (p && p->getQuantity() >= amount) {
        p->setQuantity(p->getQuantity() - amount);
        FileManager::saveTransaction(transFile, "WITHDRAW", username, id, amount);
        return true;
    }
    return false;
}

void Warehouse::save() {
    FileManager::saveDepartments(departmentsFile, departmentPaths);
    for (const auto& pair : products) {
        std::string filename = "../data/" + departmentPaths[pair.first];
        FileManager::saveProducts(filename, pair.second);
    }
}
