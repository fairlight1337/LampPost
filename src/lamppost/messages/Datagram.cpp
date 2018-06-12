#include <lamppost/messages/Datagram.h>


namespace lp
{
  namespace messages
  {
    Datagram::Datagram()
      : mRawDatagram(std::make_shared<RawDatagram>())
    {
    }
  } // namespace messages
} // namespace lp
