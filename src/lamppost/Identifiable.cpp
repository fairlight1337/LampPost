#include <lamppost/Identifiable.h>


namespace lp {
  Identifiable::Identifiable(const std::string& identifier) : mIdentifier(identifier) {
  }

  std::string Identifiable::GetIdentifier() {
    return mIdentifier;
  }
} // namespace lp