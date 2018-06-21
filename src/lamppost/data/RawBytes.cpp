#include <lamppost/data/RawBytes.h>


namespace lp
{
  namespace data
  {
    RawBytes::RawBytes(void* content, size_t size)
    {
      if(content == nullptr)
      {
        throw exceptions::ArgumentNullException("content", "Content may not be null.");
      }

      mContent = new char[size];
      memcpy(mContent, content, size);
    }

    RawBytes::~RawBytes()
    {
      if(mContent != nullptr)
      {
        delete[] mContent;
      }
    }

    const void* RawBytes::GetContent()
    {
      return mContent;
    }

    size_t RawBytes::GetSize()
    {
      return mSize;
    }
  }
}
