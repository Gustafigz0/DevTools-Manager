#pragma once

#include "ProductRepository.hpp"

class CatalogUI {
public:
    explicit CatalogUI(ProductRepository& repo);

    void run();

private:
    ProductRepository& repo_;

    void clearScreen() const;
    void printHeader() const;
    void listProducts() const;
    void addProduct();
    void editProduct();
    void removeProduct();
    void searchProducts();

    static int readInt(const char* prompt);
    static double readDouble(const char* prompt);
    static std::string readLine(const char* prompt);
};
