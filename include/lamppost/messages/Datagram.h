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

      template<class ... Args>
      Datagram(Args ... args)
        : mRawDatagram(std::make_shared<RawDatagram>(std::forward<Args>(args)...))
      {
      }

      ~Datagram() = default;
    };
  } // namespace messages
} // namespace lp


#endif //LAMPPOST_DATAGRAM_H
