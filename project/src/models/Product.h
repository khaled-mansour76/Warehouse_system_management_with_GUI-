#ifndef PRODUCT_H
#define PRODUCT_H

#include <string>

class Product {
private:
    std::string id;
    std::string name;
    int quantity;
    double price;
    int locationX;
    int locationY;
    std::string department;

public:
    Product(std::string id, std::string name, int quantity, double price, int locationX, int locationY, std::string department);
    
    std::string getId() const;
    std::string getName() const;
    int getQuantity() const;
    double getPrice() const;
    int getLocationX() const;
    int getLocationY() const;
    std::string getDepartment() const;

    void setQuantity(int quantity);
    void setPrice(double price);
    void setLocation(int x, int y);
    void setDepartment(std::string dept);
};

#endif
