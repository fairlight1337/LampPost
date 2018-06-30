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
      Datagram(const Datagram& datagram);
      Datagram(RawDatagramType rawDatagramType);
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

      bool KeyExists(std::string key);

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

      bool operator==(const Datagram& rhs) const;

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
        *mRawDatagram = contentValue;

        return *this;
      }

      Datagram& operator=(const Datagram& otherDatagram)
      {
        *mRawDatagram = *(otherDatagram.mRawDatagram->Copy());

        return *this;
      }

      operator std::string() const
      {
        return mRawDatagram->Get<std::string>();
      }

      bool IsEmpty();

      template<typename DataType>
      DataType Get(std::string path, DataType defaultValue = DataType())
      {
        return mRawDatagram->Get<DataType>(path, defaultValue);
      }

      friend std::ostream& operator<<(std::ostream& outputStream, const Datagram& datagram)
      {
        outputStream << *(datagram.mRawDatagram);

        return outputStream;
      }

      flatbuffers::Offset<schemas::FBDatagram> SerializeToStructure(flatbuffers::FlatBufferBuilder& builder)
      {
        return mRawDatagram->SerializeToStructure(builder);
      }

      static Datagram Deserialize(const schemas::FBDatagram* structure)
      {
        return Datagram(RawDatagram::Deserialize(structure));
      }

      static Datagram DeserializeFromJson(const std::string json)
      {
        return Datagram(RawDatagram::DeserializeFromJson(json));
      }
    };
  } // namespace messages
} // namespace lp


#endif //LAMPPOST_DATAGRAM_H
