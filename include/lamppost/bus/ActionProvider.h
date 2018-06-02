#ifndef LAMPPOST_ACTIONPROVIDER_H
#define LAMPPOST_ACTIONPROVIDER_H


#include <string>


namespace lp
{
  namespace bus
  {
    class ActionProvider
    {
    private:
      std::string mActionTopicNamespace;

    public:
      ActionProvider(std::string actionTopicNamespace);
      ~ActionProvider() = default;
    };
  }
}


#endif //LAMPPOST_ACTIONPROVIDER_H
