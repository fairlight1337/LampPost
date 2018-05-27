#ifndef LAMPPOST_IDENTIFIABLE_H
#define LAMPPOST_IDENTIFIABLE_H


#include <string>
#include <utility>


namespace lp {
  class Identifiable {
  private:
    std::string mIdentifier;

  public:
    Identifiable(const std::string& identifier);
    virtual ~Identifiable() = default;

    std::string GetIdentifier();
  };
} // namespace lp

#endif //LAMPPOST_IDENTIFIABLE_H
