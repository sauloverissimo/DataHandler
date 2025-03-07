# DataHandler - Manipulando dados em C++ para microcontroladores.
Nesta seção, apresentamos uma **análise minuciosa** das principais **estruturas**, **métodos** e **funções** fornecidas pelo **DataHandler**. A ideia é descrever cada recurso, **entrada e saída**, bem como o **contexto de uso** de cada um.

---

## 1. Tipos de Dados Fundamentais

### 1.1 **TypeElement**

```cpp
using TypeElement = std::variant<int, double, float, std::string, std::vector<std::string>>;
```

- **Significado**: `TypeElement` é um alias para `std::variant<...>`, permitindo armazenar em uma única variável vários tipos de dados (por exemplo, `int`, `double`, `std::string`, etc.).  
- **Entradas**: qualquer dos tipos listados (`int`, `double`, `float`, `std::string`, `std::vector<std::string>`).  
- **Saídas**: a leitura do dado armazenado exige verificação do tipo (via `std::holds_alternative` ou `std::get`).

**Uso Típico**:
```cpp
TypeElement elem = 42;              // Armazena um int
TypeElement elem2 = 3.14;           // Armazena um double
TypeElement elem3 = std::string("Texto");
TypeElement elem4 = std::vector<std::string>{"A", "B"};
```

---

### 1.2 **TypeVector**

```cpp
using TypeVector = std::vector<TypeElement>;
```

- **Significado**: Um `TypeVector` é um `std::vector` cujo elemento interno é o `TypeElement`. Dessa forma, cada posição desse vetor pode ter um tipo diferente de valor.  
- **Entradas**: qualquer coleção de elementos do tipo `TypeElement`.  
- **Saídas**: ao acessar um índice do `TypeVector`, obtemos um `TypeElement`.

**Uso Típico**:
```cpp
TypeVector linha = { 42, "Bob", 3.14f, std::vector<std::string>{"X", "Y"} };
// Assim, linha[0] é int, linha[1] é string, etc.
```

---

### 1.3 **TypeTable**

```cpp
using TypeTable = std::vector<TypeVector>;
```

- **Significado**: Um `TypeTable` é um `std::vector` de `TypeVector`. Podemos imaginar isso como uma **tabela** ou **planilha**, onde cada "linha" é um `TypeVector`, e cada célula é um `TypeElement`.  
- **Entradas**: Coleção de linhas (`TypeVector`) que formarão a tabela.  
- **Saídas**: Cada linha pode ser acessada por índice, resultando num `TypeVector`. A intersecção linha-coluna é um `TypeElement`.

**Uso Típico**:
```cpp
TypeTable tabela = {
    { "Nome", 25, "Cidade" },
    { "Bob", 30,  "LA" },
};
```

---

### 1.4 **TypeCube**

```cpp
using TypeCube = std::vector<TypeTable>;
```

- **Significado**: Um `TypeCube` é um vetor de `TypeTable`. Pode ser visto como um **cubo de dados** (ex.: planilhas em 3 dimensões) ou uma **coleção de tabelas**.  
- **Uso Típico**: manipulação mais avançada de dados, quando precisamos agrupar várias tabelas relacionadas ou armazenar dados em estrutura multidimensional.

---

## 2. Estruturas Principais

### 2.1 **struct Element**

```cpp
struct Element {
    TypeElement value;
    // Construtores
    Element();
    Element(const TypeElement& v);
    Element(const std::string& str);
    Element(const std::vector<std::string>& vec);

    operator const TypeElement&() const;
    std::string operator[](size_t index) const;
    bool isVector() const;
    std::vector<std::string> getVector() const;
};
```

**Resumo de cada membro**:
1. **`value`**: armazena o dado interno como um `TypeElement`.
2. **Construtores**:
   - `Element() : value("") {}` inicializa como string vazia.
   - `Element(const TypeElement&)`: permite construir a partir de qualquer variante válida de `TypeElement`.
   - `Element(const std::string&)`: converte diretamente de string.
   - `Element(const std::vector<std::string>&)`: converte de vetor de strings.
3. **`operator const TypeElement&() const`**: conversão implícita para `TypeElement`, simplificando manipulação.
4. **`operator[](size_t index) const`**: se o `Element` contém um `std::vector<std::string>`, permite acessar um índice específico dessa lista. Caso contrário, exibe erro.
5. **`isVector()`**: retorna `true` se `value` é `std::vector<std::string>`.
6. **`getVector()`**: retorna o `std::vector<std::string>` contido em `value`, se existir.

**Entradas/Saídas**:
- Entradas: A construção aceita strings, vetores de strings e, de modo genérico, qualquer `TypeElement`.
- Saídas: `operator[]` devolve uma string; `isVector()` devolve booleano; `getVector()` retorna `std::vector<std::string>`.

---

### 2.2 **struct Vector**

```cpp
struct Vector {
    TypeVector values;
    Vector();
    Vector(TypeVector v);
    Element operator[](size_t index);
    std::string operator()(size_t index, size_t subIndex);
    operator TypeVector&();
    operator const TypeVector&() const;
};
```

**Resumo de cada membro**:
1. **`values`**: o `Vector` interno que armazena `TypeVector`.
2. **Construtores**:
   - `Vector()` construtor padrão.
   - `Vector(TypeVector v)` inicializa com um `TypeVector`.
3. **`operator[](size_t index)`**:
   - Retorna um `Element` (e não um `TypeElement` diretamente), permitindo acesso e conversão posterior.
4. **`operator()(size_t index, size_t subIndex)`**:
   - Acessa subelementos. Se `values[index]` for `std::vector<std::string>`, retorna a string naquela posição.
5. **Conversões para `TypeVector`**:
   - `operator TypeVector&()` e `operator const TypeVector&()` permitem que o `Vector` seja tratado como um `TypeVector`.

**Entradas/Saídas**:
- Entradas: normalmente construímos `Vector` a partir de um `TypeVector`.
- Saídas: `operator[]` devolve `Element`; `operator()` devolve `std::string`.

---

### 2.3 **struct Table**

```cpp
struct Table {
    TypeTable data;
    std::vector<std::string> columnNames;
    std::unordered_map<std::string, int> columnIndex;
    std::unordered_map<std::string, int> rowNameToIndex;

    Table();
    Table(TypeTable v, std::vector<std::string> colNames = {});

    Vector row(int rowIndex) const;
    Vector row(const std::string& rowName, const std::string& by = "rowNameToIndex") const;
    Vector column(int colIndex) const;
    Vector column(const std::string& colName) const;
    Vector operator[](size_t rowIndex) const;
    void addRow(const TypeVector& row);
};
```

**Resumo de cada membro**:
1. **`data`**: armazena as linhas da tabela (um `TypeTable`).
2. **`columnNames`**: nomes das colunas, opcional.
3. **`columnIndex`**: mapeamento NomeDaColuna -> Índice.
4. **`rowNameToIndex`**: mapeamento NomeDaLinha -> Índice, facilitando busca.
5. **Construtores**:
   - `Table()`: construtor padrão.
   - `Table(TypeTable v, std::vector<std::string> colNames)`: inicializa `data` e nomes de colunas.
6. **Métodos Principais**:
   - `row(int rowIndex)`: retorna um `Vector` para a linha.
   - `row(const std::string& rowName, const std::string& by)`: retorna linha baseada em uma chave, procurando `rowNameToIndex` ou numa coluna específica.
   - `column(int colIndex)`: retorna `Vector` (coluna) pelo índice.
   - `column(const std::string& colName)`: retorna `Vector` (coluna) pelo nome.
   - `operator[](size_t rowIndex)`: forma rápida de acessar a linha `rowIndex`.
   - `addRow(const TypeVector&)`: adiciona uma nova linha ao final da tabela.

**Entradas/Saídas**:
- Entradas: cada linha deve ser `TypeVector` coerente com as colunas definidas.
- Saídas: busca de linha/coluna retorna `Vector`.

---

## 3. Funções Auxiliares

### 3.1 **`template <typename T, size_t N> TypeVector ToVector(const T (&arr)[N])`**

**Definição**:
```cpp
template <typename T, size_t N>
TypeVector ToVector(const T (&arr)[N]) {
    // ...
}
```

- **Objetivo**: Converte um array C++ estático (`T arr[N]`) em um `TypeVector`.  
- **Entrada**: `arr` - array de tamanho fixo do tipo `T`.  
- **Saída**: Um `TypeVector` onde cada elemento é convertido em `TypeElement`.

**Exemplo de Uso**:
```cpp
int arrInt[] = {1, 2, 3};
TypeVector tv = ToVector(arrInt);
// tv = {1, 2, 3} (cada um como TypeElement)
```

---

### 3.2 **`TypeTable spin(const TypeVector& arr)`**

- **Objetivo**: Gera uma tabela (TypeTable) em que cada linha é uma **rotação** do vetor base.  
  - Cria N linhas e N colunas, onde `N = arr.size()`.  
  - A linha i desloca os elementos em i posições.
- **Entrada**: `arr` - um `TypeVector`.  
- **Saída**: `TypeTable` quadrada.

**Exemplo**:
```cpp
TypeVector arr = {"A", "B", "C"};
TypeTable sp = spin(arr);
// sp[0] = {"A", "B", "C"}
// sp[1] = {"B", "C", "A"}
// sp[2] = {"C", "A", "B"}
```

### 3.3 **`TypeVector spin(const TypeVector& arr, size_t idx)`**

- **Objetivo**: Gera **apenas uma linha** correspondente à rotação do vetor.  
- **Entrada**: `arr` - vetor original; `idx` - quantidade de deslocamento.  
- **Saída**: um `TypeVector` rotacionado.

**Exemplo**:
```cpp
TypeVector rotated = spin({"A", "B", "C"}, 1);
// rotated = {"B", "C", "A"}
```

---

### 3.4 **`TypeTable repeatarray(const TypeVector& arr)`**

- **Objetivo**: Cria uma tabela onde **todas as linhas** são idênticas a `arr`.  
- **Entrada**: `arr`.  
- **Saída**: `TypeTable` com `arr.size()` linhas e cada linha = `arr`.

**Exemplo**:
```cpp
TypeVector arr = {1, 2, 3};
TypeTable rep = repeatarray(arr);
// rep.size() = 3
// rep[0] = {1, 2, 3}
// rep[1] = {1, 2, 3}
// rep[2] = {1, 2, 3}
```

---

### 3.5 **`TypeTable spread(const TypeVector& arr)`**

- **Objetivo**: Gera uma tabela em que cada coluna (para cada linha) contém o mesmo elemento.  
- **Entrada**: `arr`.  
- **Saída**: `TypeTable` de tamanho `arr.size()`, onde cada linha é um `TypeVector` contendo repetição do elemento.

**Exemplo**:
```cpp
TypeVector arr = {"X", "Y", "Z"};
TypeTable spd = spread(arr);
// spd.size() = 3
// Cada spd[i] terá 3 elementos, todos = arr[i]
```

### 3.6 **`TypeVector spread(const TypeVector& arr, size_t idx)`** (Sobrecarga)

- **Objetivo**: Retorna um `TypeVector` no qual *todos os elementos* são iguais a `arr[idx]`.  
- **Entrada**: `arr`, e `idx` - índice do elemento a repetir.  
- **Saída**: `TypeVector` onde cada posição é `arr[idx]`.

**Exemplo**:
```cpp
TypeVector arr = {"X", "Y", "Z"};
TypeVector sp1 = spread(arr, 0);
// sp1 = {"X", "X", "X"}
```

---

### 3.7 **`TypeVector rotate(const TypeVector& vec, const TypeElement& elem, size_t n = 0, const std::string& mode = "value")`**

- **Objetivo**: Rotaciona um vetor com base em um **elemento de referência** ou em um índice.  
- **Parâmetros**:
  - `vec`: vetor original.
  - `elem`: se `mode == "value"`, procura `elem` em `vec` e usa como ponto de rotação; se `mode == "idx"`, trata `elem` como índice inteiro.
  - `n`: tamanho da rotação (default = `vec.size()`).
  - `mode`: modo de interpretação de `elem` (`"value"` ou `"idx"`).
- **Saída**: Vetor `TypeVector` rotacionado.

**Exemplo**:
```cpp
TypeVector base = {"C", "D", "E", "F"};
// Modo "value"
TypeVector rotated = rotate(base, "E");
// Iniciará a partir de "E"
```

---

### 3.8 **`std::string RowNameByIndex(const std::unordered_map<std::string, int>& rowNameToIndex, int targetIndex)`**

- **Objetivo**: Faz busca reversa no mapa `rowNameToIndex` (nome -> índice) para obter a **chave** dado um **valor**.  
- **Entrada**:
  - `rowNameToIndex`: dicionário com mapeamento.
  - `targetIndex`: índice que desejamos achar o nome.
- **Saída**: a `std::string` correspondente ao `targetIndex`. Se não encontrar, retorna `""`.

**Exemplo**:
```cpp
std::unordered_map<std::string, int> rmap = {
    {"Alice", 0},
    {"Bob", 1}
};
std::string name = RowNameByIndex(rmap, 1);
// name = "Bob"
```

---

### 3.9 **`const char* intOrString(const TypeElement& value)`**

- **Objetivo**: Verifica se o `TypeElement` é `int` ou `std::string` e retorna a string correspondente (`"int"`, `"string"` ou `"unknown"`).  
- **Entrada**: `value` - um `TypeElement`.
- **Saída**: `"int"`, `"string"` ou `"unknown"`.

**Exemplo**:
```cpp
TypeElement e = 10;
const char* result = intOrString(e);
// result = "int"
```

---

## 4. Resumo Geral de Aplicações

1. **Construção de Tabelas**: Podemos usar `Table` para criar estruturas parecidas com planilhas, com nomes de colunas e índices de linha.  
2. **Manipulação de Dados**: As funções `spin`, `spread`, `repeatarray` e `rotate` permitem várias transformações de vetores e tabelas para fins musicais, estatísticos, etc.  
3. **Indexação Nomeada**: A `columnIndex` e `rowNameToIndex` facilitam a busca por nomes em vez de números.
4. **Flexibilidade de Tipos**: `TypeElement` e as estruturas derivadas possibilitam manipular dados heterogêneos.

---

## Exemplos de Uso Adicionais

### Exemplo - Construindo e Rotacionando Notas
```cpp
// Imaginando um vetor de notas:
TypeVector notas = {"C", "D", "E", "F", "G", "A", "B"};
// Rotacionar esse vetor para iniciar em "E":
TypeVector rotated = rotate(notas, "E");
// rotated: {"E", "F", "G", "A", "B", "C", "D"}
```

### Exemplo - Convertendo Array Fixo
```cpp
int valores[] = {10, 20, 30};
TypeVector tv = ToVector(valores);
// tv[0] = 10 (como TypeElement)
```

### Exemplo - Tabela com Acesso Nomeado
```cpp
Table pessoas({}, {"Nome", "Idade", "Profissao"});
pessoas.addRow({"Alice", 25, "Engenheira"});
pessoas.addRow({"Bob", 30, "Músico"});

// Acessar colunas:
Vector nomes = pessoas.column("Nome"); // Vector de TypeElement com ["Alice", "Bob"]

// Acessar por linha e índice:
Vector bob = pessoas.row(1);
Serial.println(bob[0]); // "Bob"
Serial.println(bob[2]); // "Músico"
```

---

## Conclusão

A biblioteca **DataHandler** oferece **estruturas** (Element, Vector, Table) que facilitam o manuseio de dados heterogêneos, juntamente com um conjunto robusto de **funções auxiliares** (como `spin`, `rotate`, `spread`, etc.). Cada função possui diversas formas de uso, dependendo de como se deseja **transformar**, **indexar** ou **consultar** os dados.

Ficou alguma dúvida ou quer mais detalhes de implementação? Consulte o repositório no GitHub ou abra uma issue! 🏆✨
