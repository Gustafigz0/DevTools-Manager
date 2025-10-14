
#include <filesystem>
#include <iostream>
#include "ProductRepository.hpp"
#include "CatalogUI.hpp"




std::filesystem::path encontrar_dados() {
    // Caminho absoluto do executável
    auto exe_path = std::filesystem::absolute(std::filesystem::current_path());
    // Se estiver em .../build/bin, sobe dois níveis para a raiz do projeto
    if (exe_path.filename() == "bin") {
        exe_path = exe_path.parent_path().parent_path();
    } else if (exe_path.filename() == "build") {
        exe_path = exe_path.parent_path();
    }
    std::filesystem::path dados_path = exe_path / "dados";
    std::filesystem::create_directories(dados_path);
    return dados_path;
}




int main() {
    auto dados_dir = encontrar_dados();
    std::filesystem::path json_path = dados_dir / "produtos.json";
    std::cout << "Salvando em: " << json_path << "\n";

    ProductRepository repo(json_path.string());
    repo.load();
    CatalogUI ui(repo);
    ui.run();
    repo.save();
    return 0;
}
