#ifndef LAMPPOST_UUID_H
#define LAMPPOST_UUID_H


#include <iostream>
#include <random>
#include <string>

#include <stdio.h>
#include <time.h>


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

        std::mt19937 rng;
        rng.seed(std::random_device()());
        std::uniform_int_distribution<unsigned int> dist(1, -1);

        char buffer[128];
        snprintf(buffer, 128, "%x%x-%x-%x-%x-%x%x%x",
                 dist(rng), dist(rng),              // Generates a 64-bit Hex number
                 dist(rng),                         // Generates a 32-bit Hex number
                 ((dist(rng) & 0x0fff) | 0x4000),   // Generates a 32-bit Hex number of the form 4xxx (4 indicates the UUID version)
                 dist(rng) % 0x3fff + 0x8000,       // Generates a 32-bit Hex number in the range [0x8000, 0xbfff]
                 dist(rng), dist(rng), dist(rng));  // Generates a 96-bit Hex number


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
