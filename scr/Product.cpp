#include "Product.hpp"
#include "CSV.hpp"
#include <sstream>


Product::Product(int id, std::string name, std::string category,
                 double price, int stock, std::string description)
    : id_(id), name_(std::move(name)), category_(std::move(category)),
      price_(price), stock_(stock), description_(std::move(description)) {}

int Product::id() const { return id_; }
const std::string& Product::name() const { return name_; }
const std::string& Product::category() const { return category_; }
double Product::price() const { return price_; }
int Product::stock() const { return stock_; }
const std::string& Product::description() const { return description_; }

void Product::setName(const std::string& v) { name_ = v; }
void Product::setCategory(const std::string& v) { category_ = v; }
void Product::setPrice(double v) { price_ = v; }
void Product::setStock(int v) { stock_ = v; }
void Product::setDescription(const std::string& v) { description_ = v; }

std::string Product::toCSV() const {
    std::ostringstream oss;
    oss << id_ << ','
        << CSV::escape(name_) << ','
        << CSV::escape(category_) << ','
        << price_ << ','
        << stock_ << ','
        << CSV::escape(description_);
    return oss.str();
}

bool Product::fromCSVLine(const std::string& line, Product& out) {
    auto fields = CSV::splitLine(line);
    if (fields.size() < 6) return false;
    try {
        int id = std::stoi(fields[0]);
        std::string name = fields[1];
        std::string category = fields[2];
        double price = std::stod(fields[3]);
        int stock = std::stoi(fields[4]);
        std::string description = fields[5];
        out = Product(id, name, category, price, stock, description);
        return true;
    } catch (...) {
        return false;
    }
}
