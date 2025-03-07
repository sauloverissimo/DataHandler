# DataHandler Release Notes

## Version 1.0.1

### Date: 2025-03-06

**Overview / Visão Geral**
- Minor enhancements and fixes across DataHandler structures.
- Improved documentation for usage on ESP32.

### Changes / Alterações
1. **Table Column Indexing**
   - Corrected an issue where `columnIndex` was not updated when columns were appended.
2. **Element Constructors**
   - Added better error handling for invalid vector indexing in `Element::operator[]`.
3. **Spin Function**
   - Now handles empty vectors gracefully, returning an empty table instead of producing an error.
4. **PrintHandler Integration**
   - Enhanced compatibility with `printhandler.h`, ensuring seamless printing of `Vector` and `Table`.

### New Features / Novos Recursos
- **GrauDoGrau**: Extended the function to handle negative intervals.
- **Spread Overload**: Overload that duplicates multiple subsets of a `TypeVector`.

### Bug Fixes / Correções
- Fixed a corner case in `rotate()` when `elem` is not found in the vector.
- Resolved an occasional crash when `rowNameToIndex` was empty.

### Migration / Migração
- No breaking changes introduced.
- Simply replace the old `datahandler.h/.cpp` with the new files.
- If using `printhandler.h`, ensure you update it as well.

### Known Issues / Problemas Conhecidos
- Intermittent memory usage spikes on lower-memory boards (e.g., Arduino Uno) when using large `TypeCube` objects.
- Ongoing improvement in progress.

### Acknowledgments / Agradecimentos
- Thank you to the community for feedback and testing!
- Obrigado à comunidade pelo feedback e testes!

**Enjoy the new release! / Aproveite a nova versão!**