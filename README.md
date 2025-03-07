EN 🔴
# DataHandler – Details of Structures and Functions

In this section, we present a **thorough analysis** of the main **structures**, **methods**, and **functions** provided by **DataHandler**. The goal is to describe each feature, **input and output**, as well as the **usage context** of each.

---

## 1. Fundamental Data Types

### 1.1 **TypeElement**

```cpp
using TypeElement = std::variant<int, double, float, std::string, std::vector<std::string>>;
```

- **Meaning**: `TypeElement` is an alias for `std::variant<...>`, allowing multiple data types (e.g., `int`, `double`, `std::string`, etc.) to be stored in a single variable.  
- **Inputs**: any of the listed types (`int`, `double`, `float`, `std::string`, `std::vector<std::string>`).  
- **Outputs**: reading the stored data requires a type check (via `std::holds_alternative` or `std::get`).

**Typical Use**:
```cpp
TypeElement elem = 42;                // Stores an int
TypeElement elem2 = 3.14;             // Stores a double
TypeElement elem3 = std::string("Text");
TypeElement elem4 = std::vector<std::string>{"A", "B"};
```

---

### 1.2 **TypeVector**

```cpp
using TypeVector = std::vector<TypeElement>;
```

- **Meaning**: A `TypeVector` is a `std::vector` whose internal element is `TypeElement`. Thus, each position of this vector can hold a different type of value.  
- **Inputs**: any collection of `TypeElement`.  
- **Outputs**: accessing an index of `TypeVector` yields a `TypeElement`.

**Typical Use**:
```cpp
TypeVector row = { 42, "Bob", 3.14f, std::vector<std::string>{"X", "Y"} };
// row[0] is int, row[1] is string, etc.
```

---

### 1.3 **TypeTable**

```cpp
using TypeTable = std::vector<TypeVector>;
```

- **Meaning**: A `TypeTable` is a `std::vector` of `TypeVector`. We can think of this as a **table** or **spreadsheet**, where each "row" is a `TypeVector`, and each cell is a `TypeElement`.  
- **Inputs**: a collection of rows (`TypeVector`) that will form the table.  
- **Outputs**: each row can be accessed by index, producing a `TypeVector`. The intersection row-column is a `TypeElement`.

**Typical Use**:
```cpp
TypeTable table = {
    { "Name", 25, "City" },
    { "Bob", 30,  "LA" },
};
```

---

### 1.4 **TypeCube**

```cpp
using TypeCube = std::vector<TypeTable>;
```

- **Meaning**: A `TypeCube` is a vector of `TypeTable`. It can be viewed as a **data cube** (e.g., 3D spreadsheets) or a **collection of tables**.  
- **Typical Use**: more advanced data manipulation, where we need to group multiple related tables or store data in a multidimensional structure.

---

## 2. Main Structures

### 2.1 **struct Element**

```cpp
struct Element {
    TypeElement value;
    // Constructors
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

**Summary of each member**:
1. **`value`**: holds the internal data as a `TypeElement`.
2. **Constructors**:
   - `Element() : value("") {}` initializes it as an empty string.
   - `Element(const TypeElement&)`: constructs from any valid variant of `TypeElement`.
   - `Element(const std::string&)`: directly converts from string.
   - `Element(const std::vector<std::string>&)`: converts from a vector of strings.
3. **`operator const TypeElement&() const`**: implicit conversion to `TypeElement`, simplifying usage.
4. **`operator[](size_t index) const`**: if the `Element` holds a `std::vector<std::string>`, allows accessing a specific index of that list. Otherwise, prints an error.
5. **`isVector()`**: returns `true` if `value` is `std::vector<std::string>`.
6. **`getVector()`**: returns the `std::vector<std::string>` stored in `value`, if present.

**Inputs/Outputs**:
- Inputs: can construct with strings, vectors of strings, and more generally any `TypeElement`.
- Outputs: `operator[]` returns a string; `isVector()` returns a boolean; `getVector()` returns a `std::vector<std::string>`.

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

**Summary of each member**:
1. **`values`**: the internal `Vector` that stores a `TypeVector`.
2. **Constructors**:
   - `Vector()` default constructor.
   - `Vector(TypeVector v)` initializes with a `TypeVector`.
3. **`operator[](size_t index)`**:
   - Returns an `Element` (not a `TypeElement` directly), allowing further access or conversion.
4. **`operator()(size_t index, size_t subIndex)`**:
   - Accesses sub-elements. If `values[index]` is `std::vector<std::string>`, returns the string at that position.
5. **Conversions to `TypeVector`**:
   - `operator TypeVector&()` and `operator const TypeVector&()` allow the `Vector` to be treated as a `TypeVector`.

**Inputs/Outputs**:
- Inputs: typically, you build a `Vector` from a `TypeVector`.
- Outputs: `operator[]` returns an `Element`; `operator()` returns a `std::string`.

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

**Summary of each member**:
1. **`data`**: stores the table rows (a `TypeTable`).
2. **`columnNames`**: optional list of column names.
3. **`columnIndex`**: mapping from ColumnName -> Index.
4. **`rowNameToIndex`**: mapping from RowName -> Index, helping with lookups.
5. **Constructors**:
   - `Table()`: default constructor.
   - `Table(TypeTable v, std::vector<std::string> colNames)`: initializes `data` and column names.
6. **Main Methods**:
   - `row(int rowIndex)`: returns a `Vector` for that row.
   - `row(const std::string& rowName, const std::string& by)`: returns a row based on a key, searching either `rowNameToIndex` or a specific column.
   - `column(int colIndex)`: returns a `Vector` (the column) by index.
   - `column(const std::string& colName)`: returns a `Vector` (the column) by name.
   - `operator[](size_t rowIndex)`: quick way to access row `rowIndex`.
   - `addRow(const TypeVector&)`: appends a new row to the end of the table.

**Inputs/Outputs**:
- Inputs: each row must be a `TypeVector` consistent with the defined columns.
- Outputs: row/column lookup returns a `Vector`.

---

## 3. Auxiliary Functions

### 3.1 **`template <typename T, size_t N> TypeVector ToVector(const T (&arr)[N])`**

**Definition**:
```cpp
template <typename T, size_t N>
TypeVector ToVector(const T (&arr)[N]) {
    // ...
}
```

- **Purpose**: Converts a static C++ array (`T arr[N]`) into a `TypeVector`.  
- **Input**: `arr` – a fixed-size array of type `T`.  
- **Output**: a `TypeVector` where each element is converted to `TypeElement`.

**Usage Example**:
```cpp
int arrInt[] = {1, 2, 3};
TypeVector tv = ToVector(arrInt);
// tv = {1, 2, 3} (each as TypeElement)
```

---

### 3.2 **`TypeTable spin(const TypeVector& arr)`**

- **Purpose**: Generates a table (`TypeTable`) where each row is a **rotation** of the base vector.  
  - Creates N rows and N columns, with `N = arr.size()`.  
  - Row i shifts elements by i positions.
- **Input**: `arr` – a `TypeVector`.  
- **Output**: a square `TypeTable`.

**Example**:
```cpp
TypeVector arr = {"A", "B", "C"};
TypeTable sp = spin(arr);
// sp[0] = {"A", "B", "C"}
// sp[1] = {"B", "C", "A"}
// sp[2] = {"C", "A", "B"}
```

### 3.3 **`TypeVector spin(const TypeVector& arr, size_t idx)`**

- **Purpose**: Generates **just one row** corresponding to the vector’s rotation.  
- **Input**: `arr` – original vector; `idx` – shift quantity.  
- **Output**: a rotated `TypeVector`.

**Example**:
```cpp
TypeVector rotated = spin({"A", "B", "C"}, 1);
// rotated = {"B", "C", "A"}
```

---

### 3.4 **`TypeTable repeatarray(const TypeVector& arr)`**

- **Purpose**: Creates a table where **all rows** are identical to `arr`.  
- **Input**: `arr`.  
- **Output**: a `TypeTable` with `arr.size()` rows, each row = `arr`.

**Example**:
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

- **Purpose**: Generates a table where each column (in each row) contains the same element.  
- **Input**: `arr`.  
- **Output**: a `TypeTable` of size `arr.size()`, where each row is a `TypeVector` containing repetitions of a single element.

**Example**:
```cpp
TypeVector arr = {"X", "Y", "Z"};
TypeTable spd = spread(arr);
// spd.size() = 3
// Each spd[i] has 3 elements, all = arr[i]
```

### 3.6 **`TypeVector spread(const TypeVector& arr, size_t idx)`** (Overload)

- **Purpose**: Returns a `TypeVector` in which *all elements* are equal to `arr[idx]`.  
- **Input**: `arr`, and `idx` – index of the element to repeat.  
- **Output**: a `TypeVector` where every position is `arr[idx]`.

**Example**:
```cpp
TypeVector arr = {"X", "Y", "Z"};
TypeVector sp1 = spread(arr, 0);
// sp1 = {"X", "X", "X"}
```

---

### 3.7 **`TypeVector rotate(const TypeVector& vec, const TypeElement& elem, size_t n = 0, const std::string& mode = "value")`**

- **Purpose**: Rotates a vector based on a **reference element** or an index.  
- **Parameters**:
  - `vec`: original vector.
  - `elem`: if `mode == "value"`, searches `elem` in `vec` and uses it as the rotation anchor; if `mode == "idx"`, treats `elem` as an integer index.
  - `n`: the rotation size (default = `vec.size()`).
  - `mode`: how to interpret `elem` (`"value"` or `"idx"`).
- **Output**: a rotated `TypeVector`.

**Example**:
```cpp
TypeVector base = {"C", "D", "E", "F"};
// "value" mode
TypeVector rotated = rotate(base, "E");
// Starts from "E"
```

---

### 3.8 **`std::string RowNameByIndex(const std::unordered_map<std::string, int>& rowNameToIndex, int targetIndex)`**

- **Purpose**: Performs a reverse lookup in `rowNameToIndex` (name -> index) to get the **key** given a **value**.  
- **Input**:
  - `rowNameToIndex`: mapping.
  - `targetIndex`: the index whose name we want.
- **Output**: the `std::string` corresponding to `targetIndex`. If not found, returns `""`.

**Example**:
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

- **Purpose**: Checks whether `TypeElement` is an `int` or a `std::string` and returns the corresponding string (`"int"`, `"string"`, or `"unknown"`).  
- **Input**: `value` – a `TypeElement`.
- **Output**: `"int"`, `"string"`, or `"unknown"`.

**Example**:
```cpp
TypeElement e = 10;
const char* result = intOrString(e);
// result = "int"
```

---

## 4. General Application Summary

1. **Table Construction**: We can use `Table` to create spreadsheet-like structures, with column names and row indices.  
2. **Data Manipulation**: Functions like `spin`, `spread`, `repeatarray`, and `rotate` provide various transformations of vectors and tables for musical, statistical, or other purposes.  
3. **Named Indexing**: `columnIndex` and `rowNameToIndex` simplify lookups by names instead of numeric indices.
4. **Type Flexibility**: `TypeElement` and its derived structures enable handling heterogeneous data.

---

## Additional Usage Examples

### Example – Building and Rotating Notes
```cpp
// Suppose we have a vector of notes:
TypeVector notes = {"C", "D", "E", "F", "G", "A", "B"};
// Rotate this vector so it starts at "E":
TypeVector rotated = rotate(notes, "E");
// rotated: {"E", "F", "G", "A", "B", "C", "D"}
```

### Example – Converting a Fixed Array
```cpp
int values[] = {10, 20, 30};
TypeVector tv = ToVector(values);
// tv[0] = 10 (as a TypeElement)
```

### Example – Table with Named Access
```cpp
Table people({}, {"Name", "Age", "Profession"});
people.addRow({"Alice", 25, "Engineer"});
people.addRow({"Bob", 30, "Musician"});

// Access columns:
Vector names = people.column("Name"); // Vector of TypeElement with ["Alice", "Bob"]

// Access by row and index:
Vector bob = people.row(1);
Serial.println(bob[0]); // "Bob"
Serial.println(bob[2]); // "Musician"
```

---

## Conclusion

The **DataHandler** library provides **structures** (Element, Vector, Table) that make it easier to handle heterogeneous data, along with a robust set of **auxiliary functions** (like `spin`, `rotate`, `spread`, etc.). Each function offers multiple usage patterns, depending on how you want to **transform**, **index**, or **query** your data.

Any doubts or need more implementation details? Check out the GitHub repository or open an issue! 🏆✨


--- 


PT-BR 🟢
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
