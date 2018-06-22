#ifndef LAMPPOST_DATA_H
#define LAMPPOST_DATA_H


#include <lamppost/messages/DataBase.h>


namespace lp
{
  namespace messages
  {
    template<typename T>
    class Data : public DataBase
    {
    private:
      T mValue;

    public:
      Data() : mValue(T()) {}
      Data(T value) : mValue(value) {}
      ~Data() = default;

      T Get()
      {
        return mValue;
      }

      std::shared_ptr<DataBase> Copy() override
      {
        return std::make_shared<Data<T>>(mValue);
      }

      bool Equals(std::shared_ptr<DataBase> rhs) override
      {
        bool isEqual = false;
        std::shared_ptr<Data<T>> data = std::dynamic_pointer_cast<Data<T>>(rhs);

        if(data != nullptr)
        {
          isEqual = (data->Get() == this->Get());
        }

        return isEqual;
      }

      void ToString(std::ostream& outputStream) const override
      {
        outputStream << mValue;
      }

      bool operator==(const Data<T>& rhs) const
      {
        return mValue == rhs.mValue;
      }

      bool operator!=(const Data<T>& rhs) const
      {
        return !(rhs == *this);
      }
    };
  } // namespace messages
} // namespace lp


#endif //LAMPPOST_DATA_H
