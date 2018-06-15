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

      Datagram operator[](unsigned int index)
      {
        return Datagram((*mRawDatagram)[index]);
      }

      Datagram operator[](std::string key)
      {
        std::shared_ptr<RawDatagram> rawDatagram = (*mRawDatagram)[key];
        if(rawDatagram == nullptr)
        {
          (*mRawDatagram)[key] = std::make_shared<RawDatagram>();
          rawDatagram = (*mRawDatagram)[key];
        }

        return Datagram(rawDatagram);
      }

      template<class ... Args>
      void Add(Args ... args)
      {
        mRawDatagram->Add(std::forward<Args>(args)...);
      }

      template<class ... Args>
      void Remove(Args ... args)
      {
        mRawDatagram->Remove(std::forward<Args>(args)...);
      }

      unsigned int GetCount();
      void Clear();

      RawDatagramType GetType();

      template<typename ContentType>
      Datagram& operator=(ContentType contentValue)
      {
        (*mRawDatagram) = contentValue;

        return *this;
      }

      operator std::string() const
      {
        return mRawDatagram->Get<std::string>();
      }

      bool IsEmpty();
    };
  } // namespace messages
} // namespace lp


#endif //LAMPPOST_DATAGRAM_H
