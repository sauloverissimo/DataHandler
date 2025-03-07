#ifndef PRINTHANDLER_H
#define PRINTHANDLER_H
//

#include <Arduino.h>
#include <vector>
#include <variant>
#include <string>
#include "datahandler.h"

// ------------------- IMPRESSÃO DE ELEMENTOS -------------------

// 🔹 Imprime um único elemento (TypeElement ou Element)
inline void print(const TypeElement& element) {
    std::visit([](auto&& value) {
        if constexpr (std::is_same_v<std::decay_t<decltype(value)>, std::string>) {
            Serial.print(value.c_str());
        } else if constexpr (std::is_same_v<std::decay_t<decltype(value)>, std::vector<std::string>>) {
            Serial.print("{");
            for (size_t i = 0; i < value.size(); ++i) {
                Serial.print(value[i].c_str());
                if (i < value.size() - 1) Serial.print(", ");
            }
            Serial.print("}");
        } else {
            Serial.print(value);
        }
    }, element);
}

// 🔹 Imprime um Element diretamente
inline void print(const Element& element) {
    print(element.value);
}

// 🔹 Imprime um TypeElement e pula linha
inline void println(const TypeElement& element) {
    print(element);
    Serial.println();
}


// ------------------- IMPRESSÃO DE VETORES -------------------

// 🔹 Imprime um vetor de elementos (TypeVector)
inline void print(const TypeVector& arr) {
    Serial.print("{ ");
    for (size_t i = 0; i < arr.size(); ++i) {
        print(arr[i]);
        if (i < arr.size() - 1) Serial.print(", ");
    }
    Serial.print(" }");
}

// 🔹 Imprime um vetor e pula linha
inline void println(const TypeVector& arr) {
    print(arr);
    Serial.println();
}

// 🔹 Imprime um objeto Vector corretamente (agora com suporte completo)
inline void print(const Vector& vec) {
    Serial.print("{ ");
    for (size_t i = 0; i < vec.values.size(); ++i) {
        std::visit([](auto&& value) {
            using T = std::decay_t<decltype(value)>;
            if constexpr (std::is_same_v<T, std::string>) {
                Serial.print(value.c_str());
            } else if constexpr (std::is_same_v<T, std::vector<std::string>>) {
                Serial.print("{ ");
                for (size_t j = 0; j < value.size(); ++j) {
                    Serial.print(value[j].c_str());
                    if (j < value.size() - 1) Serial.print(", ");
                }
                Serial.print(" }");
            } else {
                Serial.print(value);
            }
        }, vec.values[i]);

        if (i < vec.values.size() - 1) Serial.print(", ");
    }
    Serial.print(" }");
}

// 🔹 Imprime um objeto Vector e pula linha
inline void println(const Vector& vec) {
    print(vec);
    Serial.println();
}

// // ------------------- IMPRESSÃO DE TABLE -------------------

// 🔹 Imprime uma tabela (TypeTable)
inline void print(const TypeTable& table) {
    Serial.println("{");
    for (const auto& row : table) {
        Serial.print("  ");
        print(row);
        Serial.println(",");
    }
    Serial.println("}");
}

// 🔹 Imprime uma tabela (Table)
inline void print(const Table& table) {
    print(table.data);
}

// 🔹 Imprime uma tabela e pula linha
inline void println(const Table& table) {
    print(table);
    Serial.println();
}

#endif // PRINTHANDLER_H
