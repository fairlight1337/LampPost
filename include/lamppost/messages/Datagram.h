#ifndef LAMPPOST_DATAGRAM_H
#define LAMPPOST_DATAGRAM_H


#include <lamppost/messages/RawDatagram.h>


namespace lp
{
  namespace messages
  {
    class Datagram
    {
    private:
      std::shared_ptr<RawDatagram> mRawDatagram;

    public:
      Datagram();
      explicit Datagram(std::shared_ptr<RawDatagram> rawDatagram);

      template<class ... Args>
      Datagram(Args ... args)
        : mRawDatagram(std::make_shared<RawDatagram>(std::forward<Args>(args)...))
      {
      }

      ~Datagram() = default;

      template<typename DataType>
      DataType Get()
      {
        return mRawDatagram->Get<DataType>();
      }

      Datagram operator[](std::string key)
      {
        return Datagram((*mRawDatagram)[key]);
      }

      Datagram operator[](unsigned int index)
      {
        return Datagram((*mRawDatagram)[index]);
      }
    };
  } // namespace messages
} // namespace lp


#endif //LAMPPOST_DATAGRAM_H
