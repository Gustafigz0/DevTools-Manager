#pragma once
#include <string>

class Product {
public:
    Product() = default;
    Product(int id,
            std::string name,
            std::string category,
            double price,
            int stock,
            std::string description);

    int id() const;
    const std::string& name() const;
    const std::string& category() const;
    double price() const;
    int stock() const;
    const std::string& description() const;

    void setName(const std::string& v);
    void setCategory(const std::string& v);
    void setPrice(double v);
    void setStock(int v);
    void setDescription(const std::string& v);

    std::string toCSV() const;
    static bool fromCSVLine(const std::string& line, Product& out);

private:
    int id_ = 0;
    std::string name_;
    std::string category_;
    double price_ = 0.0;
    int stock_ = 0;
    std::string description_;
};
