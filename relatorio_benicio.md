# Relatório Semanal de Trabalho - DevTools Manager
**Período:** 3 Semanas  
**Desenvolvedor:** @benirios

---

## Semana 1

### Estrutura Base e Funcionalidades Core

1. **Criação da Estrutura do Projeto**
   - Configuração do projeto Qt com CMakeLists.txt
   - Definição da estrutura de diretórios (scr, include, dados)
   - Configuração do sistema de build com CMake e qmake

2. **Implementação do Modelo de Dados (Product)**
   - Criação da classe `Product` com todos os atributos necessários
   - Implementação de getters e setters
   - Suporte para armazenamento de imagens
   - Sistema de IDs únicos para produtos

3. **Gerenciador de Produtos (ProductManager)**
   - Sistema de gerenciamento CRUD completo
   - Funções de adicionar, atualizar e deletar produtos
   - Sistema de busca por nome, categoria e ID
   - Filtros por categoria e status de estoque
   - Emissão de sinais Qt para atualização da UI

4. **Sistema de Armazenamento Local**
   - Implementação de persistência em JSON
   - Arquivo `produtos.json` para dados dos produtos
   - Carregamento e salvamento automático
   - Sem dependência de internet ou servidor externo

---

## Semana 2

### Interface Principal e Autenticação

1. **Janela Principal (MainWindow)**
   - Criação da interface principal com layout moderno
   - Sidebar com navegação (Produtos, Favoritos)
   - Área principal para exibição de cards de produtos
   - Sistema de busca em tempo real
   - Filtros por categoria e estoque

2. **Sistema de Autenticação (UserAuthDialog)**
   - Tela de login com validação de credenciais
   - Sistema de registro de novos usuários
   - Funcionalidade de alteração de senha
   - Armazenamento seguro com hash criptográfico
   - Arquivo `users.json` para dados de usuários
   - Botão de logout na interface principal

3. **Interface de Produto (AddProductDialog)**
   - Diálogo modal para adicionar novos produtos
   - Modo de edição para produtos existentes
   - Campos para:
     - ID, Nome, Categoria
     - Quantidade em estoque
     - Estoque mínimo
     - Preço por item
     - Localização
     - Notas adicionais
     - Upload de imagem do produto
   - Validação de campos obrigatórios
   - Preview de imagem selecionada

4. **Sistema de Categorias**
   - ComboBox com categorias únicas
   - Sugestão automática de categorias existentes
   - Possibilidade de criar novas categorias

---

## Semana 3

### Funcionalidades Avançadas e UI/UX

1. **Sistema de Favoritos**
   - Marcação de produtos como favoritos (estrela)
   - Filtro para exibir apenas favoritos
   - Persistência do estado de favoritos
   - Ícone visual interativo nos cards

2. **Seleção Múltipla e Exclusão em Lote**
   - Checkbox em cada card de produto
   - Botão "Delete Selected" habilitado dinamicamente
   - Exclusão de múltiplos produtos de uma vez
   - Confirmação antes de deletar

3. **Notificações Toast**
   - Sistema de notificações não-intrusivas (ToastNotification)
   - Animação de fade in/out suave
   - Cores personalizáveis por tipo de operação
   - Feedback visual para ações do usuário

4. **Sistema de Exportação/Importação**
   - Exportação do catálogo completo para JSON
   - Importação de catálogo de arquivo externo
   - Botões na sidebar para fácil acesso
   - Validação de formato durante importação

5. **Cards de Produtos Interativos**
   - Design moderno com bordas arredondadas
   - Exibição de imagem do produto
   - Informações principais visíveis
   - Botões de ação (editar, favoritar, selecionar)
   - Indicador visual de estoque baixo
   - Labels coloridos para diferentes categorias

6. **Sistema de Tema (Theme)**
   - Tema escuro moderno (#1a1a1a background)
   - Paleta de cores consistente
   - Estilização personalizada para todos os componentes
   - Efeitos hover e focus states
   - Design inspirado em Vision-style UI

7. **ClickableLabel**
   - Componente customizado para labels clicáveis
   - Usado para imagens de produtos
   - Eventos de clique integrados

8. **Alertas de Estoque Baixo**
   - Indicação visual quando quantidade < estoque mínimo
   - Cores diferenciadas para alertas
   - Sistema de monitoramento automático

9. **Histórico de Operações**
   - Log interno de operações (adicionar, editar, deletar)
   - Rastreamento de mudanças nos produtos
   - Estrutura para futuro diálogo de histórico

10. **Melhorias de UX**
    - Barra de busca com filtro em tempo real
    - Scroll suave na lista de produtos
    - Estados visuais claros (hover, active, disabled)
    - Mensagens de erro e sucesso amigáveis
    - Atalhos de teclado integrados
    - Responsividade do layout

---

## Resumo de Entregas

### Funcionalidades Implementadas
- ✅ CRUD completo de produtos
- ✅ Sistema de autenticação de usuários
- ✅ Armazenamento local em JSON
- ✅ Upload e armazenamento seguro de imagens
- ✅ Sistema de favoritos
- ✅ Busca e filtros avançados
- ✅ Seleção múltipla e exclusão em lote
- ✅ Exportação/Importação de catálogo
- ✅ Notificações toast
- ✅ Alertas de estoque baixo
- ✅ Interface moderna Vision-style
- ✅ Sistema de categorias dinâmico
- ✅ Rastreamento de preços
- ✅ Gerenciamento de localização

### Tecnologias Utilizadas
- **Framework:** Qt6 (Core, Widgets, Gui)
- **Linguagem:** C++17
- **Build System:** CMake 3.16+
- **Armazenamento:** JSON local
- **Segurança:** QCryptographicHash para senhas
- **UI/UX:** Custom styling com Vision-style design

### Arquivos Principais Desenvolvidos
- `MainWindow.cpp/h` - Interface principal
- `Product.cpp/h` - Modelo de dados
- `ProductManager.cpp/h` - Lógica de negócio
- `AddProductDialog.cpp/h` - Diálogo de produto
- `UserAuthDialog.cpp/h` - Autenticação
- `ToastNotification.cpp/h` - Notificações
- `ClickableLabel.cpp/h` - Componente customizado
- `Theme.cpp/h` - Sistema de tema
- `CMakeLists.txt` - Configuração de build

---

## Observações Finais

O projeto DevTools Manager foi desenvolvido como uma solução **100% offline** para gerenciamento de inventário de TI e licenças de software. Todas as funcionalidades core foram implementadas com foco em privacidade, segurança e usabilidade. A aplicação está pronta para uso em ambientes empresariais que requerem armazenamento local de dados sensíveis.
