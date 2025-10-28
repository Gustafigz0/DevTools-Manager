#include "CatalogUI.hpp"
#include "Ansi.hpp"
#include <iostream>
#include <iomanip>
#include <limits>


double readDoubleSafe(const char* prompt, double valorPadrao) {
    while (true) {
        std::cout << prompt;
        std::string entrada;
        std::getline(std::cin, entrada);
        if (entrada.empty()) return valorPadrao;
        try {
            size_t pos;
            double val = std::stod(entrada, &pos);
            if (pos != entrada.size()) throw std::invalid_argument("extra chars");
            return val;
        }
        catch (...) {
            std::cout << Ansi::colorize("Valor invalido, tente novamente.\n", Ansi::fg::red);
        }
    }
}

int readIntSafe(const char* prompt, int valorPadrao) {
    while (true) {
        std::cout << prompt;
        std::string entrada;
        std::getline(std::cin, entrada);
        if (entrada.empty()) return valorPadrao;
        try {
            size_t pos;
            int val = std::stoi(entrada, &pos);
            if (pos != entrada.size()) throw std::invalid_argument("extra chars");
            return val;
        }
        catch (...) {
            std::cout << Ansi::colorize("Valor invalido, tente novamente.\n", Ansi::fg::red);
        }
    }
}






// Função auxiliar para linha da tabela
static void printRow(const Product& p) {
    std::cout << std::left
        << std::setw(6) << p.id() << " | "
        << std::setw(20) << p.name() << " | "
        << std::setw(15) << p.category() << " | "
        << std::setw(10) << std::fixed << std::setprecision(2) << p.price() << " | "
        << std::setw(6) << p.stock() << " | "
        << p.description() << "\n";
}

CatalogUI::CatalogUI(ProductRepository& repo) : repo_(repo) {}

void CatalogUI::clearScreen() const {
    std::cout << "\x1b[2J\x1b[H";
}

void CatalogUI::printHeader() const {
    std::cout << Ansi::colorize("== Catalogo de Produtos ==", Ansi::fg::cyan) << "\n\n";
}

void CatalogUI::listProducts() const {
    clearScreen();
    printHeader();
    auto lista = repo_.list();
    if (lista.empty()) {
        std::cout << Ansi::colorize("Nenhum produto cadastrado.\n", Ansi::fg::red) << "\n";
        std::cout << std::string(50, '-') << "\n";
        std::cout << "Pressione Enter para continuar...\n";
        std::cin.get();
        return;
    }
    std::cout << Ansi::colorize(
        "ID    | Nome                | Categoria       | Preco     | Stock | Descricao",
        Ansi::fg::yellow)
        << "\n";
    std::cout << std::string(80, '-') << "\n";
    for (const auto& p : lista) {
        printRow(p);
    }
    std::cout << std::string(80, '-') << "\n\n";
    std::cout << "Pressione Enter para continuar...\n";
    std::cin.get();
}

int CatalogUI::readInt(const char* prompt) {
    while (true) {
        std::cout << prompt;
        int v;
        if (std::cin >> v) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return v;
        }
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << Ansi::colorize("Valor invalido, tente novamente.\n", Ansi::fg::red);
    }
}

double CatalogUI::readDouble(const char* prompt) {
    while (true) {
        std::cout << prompt;
        double v;
        if (std::cin >> v) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return v;
        }
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << Ansi::colorize("Valor invalido, tente novamente.\n", Ansi::fg::red);
    }
}

std::string CatalogUI::readLine(const char* prompt) {
    std::cout << prompt;
    std::string s;
    std::getline(std::cin, s);
    return s;
}

void CatalogUI::addProduct() {
    clearScreen();
    printHeader();
    std::string name = readLine("Nome: ");
    std::string category = readLine("Categoria: ");
    double price = readDoubleSafe("Preco: ", 0.0);
    int stock = readIntSafe("Stock: ", 0);
    std::string description = readLine("Descricao: ");
    
    std::cout << "\n" << std::string(50, '-') << "\n";

    int id = repo_.add(name, category, price, stock, description);
    std::cout << Ansi::colorize("Adicionado com ID ", Ansi::fg::green) << id << "\n";

    std::cout << std::string(50, '-') << "\n";
    std::cout << "Pressione Enter para continuar...\n";
    std::cin.get();
}

void CatalogUI::editProduct() {
    clearScreen();
    printHeader();
    while (true) {
        std::cout << "1) Editar produto pelo ID\n";
        std::cout << "2) Ver todos os produtos\n";
        std::cout << "0) Sair\n";
        int escolha = readInt("Escolha: ");
        if (escolha == 0)
            return;
        else if (escolha == 2) {
            // Mostra todos antes de editar
            listProducts();
        } else if (escolha == 1) {
            int id = readInt("ID do produto a editar (ou 0 para cancelar): ");
            if (id == 0) return;

            std::cout << "\n" << std::string(50, '-') << "\n";
            auto maybe = repo_.findById(id);
            if (!maybe) {
                std::cout << Ansi::colorize("Produto nao encontrado.\n", Ansi::fg::red);
            } else {
                std::cout << "Deixe em branco para manter o valor atual.\n";
                std::string name = readLine(("Nome [" + maybe->name() + "]: ").c_str());
                if (name.empty()) name = maybe->name();
                std::string category = readLine(("Categoria [" + maybe->category() + "]: ").c_str());
                if (category.empty()) category = maybe->category();
                double price = readDoubleSafe(("Preco [" + std::to_string(maybe->price()) + "]: ").c_str(), maybe->price());
                int stock = readIntSafe(("Stock [" + std::to_string(maybe->stock()) + "]: ").c_str(), maybe->stock());
                std::string description = readLine(("Descricao [" + maybe->description() + "]: ").c_str());
                if (description.empty()) description = maybe->description();
                std::cout << "\n" << std::string(50, '-') << "\n";
                if (repo_.update(id, name, category, price, stock, description))
                    std::cout << Ansi::colorize("Atualizado.\n", Ansi::fg::green);
                else
                    std::cout << Ansi::colorize("Falha ao atualizar.\n", Ansi::fg::red);
            }
            std::cout << std::string(50, '-') << "\n";
            std::cout << "Pressione Enter para continuar...\n";
            std::cin.get();
            return; // Sai depois de editar um produto
        }
    }
}


void CatalogUI::removeProduct() {
    clearScreen();
    printHeader();

    while (true) {
        std::cout << "0) Sair\n";
        std::cout << "1) Remover produto pelo ID\n";
        std::cout << "2) Ver todos os produtos\n";

        int escolha = readInt("Escolha: ");
        if (escolha == 0)
            return;
        else if (escolha == 2) {
            // Mostra todos antes de remover
            listProducts();
        } else if (escolha == 1) {
            int id = readInt("Digite o ID do produto a remover (ou 0 para cancelar): ");
            if (id == 0) return;
            std::cout << "\n" << std::string(50, '-') << "\n";
            if (repo_.remove(id))
                std::cout << Ansi::colorize("Removido.\n", Ansi::fg::green);
            else
                std::cout << Ansi::colorize("Produto nao encontrado.\n", Ansi::fg::red);
            std::cout << std::string(50, '-') << "\n";
            std::cout << "Pressione Enter para continuar...\n";
            std::cin.get();
            return; // Sai depois de remover um produto
        }
    }
}



void CatalogUI::searchProducts() {
    clearScreen();
    printHeader();
    std::cout << "0) Sair\n";
    std::cout << "1) Pesquisar por nome\n";
    std::cout << "2) Pesquisar por categoria\n";
    int op = readInt("Opcao: ");
    std::string term = readLine("Termo: ");

    std::cout << "\n" << std::string(50, '-') << "\n";

    std::vector<Product> results;
    if (op == 1) results = repo_.searchByName(term);
    else results = repo_.searchByCategory(term);

    std::cout << Ansi::colorize("Resultados", Ansi::fg::yellow) << "\n";
    std::cout << std::string(80, '-') << "\n";
    for (const auto& p : results) printRow(p);
    if (results.empty())
        std::cout << Ansi::colorize("Nenhum resultado encontrado.", Ansi::fg::red) << "\n";
    std::cout << std::string(50, '-') << "\n";
    std::cout << "Pressione Enter para continuar...\n";
    std::cin.get();
}

void CatalogUI::run() {
    while (true) {
        clearScreen();
        printHeader();
        std::cout << Ansi::colorize("1) Listar", Ansi::fg::green) << "  ";
        std::cout << Ansi::colorize("2) Adicionar", Ansi::fg::green) << "  ";
        std::cout << Ansi::colorize("3) Editar", Ansi::fg::green) << "  ";
        std::cout << Ansi::colorize("4) Remover", Ansi::fg::green) << "  ";
        std::cout << Ansi::colorize("5) Pesquisar", Ansi::fg::green) << "  ";
        std::cout << Ansi::colorize("0) Sair", Ansi::fg::red) << "\n\n";
        int op = readInt("Escolha: ");
        switch (op) {
            case 1: listProducts(); break;
            case 2: addProduct(); break;
            case 3: editProduct(); break;
            case 4: removeProduct(); break;
            case 5: searchProducts(); break;
            case 0: return;
            default: std::cout << Ansi::colorize("Opcao invalida.\n", Ansi::fg::red); break;
        }
    }
}
