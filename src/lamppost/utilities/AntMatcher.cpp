#include <lamppost/utilities/AntMatcher.h>


namespace lp
{
  namespace utilities
  {
    bool AntMatcher::Matches(std::string pattern, std::string expression)
    {
      bool result = false;

      if(expression == pattern)
      {
        result = true;
      }
      else
      {
        // TODO(fairlight1337): Implement this.
      }

      return result;
    }
  }
}
