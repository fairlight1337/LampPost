#include <lamppost/messages/Datagram.h>


namespace lp
{
  namespace messages
  {
    Datagram::Datagram(std::shared_ptr<RawDatagram> rawDatagram)
      : mRawDatagram(rawDatagram)
    {
    }

    unsigned int Datagram::GetCount()
    {
      return mRawDatagram->GetCount();
    }

    void Datagram::Clear()
    {
      mRawDatagram->Clear();
    }

    RawDatagramType Datagram::GetType()
    {
      return mRawDatagram->GetType();
    }
  } // namespace messages
} // namespace lp
