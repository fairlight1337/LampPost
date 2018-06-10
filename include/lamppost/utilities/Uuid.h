#ifndef LAMPPOST_UUID_H
#define LAMPPOST_UUID_H


#include <string>
#include <iostream>

#include <time.h>
#include <stdio.h>


namespace lp
{
  namespace utilities
  {
    class Uuid
    {
    private:
      bool mIsEmpty;
      std::string mContent;

    public:
      Uuid();
      ~Uuid() = default;

      static Uuid CreateUuid()
      {
        Uuid uuid;

        srand(static_cast<unsigned int>(time(nullptr)));
        char buffer[128];
        sprintf_s(buffer, 128, "%x%x-%x-%x-%x-%x%x%x",
                  rand(), rand(),                 // Generates a 64-bit Hex number
                  rand(),                         // Generates a 32-bit Hex number
                  ((rand() & 0x0fff) | 0x4000),   // Generates a 32-bit Hex number of the form 4xxx (4 indicates the UUID version)
                  rand() % 0x3fff + 0x8000,       // Generates a 32-bit Hex number in the range [0x8000, 0xbfff]
                  rand(), rand(), rand());        // Generates a 96-bit Hex number

        uuid.mContent = std::string(buffer);
        uuid.mIsEmpty = false;

        return uuid;
      }

      friend std::ostream& operator<<(std::ostream& outputStream, const Uuid& uuid)
      {
        std::string content = uuid.mContent;
        outputStream << content;

        return outputStream;
      }

      operator std::string() const
      {
        return mContent;
      }

      bool operator==(const Uuid& otherUuid)
      {
        return ((this->mIsEmpty && otherUuid.mIsEmpty) ||
                (!this->mIsEmpty && !otherUuid.mIsEmpty && this->mContent == otherUuid.mContent));
      }
    };
  } // namespace utilities
} // namespace lp


#endif //LAMPPOST_UUID_H
