#ifndef LAMPPOST_RAWBYTES_H
#define LAMPPOST_RAWBYTES_H


#include <cstring>

#include <lamppost/exceptions/ArgumentNullException.h>


namespace lp
{
  namespace data
  {
    class RawBytes
    {
    private:
      void* mContent;
      std::size_t mSize;

    public:
      RawBytes();
      RawBytes(void* content, std::size_t size);
      ~RawBytes();

      const void* GetContent();
      std::size_t GetSize();

      void Append(const void* data, std::size_t size);
    };
  } // namespace data
} // namespace lp


#endif //LAMPPOST_RAWBYTES_H
