#include "ProductRepository.hpp"
#include <fstream>
#include <algorithm>

ProductRepository::ProductRepository(std::string filePath)
    : filePath_(std::move(filePath)) {}

bool ProductRepository::load() {
    products_.clear();
    std::ifstream in(filePath_);
    if (!in) {
        nextId_ = 1;
        return true; // Sem arquivo ainda: repositório vazio
    }
    std::string line;
    while (std::getline(in, line)) {
        if (line.empty()) continue;
        Product p;
        if (Product::fromCSVLine(line, p)) {
            products_.push_back(p);
        }
    }
    recomputeNextId();
    return true;
}

bool ProductRepository::save() const {
    std::ofstream out(filePath_, std::ios::trunc);
    if (!out) return false;
    for (const auto& p : products_) {
        out << p.toCSV() << "\n";
    }
    return true;
}

const std::vector<Product>& ProductRepository::list() const {
    return products_;
}

std::optional<Product> ProductRepository::findById(int id) const {
    auto it = std::find_if(products_.begin(), products_.end(),
        [id](const Product& p){ return p.id() == id; });
    if (it == products_.end()) return std::nullopt;
    return *it;
}

int ProductRepository::add(std::string name, std::string category, double price, int stock, std::string description) {
    int id = nextId_++;
    products_.emplace_back(id, std::move(name), std::move(category), price, stock, std::move(description));
    return id;
}

bool ProductRepository::update(int id, std::string name, std::string category, double price, int stock, std::string description) {
    for (auto& p : products_) {
        if (p.id() == id) {
            p.setName(name);
            p.setCategory(category);
            p.setPrice(price);
            p.setStock(stock);
            p.setDescription(description);
            return true;
        }
    }
    return false;
}

bool ProductRepository::remove(int id) {
    auto oldSize = products_.size();
    products_.erase(std::remove_if(products_.begin(), products_.end(),
        [id](const Product& p){ return p.id() == id; }), products_.end());
    return products_.size() != oldSize;
}

std::vector<Product> ProductRepository::searchByName(const std::string& term) const {
    std::vector<Product> out;
    for (const auto& p : products_) {
        if (p.name().find(term) != std::string::npos) out.push_back(p);
    }
    return out;
}

std::vector<Product> ProductRepository::searchByCategory(const std::string& term) const {
    std::vector<Product> out;
    for (const auto& p : products_) {
        if (p.category().find(term) != std::string::npos) out.push_back(p);
    }
    return out;
}

void ProductRepository::recomputeNextId() {
    int maxId = 0;
    for (const auto& p : products_) if (p.id() > maxId) maxId = p.id();
    nextId_ = maxId + 1;
}
