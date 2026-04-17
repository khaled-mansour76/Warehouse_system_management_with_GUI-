#include "Product.h"

Product::Product(std::string id, std::string name, int quantity, double price, int locationX, int locationY, std::string department)
    : id(id), name(name), quantity(quantity), price(price), locationX(locationX), locationY(locationY), department(department) {}

std::string Product::getId() const { return id; }
std::string Product::getName() const { return name; }
int Product::getQuantity() const { return quantity; }
double Product::getPrice() const { return price; }
int Product::getLocationX() const { return locationX; }
int Product::getLocationY() const { return locationY; }
std::string Product::getDepartment() const { return department; }

void Product::setQuantity(int q) { quantity = q; }
void Product::setPrice(double p) { price = p; }
void Product::setLocation(int x, int y) { locationX = x; locationY = y; }
void Product::setDepartment(std::string d) { department = d; }
