#include "datahandler.h"


// Implementação das funções da biblioteca (se necessário)

std::vector<std::string> getRotation(const std::vector<std::string>& sequencia,
                                     const std::string& inicio,
                                     const std::vector<int>& remove) {
  std::vector<std::string> rotacionada = sequencia;

  // Encontrar o índice da nota inicial
  auto it = std::find(rotacionada.begin(), rotacionada.end(), inicio);
  if (it == rotacionada.end()) {
    Serial.println("Erro: Nota inicial não encontrada na sequência!");
    return {};
  }

  // Rotacionar o vetor para que a nota inicial seja a primeira
  std::rotate(rotacionada.begin(), it, rotacionada.end());

  // Criar um novo vetor excluindo os índices especificados
  std::vector<std::string> resultado;
  for (size_t i = 0; i < rotacionada.size(); ++i) {
    if (std::find(remove.begin(), remove.end(), i) == remove.end()) {
      resultado.push_back(rotacionada[i]);
    }
  }

  return resultado;
}
