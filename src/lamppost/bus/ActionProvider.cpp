#include <lamppost/bus/ActionProvider.h>


namespace lp
{
  namespace bus
  {
    ActionProvider::ActionProvider(std::string actionTopicNamespace)
      : mActionTopicNamespace(actionTopicNamespace)
    {
    }
  }
}
