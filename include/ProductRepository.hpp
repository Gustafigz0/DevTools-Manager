#pragma once
#include "Product.hpp"
#include <string>
#include <vector>
#include <optional>

class ProductRepository {
public:
    explicit ProductRepository(std::string filePath);

    bool load();
    bool save() const;

    const std::vector<Product>& list() const;
    std::optional<Product> findById(int id) const;

    int add(std::string name, std::string category, double price, int stock, std::string description);
    bool update(int id, std::string name, std::string category, double price, int stock, std::string description);
    bool remove(int id);

    std::vector<Product> searchByName(const std::string& term) const;
    std::vector<Product> searchByCategory(const std::string& term) const;

private:
    int nextId_ = 1;
    std::string filePath_;
    std::vector<Product> products_;

    void recomputeNextId();
};
