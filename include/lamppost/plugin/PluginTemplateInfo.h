#ifndef LAMPPOST_PLUGINTEMPLATEINFO_H
#define LAMPPOST_PLUGINTEMPLATEINFO_H


#include <lamppost/Version.h>


namespace lp
{
  struct PluginTemplateInfo
  {
    std::string mIdentifier;
    Version mVersion;
  };
} // namespace lp


#endif //LAMPPOST_PLUGINTEMPLATEINFO_H
