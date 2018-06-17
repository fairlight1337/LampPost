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

      void ToString(std::ostream& outputStream) const override
      {
        outputStream << mValue;
      }
    };
  } // namespace messages
} // namespace lp


#endif //LAMPPOST_DATA_H
