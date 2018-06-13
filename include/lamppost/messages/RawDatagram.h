#ifndef LAMPPOST_RAWDATAGRAM_H
#define LAMPPOST_RAWDATAGRAM_H


#include <algorithm>
#include <list>
#include <map>
#include <memory>
#include <string>
#include <utility>

#include <lamppost/messages/DataBase.h>
#include <lamppost/messages/Data.h>
#include <lamppost/exceptions/KeyNotFoundException.h>
#include <lamppost/exceptions/InvalidOperationException.h>
#include <lamppost/exceptions/IndexOutOfBoundsException.h>


namespace lp
{
  namespace messages
  {
    enum class RawDatagramType
    {
      List = 0,
      Dictionary = 1,
      Value = 2
    };

    class RawDatagram
    {
    private:
      std::map<std::string, std::shared_ptr<RawDatagram>> mDictionary;
      std::list<std::shared_ptr<RawDatagram>> mList;
      std::shared_ptr<DataBase> mValue;

      RawDatagramType mType;

      template<typename T>
      void SetValue(T value)
      {
        if(mType != RawDatagramType::Value)
        {
          mDictionary.clear();
          mList.clear();

          mType = RawDatagramType::Value;
        }

        mValue = std::make_shared<Data<T>>(value);
      }

    public:
      RawDatagram();

      template<typename ContentType>
      RawDatagram(ContentType contentValue)
        : mType(RawDatagramType::Value),
          mValue(nullptr)
      {
        SetValue(contentValue);
      }

      ~RawDatagram();

      RawDatagramType GetType();

      // Dictionary
      std::shared_ptr<RawDatagram>& operator[](std::string key);
      std::shared_ptr<RawDatagram>& Get(const std::string& key);
      void Remove(std::string key);

      // List
      std::shared_ptr<RawDatagram>& operator[](unsigned int index);
      std::shared_ptr<RawDatagram>& Get(unsigned int index);
      void Add(std::shared_ptr<RawDatagram> datagram);
      void Remove(unsigned int index);

      // Dictionary and List
      unsigned int GetCount();
      void Clear();

      // Values
      RawDatagram& operator=(std::string value);
      RawDatagram& operator=(int value);
      RawDatagram& operator=(double value);
      RawDatagram& operator=(bool value);

      template<typename T>
      T Get()
      {
        std::shared_ptr<Data<T>> data = std::dynamic_pointer_cast<Data<T>>(mValue);

        if(data != nullptr)
        {
          return data->Get();
        }
        else
        {
          throw exceptions::InvalidOperationException("Wrong data type.");
        }
      }
    };
  } // namespace messages
} // namespace lp


#endif //LAMPPOST_RAWDATAGRAM_H
