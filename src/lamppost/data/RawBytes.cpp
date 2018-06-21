#include <lamppost/data/RawBytes.h>


namespace lp
{
  namespace data
  {
    RawBytes::RawBytes(void* content, std::size_t size)
    {
      if(content == nullptr)
      {
        throw exceptions::ArgumentNullException("content", "Content may not be null.");
      }

      mContent = (void*)(new char[size]);
      std::memcpy(mContent, content, size);
    }

    RawBytes::~RawBytes()
    {
      if(mContent != nullptr)
      {
        delete[] (char*)mContent;
      }
    }

    const void* RawBytes::GetContent()
    {
      return mContent;
    }

    std::size_t RawBytes::GetSize()
    {
      return mSize;
    }
  }
}
