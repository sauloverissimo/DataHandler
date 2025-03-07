/*
  note.ino - Example of using DataHandler + note.h on an ESP32
  Exemplo de uso de DataHandler + note.h em um ESP32

  Demonstrates multiple features / Demonstra diversas funcionalidades:
    1) Initialization and printing of the 'note' Table (note) / 
       Inicialização e impressão da Tabela de Notas (note).
    2) Row and column access (row / column) / 
       Acesso a linhas e colunas (row / column).
    3) Data manipulation with spin, rotate, spread, etc. / 
       Manipulação de dados com spin, rotate, spread, etc.
    4) Use of TypeVector, TypeElement, and printing with printhandler / 
       Uso de TypeVector, TypeElement e impressão com printhandler.
*/

#include <Arduino.h>
#include <datahandler.h>   // Main DataHandler library / Biblioteca principal DataHandler
#include <printhandler.h>  // Printing utilities / Utilitários de impressão
#include "note.h"          // 'note' table and 'initializeNote()' / Tabela 'note' e 'initializeNote()'

void setup() {
  Serial.begin(115200);
  delay(1000);

  // 1) Initialize the 'note' Table from note.cpp
  //    Inicializa a Tabela 'note' a partir de note.cpp
  initializeNote();

  Serial.println("\n--- DataHandler + Note.h Example / Exemplo ---");

  // 2) Print the entire 'note' table (raw data) / Imprime a tabela 'note' inteira (dados brutos)
  //    The 'note' table has 8 rows in rowNameToIndex, each row is a TypeVector,
  //    and 12 columns for notes (C, C#, D, etc.) / 
  //    A tabela 'note' possui 8 linhas em rowNameToIndex, cada linha é um TypeVector,
  //    e 12 colunas correspondentes às notas (C, C#, D etc.).

  Serial.println("\n1) Printing the 'note' Table / Imprimindo a Tabela 'note':");
  for (size_t r = 0; r < note.data.size(); r++) {
    Serial.print("Row / Linha ");
    Serial.print(r);
    Serial.print(": ");
    println(note.data[r]);  // Using printhandler's println for TypeVector
  }

  // 3) Accessing a row by name
  //    Acessando uma linha por nome (rowNameToIndex)
  Serial.println("\n2) Accessing the 'note' row by name / Acessando a linha 'note':");
  Vector rowNote = note.row("note"); // Returns a Vector
  Serial.print("Contents of 'note' row / Conteúdo da linha 'note': ");
  println(rowNote); // Prints a Vector directly

  // 4) Accessing a column by index
  //    Acessando uma coluna por índice
  Serial.println("\n3) Accessing column index 3 / Acessando a coluna de índice 3:");
  Vector col3 = note.column(3);
  Serial.print("Column 3: ");
  println(col3);

  // 5) Demonstration of spin/rotate on the note list
  //    Demonstração de spin/rotate na lista de notas
  Serial.println("\n4) Using rotate/spin on notes / Usando rotate/spin em notas.");

  // Convert the 'note' row (12 notes) to a TypeVector
  // Converte a linha 'note' (12 notas) para TypeVector
  TypeVector tvNotes = rowNote; // rowNote -> Vector -> TypeVector

  // a) spin: NxN table
  //    spin: gera tabela NxN
  Serial.println("   a) spin: NxN table, each row a rotation / tabela NxN, cada linha é uma rotação.");
  TypeTable sp = spin(tvNotes);
  for (size_t i = 0; i < sp.size(); i++) {
    Serial.print("spin row / linha ");
    Serial.print(i);
    Serial.print(": ");
    println(sp[i]); // Print each rotated row
  }

  // b) spin with index: single rotated row
  //    spin com índice: apenas uma linha rotacionada
  Serial.println("   b) spin(tvNotes, 2): shift 2 positions / desloca 2 posições.");
  TypeVector spIndex = spin(tvNotes, 2);
  Serial.print("Result / Resultado: ");
  println(spIndex);

  // c) rotate: rotate from a specific element
  //    rotate: rotaciona a partir de um elemento específico
  Serial.println("   c) rotate(tvNotes, \"F#\"):");
  TypeVector rotatedFsharp = rotate(tvNotes, std::string("F#"));
  Serial.print("Result / Resultado: ");
  println(rotatedFsharp);

  // 6) Demonstrating spread & repeatarray
  //    Demonstrando spread & repeatarray
  Serial.println("\n5) spread & repeatarray:");
  TypeVector simpleVec = {"X", "Y", "Z"};

  Serial.println("   - repeatarray(simpleVec): repeats the same row / repete a mesma linha:");
  TypeTable repeated = repeatarray(simpleVec);
  for (size_t i = 0; i < repeated.size(); i++) {
    Serial.print("Row / Linha ");
    Serial.print(i);
    Serial.print(": ");
    println(repeated[i]);
  }

  Serial.println("   - spread(simpleVec): NxN, each row repeated element / NxN, cada linha com elemento repetido:");
  TypeTable spSpread = spread(simpleVec);
  for (size_t i = 0; i < spSpread.size(); i++) {
    Serial.print("Row / Linha ");
    Serial.print(i);
    Serial.print(": ");
    println(spSpread[i]);
  }

  // 7) Converting a fixed array with ToVector
  //    Convertendo um array fixo com ToVector
  Serial.println("\n6) Converting a fixed array / Convertendo array fixo:");
  int arrTest[] = {100, 200, 300};
  TypeVector tvArr = ToVector(arrTest);
  Serial.print("tvArr = ");
  println(tvArr);

  // 8) Example: creating and manipulating a custom Table
  //    Exemplo: criando e manipulando uma Table custom
  Serial.println("\n7) Building a custom Table / Criando uma Table custom:");
  Table customTable({}, {"ID", "Value", "Comment"});
  customTable.addRow({1, 2.5, "First row / Primeira linha"});
  customTable.addRow({2, 3.14, "Second row / Segunda linha"});
  customTable.addRow({3, "XYZ", "Mixed types / Tipos mistos"});

  for (size_t r2 = 0; r2 < customTable.data.size(); r2++) {
    Serial.print("Row / Linha ");
    Serial.print(r2);
    Serial.print(": ");
    println(customTable.data[r2]);
  }

  Serial.println("\n--- End of Setup / Fim do Setup ---");
}

void loop() {
  // No continuous logic here / Não há lógica contínua aqui
  // All is demonstrated in setup() / Tudo demonstrado em setup()
}
