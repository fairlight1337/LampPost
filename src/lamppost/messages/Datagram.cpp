#include <lamppost/messages/Datagram.h>


namespace lp
{
  namespace messages
  {
    Datagram::Datagram()
      : mRawDatagram(std::make_shared<RawDatagram>())
    {
    }

    Datagram::Datagram(std::shared_ptr<RawDatagram> rawDatagram)
      : mRawDatagram(rawDatagram)
    {
    }
  } // namespace messages
} // namespace lp
