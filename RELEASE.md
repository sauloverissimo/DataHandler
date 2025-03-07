```markdown
# DataHandler 📊📂  

**Advanced Data Handling Library for Arduino**  

A library for handling **variants, vectors, tables, and dataframes** efficiently in Arduino projects. Enables structured data processing, transformation, and management.  

---

## English 🇺🇸 🏈  

### Overview  
The **DataHandler** library provides a structured approach to handling **tables**, **vectors**, and **variants** in Arduino projects. It enables **storing, manipulating, and querying** data using a flexible and modular approach.  

This library is designed to **simplify** working with structured data and make Arduino-based applications more powerful, especially when handling tabular data like notes, scales, chords, and sequences.  

### File Structure  

- **datahandler.h / datahandler.cpp**  
  Core library files defining **Table**, **Vector**, and **Element** structures.  
  - **Key Features:**  
    - Dynamic **tables** with column-based indexing.  
    - **Vectors** for row-based operations.  
    - **Variants** for storing multiple data types (integers, floats, strings, vectors).  
    - Utility functions for **data transformation**, **spinning**, and **row/column manipulation**.  

- **examples/**  
  Includes multiple examples demonstrating how to **create, manipulate, and query** structured data.  
  - Example 1: **Basic table creation and row/column access**.  
  - Example 2: **Using tables with different data types**.  
  - Example 3: **Data transformations (spread, spin, and rotation functions)**.  

---

## Data Structures  

### 🔹 **Element**  
Represents a single data item that can be:  
✅ **Integer** (`int`)  
✅ **Float** (`float`)  
✅ **String** (`std::string`)  
✅ **Vector of Strings** (`std::vector<std::string>`)  

Example:  
```cpp
Element e1 = 42;  
Element e2 = "Hello";  
Element e3 = std::vector<std::string>{"A", "B", "C"};  
```

### 🔹 **Vector**  
A one-dimensional structure representing a **row** in a table.  
Example:  
```cpp
Vector row({"Alice", 25, "New York"});  
Serial.println(row[0]);  // Output: Alice  
```

### 🔹 **Table**  
A two-dimensional structure representing **structured data** with named columns.  
Example:  
```cpp
Table myTable({}, {"Name", "Age", "City"});  
myTable.addRow({"Bob", 30, "Los Angeles"});  
```

---

## 📌 Key Functions  

### **1️⃣ Accessing Rows and Columns**  

- **Get row by index**  
  ```cpp
  Vector row = myTable.row(0);  
  Serial.println(row[1]);  // Prints "30"
  ```
  
- **Get column by name**  
  ```cpp
  Vector ages = myTable.column("Age");  
  Serial.println(ages[0]);  // Prints "30"
  ```

### **2️⃣ Transforming Data**  

- **Spin a Vector**  
  ```cpp
  TypeVector myVec = {"A", "B", "C", "D"};  
  TypeVector rotated = spin(myVec, 1);  
  // Output: ["B", "C", "D", "A"]
  ```

- **Repeat Array into a Table**  
  ```cpp
  TypeTable repeated = repeatarray(myVec);  
  ```

### **3️⃣ Querying Data**  

- **Find row by value in a specific column**  
  ```cpp
  Vector foundRow = myTable.row("Bob", "Name");  
  Serial.println(foundRow[2]);  // Output: "Los Angeles"
  ```

---

## Example Usage  

### **Example 1: Creating a Table and Accessing Data**  

```cpp
#include <datahandler.h>

void setup() {
    Serial.begin(115200);
    Serial.println("\n🔹 DataHandler Library Initialized");

    // Create a table with column names
    Table people({}, {"Name", "Age", "City"});

    // Add rows of data
    people.addRow({"Alice", 25, "New York"});
    people.addRow({"Bob", 30, "Los Angeles"});

    // Access and print data
    Serial.println(people.row(1)["City"]);  // Output: Los Angeles
}

void loop() {}
```

---

## 🔄 How It Works  

1. **Table Creation**  
   - Define column names and structure.  
   - Add rows dynamically.  

2. **Data Access**  
   - Query rows by index or by column name.  
   - Retrieve columns as vectors for easy data manipulation.  

3. **Transformation Functions**  
   - **Spin**: Rotates a vector cyclically.  
   - **Spread**: Expands data for structured processing.  
   - **RepeatArray**: Creates repeated structured data.  

---

## 📖 Supported Platforms  

| Platform | Supported |
|----------|-----------|
| Arduino Uno | ❌ (Limited memory) |
| ESP32 | ✅ Fully Supported |
| ESP8266 | ✅ Fully Supported |
| STM32 | ✅ Fully Supported |

---

## 📖 Documentation  

For a detailed breakdown of the API, visit:  
📌 **[GitHub Repository](https://github.com/meuusuario/datahandler)**  

If you have any issues or suggestions, feel free to open an **Issue** or a **Pull Request** on GitHub.  

Happy Coding! 🚀📊
```

