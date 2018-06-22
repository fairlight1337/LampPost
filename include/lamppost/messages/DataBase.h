#ifndef LAMPPOST_DATABASE_H
#define LAMPPOST_DATABASE_H


#include <iostream>
#include <memory>


namespace lp
{
  namespace messages
  {
    class DataBase
    {
    public:
      DataBase() = default;
      virtual ~DataBase() = default;

      virtual void ToString(std::ostream& outputStream) const = 0;

      virtual std::shared_ptr<DataBase> Copy() = 0;
      virtual bool Equals(std::shared_ptr<DataBase> rhs) = 0;

      friend std::ostream& operator<<(std::ostream& outputStream, const DataBase& dataBase)
      {
        dataBase.ToString(outputStream);

        return outputStream;
      }
    };
  } // namespace messages
} // namespace lp


#endif //LAMPPOST_DATABASE_H
