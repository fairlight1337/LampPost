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
        // TODO(fairlight1337): Implement this. The current hack only allows the pattern "/**/*" to match every expression. This is required for the Link plugin development.
        if(pattern == "/**/*")
        {
          result = true;
        }
      }

      return result;
    }
  } // namespace utilities
} // namespace lp
