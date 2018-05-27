#include <lamppost/messages/Datagram.h>


namespace lp {
  namespace messages {
    Datagram::Datagram() : mType(DatagramType::List), mValue(nullptr) {
    }

    Datagram::~Datagram() {
      mDictionary.clear();
      mList.clear();
      mValue = nullptr;
    }

    DatagramType Datagram::GetType() {
      return mType;
    }

    std::shared_ptr<Datagram>& Datagram::operator[](std::string key) {
      return Get(std::move(key));
    }

    std::shared_ptr<Datagram>& Datagram::Get(const std::string& key) {
      if(mType != DatagramType::Dictionary) {
        mType = DatagramType::Dictionary;
        mList.clear();
        mValue = nullptr;
      }

      if(mDictionary.find(key) == mDictionary.end()) {
        throw exceptions::KeyNotFoundException(key, "Key not found in dictionary.");
      }

      return mDictionary[key];
    }

    void Datagram::Remove(std::string key) {
      if(mType != DatagramType::Dictionary) {
        throw exceptions::InvalidOperationException("Datagram must be a dictionary.");
      }

      std::map<std::string, std::shared_ptr<Datagram>>::iterator it = mDictionary.find(key);
      if(it == mDictionary.end()) {
        throw exceptions::KeyNotFoundException(key, "Key not found in dictionary.");
      }

      mDictionary.erase(it);
    }

    std::shared_ptr<Datagram>& Datagram::operator[](unsigned int index) {
      return Get(index);
    }

    std::shared_ptr<Datagram>& Datagram::Get(unsigned int index) {
      if(mType != DatagramType::List) {
        mType = DatagramType::List;
        mDictionary.clear();
        mValue = nullptr;
      }

      if(index >= mList.size()) {
        throw exceptions::IndexOutOfBoundsException(index, "Index out of bounds.");
      }

      std::list<std::shared_ptr<Datagram>>::iterator it = mList.begin();
      std::advance(it, index);

      return *it;
    }

    void Datagram::Add(std::shared_ptr<Datagram> datagram) {
      if(mType != DatagramType::List) {
        throw exceptions::InvalidOperationException("Datagram must be a list.");
      }

      mList.push_back(datagram);
    }

    void Datagram::Remove(unsigned int index) {
      if(mType != DatagramType::List) {
        throw exceptions::InvalidOperationException("Datagram must be a list.");
      }

      if(index >= mList.size()) {
        throw exceptions::IndexOutOfBoundsException(index, "Index out of bounds.");
      }

      std::list<std::shared_ptr<Datagram>>::iterator it = mList.begin();
      std::advance(it, index);

      mList.erase(it);
    }

    unsigned int Datagram::GetCount() {
      int count;

      switch(mType) {
        case DatagramType::List:
          count = mList.size();
          break;

        case DatagramType::Dictionary:
          count = mDictionary.size();
          break;

        default:
          throw exceptions::InvalidOperationException("Datagram must be a list or a dictionary.");
          break;
      }

      return count;
    }

    void Datagram::Clear() {
      switch(mType) {
        case DatagramType::List:
          mList.clear();
          break;

        case DatagramType::Dictionary:
          mDictionary.clear();
          break;

        default:
          throw exceptions::InvalidOperationException("Datagram must be a list or a dictionary.");
          break;
      }
    }

    Datagram& Datagram::operator=(int value) {
      SetValue(value);

      return *this;
    }

    Datagram& Datagram::operator=(bool value) {
      SetValue(value);

      return *this;
    }

    Datagram& Datagram::operator=(double value) {
      SetValue(value);

      return *this;
    }

    Datagram& Datagram::operator=(std::string value) {
      SetValue(value);

      return *this;
    }
  } // namespace messages
} // namespace lp
