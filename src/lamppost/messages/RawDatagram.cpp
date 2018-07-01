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

    bool RawDatagram::KeyExists(std::string key)
    {
      return mDictionary.find(key) != mDictionary.end();
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
      SetValue(std::move(value));

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

    flatbuffers::Offset<schemas::FBDatagram> RawDatagram::SerializeToStructure(flatbuffers::FlatBufferBuilder &builder)
    {
      schemas::DatagramType datagramType;
      schemas::ValueType valueType = schemas::ValueType::ValueType_String;

      flatbuffers::Offset<flatbuffers::String> stringContent;
      int32_t integerContent = 0;
      double doubleContent = 0.0f;
      bool booleanContent = false;
      std::vector<flatbuffers::Offset<schemas::FBDatagram>> listContentRaw;

      std::vector<flatbuffers::Offset<flatbuffers::String>> dictionaryKeysContentRaw;
      std::vector<flatbuffers::Offset<schemas::FBDatagram>> dictionaryValuesContentRaw;

      switch(GetType())
      {
        case RawDatagramType::Value:
          datagramType = schemas::DatagramType::DatagramType_Value;

          if(IsOfValueType<std::string>())
          {
            valueType = schemas::ValueType::ValueType_String;
            stringContent = builder.CreateString(Get<std::string>());
          }
          else if(IsOfValueType<int>())
          {
            valueType = schemas::ValueType::ValueType_Integer;
            integerContent = Get<int>();
          }
          else if(IsOfValueType<double>())
          {
            valueType = schemas::ValueType::ValueType_Double;
            doubleContent = Get<double>();
          }
          else if(IsOfValueType<bool>())
          {
            valueType = schemas::ValueType::ValueType_Boolean;
            booleanContent = Get<bool>();
          }
          else
          {
            datagramType = schemas::DatagramType::DatagramType_Empty;
          }
          break;

        case RawDatagramType::List:
          datagramType = schemas::DatagramType::DatagramType_List;

          for(const std::shared_ptr<RawDatagram>& listItem : mList)
          {
            listContentRaw.push_back(listItem->SerializeToStructure(builder));
          }
          break;

        case RawDatagramType::Dictionary:
          datagramType = schemas::DatagramType::DatagramType_Dictionary;

          for(std::pair<std::string, std::shared_ptr<RawDatagram>> dictionaryItem : mDictionary)
          {
            dictionaryKeysContentRaw.push_back(builder.CreateString(dictionaryItem.first));
            dictionaryValuesContentRaw.push_back(dictionaryItem.second->SerializeToStructure(builder));
          }
          break;

        case RawDatagramType::Empty:
          datagramType = schemas::DatagramType::DatagramType_Empty;
          break;
      }

      flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<schemas::FBDatagram>>> listContent = builder.CreateVector(listContentRaw);
      flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<flatbuffers::String>>> dictionaryKeysContent = builder.CreateVector(dictionaryKeysContentRaw);
      flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<schemas::FBDatagram>>> dictionaryValuesContent = builder.CreateVector(dictionaryValuesContentRaw);

      return schemas::CreateFBDatagram(
        builder,
        datagramType,
        valueType,
        stringContent,
        integerContent,
        doubleContent,
        booleanContent,
        listContent,
        dictionaryKeysContent,
        dictionaryValuesContent);
    }

    std::shared_ptr<RawDatagram> RawDatagram::DeserializeFromStructure(const schemas::FBDatagram* structure)
    {
      std::shared_ptr<RawDatagram> rawDatagram = std::make_shared<RawDatagram>();

      switch(structure->datagram_type())
      {
        case schemas::DatagramType::DatagramType_Value:
        {
          switch(structure->value_type())
          {
            case schemas::ValueType::ValueType_String:
              rawDatagram->SetValue<std::string>(structure->string_content()->str());
              break;

            case schemas::ValueType::ValueType_Double:
              rawDatagram->SetValue<double>(structure->double_content());
              break;

            case schemas::ValueType::ValueType_Integer:
              rawDatagram->SetValue<int>(structure->integer_content());
              break;

            case schemas::ValueType::ValueType_Boolean:
              rawDatagram->SetValue<bool>(structure->boolean_content());
              break;
          }
        } break;

        case schemas::DatagramType::DatagramType_List:
        {
          unsigned int length = structure->list_content()->size();

          for(unsigned int i = 0; i < length; ++i)
          {
            rawDatagram->Add(RawDatagram::DeserializeFromStructure(structure->list_content()->operator[](i)));
          }
        } break;

        case schemas::DatagramType::DatagramType_Dictionary:
        {
          unsigned int length = std::min(structure->dictionary_keys()->size(), structure->dictionary_contents()->size());

          for(unsigned int i = 0; i < length; ++i)
          {
            rawDatagram->operator[](structure->dictionary_keys()->operator[](i)->str()) = RawDatagram::DeserializeFromStructure(structure->dictionary_contents()->operator[](i));
          }
        } break;

        case schemas::DatagramType::DatagramType_Empty:
        {
        } break;
      }

      return rawDatagram;
    }

    std::shared_ptr<RawDatagram> RawDatagram::DeserializeFromJson(const std::string& json)
    {
      std::shared_ptr<RawDatagram> rawDatagram = std::make_shared<RawDatagram>();

      struct json_tokener* tok;
      struct json_object* jobj;
      enum json_tokener_error error;

      tok = json_tokener_new_ex(1000);

      jobj = json_tokener_parse_ex(tok, json.c_str(), json.length());
      error = tok->err;

      if(error == json_tokener_success)
      {
        ParseJsonObject(jobj, rawDatagram);
      }

      return rawDatagram;
    }

    void RawDatagram::ParseJsonObject(json_object* jobj, std::shared_ptr<RawDatagram> root)
    {
      switch(json_object_get_type(jobj)) {
        case json_type_boolean:
        {
          *root = static_cast<bool>(json_object_get_boolean(jobj));
        } break;

        case json_type_double:
        {
          *root = json_object_get_double(jobj);
        } break;

        case json_type_int:
        {
          *root = static_cast<int>(json_object_get_int(jobj));
        } break;

        case json_type_string:
        {
          *root = std::string(json_object_get_string(jobj));
        } break;

        case json_type_array:
        {
          json_object* jarray = jobj, *jvalue;

          for(size_t i = 0; i < json_object_array_length(jarray); i++)
          {
            jvalue = json_object_array_get_idx(jarray, i);
            std::shared_ptr<RawDatagram> rawChildDatagram = std::make_shared<RawDatagram>();
            ParseJsonObject(jvalue, rawChildDatagram);

            root->Add(rawChildDatagram);
          }
        } break;

        case json_type_object:
        {
          // Note(fairlight1337): The following line is excluded from tidy checks as the utilized macro expands to
          // (according to tidy) unclean code which cannot be changed at this time.
          json_object_object_foreach(jobj, key, val) // NOLINT
          {
            std::shared_ptr<RawDatagram> rawChildDatagram = std::make_shared<RawDatagram>();
            ParseJsonObject(val, rawChildDatagram);

            root->operator[](key) = rawChildDatagram;
          }
        } break;

        case json_type_null:
        {
        } break;
      }
    }

    bool RawDatagram::operator==(const RawDatagram& rhs) const
    {
      bool isEqual = false;

      if(rhs.mType == mType)
      {
        switch(mType)
        {
          case RawDatagramType::Empty:
          {
            isEqual = true;
          } break;

          case RawDatagramType::Value:
          {
            isEqual = rhs.mValue != nullptr && mValue != nullptr && rhs.mValue->Equals(mValue);
          } break;

          case RawDatagramType::List:
          {
            if(rhs.mList.size() == mList.size())
            {
              isEqual = true;

              std::list<std::shared_ptr<RawDatagram>>::const_iterator iterOwn = mList.begin();
              std::list<std::shared_ptr<RawDatagram>>::const_iterator iterRhs = rhs.mList.begin();

              while(iterOwn != mList.end())
              {
                if(**iterOwn != **iterRhs)
                {
                  isEqual = false;
                  break;
                }

                iterOwn++;
                iterRhs++;
              }
            }
          } break;

          case RawDatagramType::Dictionary:
          {
            if(rhs.mDictionary.size() == mDictionary.size())
            {
              isEqual = true;

              for(std::pair<std::string, std::shared_ptr<RawDatagram>> pair : mDictionary)
              {
                std::string key = pair.first;
                std::map<std::string, std::shared_ptr<RawDatagram>>::const_iterator iterRhs = rhs.mDictionary.find(key);

                if((iterRhs == rhs.mDictionary.end()) || (*(*iterRhs).second != *(pair.second)))
                {
                  isEqual = false;
                  break;
                }
              }
            }
          } break;
        }
      }

      return isEqual;
    }

    bool RawDatagram::operator!=(const RawDatagram& rhs) const
    {
      return !(*this == rhs);
    }
  } // namespace messages
} // namespace lp
