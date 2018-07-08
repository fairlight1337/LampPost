#ifndef LAMPPOST_ANTMATCHER_H
#define LAMPPOST_ANTMATCHER_H


#include <string>


namespace lp
{
  namespace utilities
  {
    class AntMatcher
    {
    public:
      AntMatcher() = default;
      virtual ~AntMatcher() = default;

      static bool Matches(std::string pattern, std::string expression);
    };
  }
}

#endif //LAMPPOST_ANTMATCHER_H
