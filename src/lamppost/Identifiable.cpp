#include <lamppost/Identifiable.h>


namespace lp {
  Identifiable::Identifiable(std::string identifier) : mIdentifier(identifier) {
  }

  Identifiable::~Identifiable() {
  }

  std::string Identifiable::GetIdentifier() {
    return mIdentifier;
  }
}