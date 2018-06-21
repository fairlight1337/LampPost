#ifndef LAMPPOST_RAWBYTES_H
#define LAMPPOST_RAWBYTES_H


#include <lamppost/exceptions/ArgumentNullException.h>


namespace lp
{
  namespace data
  {
    class RawBytes
    {
    private:
      void* mContent;
      size_t mSize;

    public:
      RawBytes(void* content, size_t size);
      ~RawBytes();

      const void* GetContent();
      size_t GetSize();
    };
  }
}


#endif //LAMPPOST_RAWBYTES_H
