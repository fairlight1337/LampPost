#include <lamppost/messages/RawDatagram.h>


namespace lp
{
  namespace messages
  {
    RawDatagram::RawDatagram()
      : mType(RawDatagramType::Empty),
        mValue(nullptr)
    {
    }

    RawDatagram::RawDatagram(RawDatagramType rawDatagramType)
      : mType(rawDatagramType)
    {
      switch(mType)
      {
        case RawDatagramType::Value:
          mValue = std::make_shared<Data<int>>(0);
          break;

        default:
          mValue = nullptr;
          break;
      }
    }

    RawDatagram::RawDatagram(std::shared_ptr<DataBase> data)
      : mType(RawDatagramType::Value),
        mValue(data)
    {
    }

    RawDatagram::~RawDatagram()
    {
      mDictionary.clear();
      mList.clear();
      mValue = nullptr;
    }

    RawDatagramType RawDatagram::GetType()
    {
      return mType;
    }

    std::shared_ptr<RawDatagram> RawDatagram::Copy()
    {
      std::shared_ptr<RawDatagram> copy = std::make_shared<RawDatagram>();
      copy->mType = mType;

      switch(mType)
      {
        case RawDatagramType::Empty:
          break;

        case RawDatagramType::Dictionary:
        {
          for(std::pair<std::string, std::shared_ptr<RawDatagram>> pair : mDictionary)
          {
            (*copy)[pair.first] = pair.second->Copy();
          }
        } break;

        case RawDatagramType::List:
        {
          for(const std::shared_ptr<RawDatagram>& listItem : mList)
          {
            copy->Add(listItem->Copy());
          }
        } break;

        case RawDatagramType::Value:
        {
          *copy = mValue->Copy();
        } break;
      }

      return copy;
    }

    std::shared_ptr<RawDatagram>& RawDatagram::operator[](std::string key)
    {
      return Get(key);
    }

    std::shared_ptr<RawDatagram>& RawDatagram::Get(const std::string& key)
    {
      if(mType != RawDatagramType::Dictionary)
      {
        mType = RawDatagramType::Dictionary;
        mList.clear();
        mValue = nullptr;
      }

      if(mDictionary.find(key) == mDictionary.end())
      {
        mDictionary[key] = std::make_shared<RawDatagram>();
      }

      return mDictionary[key];
    }

    void RawDatagram::Remove(std::string key)
    {
      if(mType != RawDatagramType::Dictionary)
      {
        throw exceptions::InvalidOperationException("RawDatagram must be a dictionary.");
      }

      std::map<std::string, std::shared_ptr<RawDatagram>>::iterator it = mDictionary.find(key);
      if(it == mDictionary.end())
      {
        throw exceptions::KeyNotFoundException(key, "Key not found in dictionary.");
      }

      mDictionary.erase(it);
    }

    std::shared_ptr<RawDatagram>& RawDatagram::operator[](unsigned int index)
    {
      return Get(index);
    }

    std::shared_ptr<RawDatagram>& RawDatagram::Get(unsigned int index)
    {
      if(mType != RawDatagramType::List)
      {
        mType = RawDatagramType::List;
        mDictionary.clear();
        mValue = nullptr;
      }

      if(index >= mList.size())
      {
        throw exceptions::IndexOutOfBoundsException(index, "Index out of bounds.");
      }

      std::list<std::shared_ptr<RawDatagram>>::iterator it = mList.begin();
      std::advance(it, index);

      return *it;
    }

    void RawDatagram::Add(std::shared_ptr<RawDatagram> datagram)
    {
      if(mType != RawDatagramType::List)
      {
        mDictionary.clear();
        mValue = nullptr;

        mType = RawDatagramType::List;
      }

      mList.push_back(datagram);
    }

    void RawDatagram::Remove(unsigned int index)
    {
      if(mType != RawDatagramType::List)
      {
        throw exceptions::InvalidOperationException("RawDatagram must be a list.");
      }

      if(index >= mList.size())
      {
        throw exceptions::IndexOutOfBoundsException(index, "Index out of bounds.");
      }

      std::list<std::shared_ptr<RawDatagram>>::iterator it = mList.begin();
      std::advance(it, index);

      mList.erase(it);
    }

    unsigned int RawDatagram::GetCount()
    {
      int count;

      switch(mType) {
        case RawDatagramType::List:
          count = mList.size();
          break;

        case RawDatagramType::Dictionary:
          count = mDictionary.size();
          break;

        default:
          throw exceptions::InvalidOperationException("RawDatagram must be a list or a dictionary.");
          break;
      }

      return count;
    }

    void RawDatagram::Clear()
    {
      switch(mType) {
        case RawDatagramType::List:
          mList.clear();
          break;

        case RawDatagramType::Dictionary:
          mDictionary.clear();
          break;

        default:
          throw exceptions::InvalidOperationException("RawDatagram must be a list or a dictionary.");
          break;
      }
    }

    RawDatagram& RawDatagram::operator=(int value)
    {
      SetValue(value);

      return *this;
    }

    RawDatagram& RawDatagram::operator=(bool value)
    {
      SetValue(value);

      return *this;
    }

    RawDatagram& RawDatagram::operator=(double value)
    {
      SetValue(value);

      return *this;
    }

    RawDatagram& RawDatagram::operator=(std::string value)
    {
      SetValue(value);

      return *this;
    }

    RawDatagram& RawDatagram::operator=(const std::shared_ptr<DataBase>& data)
    {
      if(mType != RawDatagramType::Value)
      {
        mDictionary.clear();
        mList.clear();

        mType = RawDatagramType::Value;
      }

      mValue = data;

      return *this;
    }
  } // namespace messages
} // namespace lp
