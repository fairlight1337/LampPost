#include <lamppost/messages/Datagram.h>


namespace lp
{
  namespace messages
  {
    Datagram::Datagram(const Datagram& datagram)
      : mRawDatagram(std::make_shared<RawDatagram>())
    {
      if(datagram.mRawDatagram != nullptr)
      {
        *mRawDatagram = *(datagram.mRawDatagram->Copy());
      }
    }

    Datagram::Datagram(RawDatagramType rawDatagramType)
      : mRawDatagram(std::make_shared<RawDatagram>(rawDatagramType))
    {
    }

    Datagram::Datagram(std::shared_ptr<RawDatagram> rawDatagram)
      : mRawDatagram(std::move(rawDatagram))
    {
      if(mRawDatagram == nullptr)
      {
        mRawDatagram = std::make_shared<RawDatagram>();
      }
    }

    bool Datagram::KeyExists(std::string key)
    {
      return mRawDatagram->KeyExists(std::move(key));
    }

    bool Datagram::operator==(const Datagram& rhs) const
    {
      return *(rhs.mRawDatagram) == *mRawDatagram;
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
