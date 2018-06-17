#include <lamppost/messages/Datagram.h>


namespace lp
{
  namespace messages
  {
    Datagram::Datagram(const Datagram& datagram)
    {
      mRawDatagram = std::make_shared<RawDatagram>();

      if(datagram.mRawDatagram != nullptr)
      {
        *mRawDatagram = *(datagram.mRawDatagram->Copy());
      }
    }

    Datagram::Datagram(std::shared_ptr<RawDatagram> rawDatagram)
    {
      mRawDatagram = rawDatagram;

      if(mRawDatagram == nullptr)
      {
        mRawDatagram = std::make_shared<RawDatagram>();
      }
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

    bool Datagram::IsEmpty()
    {
      return mRawDatagram->GetType() == RawDatagramType::Empty;
    }
  } // namespace messages
} // namespace lp
