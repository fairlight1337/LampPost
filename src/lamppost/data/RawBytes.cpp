#include <lamppost/data/RawBytes.h>


namespace lp
{
  namespace data
  {
    RawBytes::RawBytes(void* content, std::size_t size)
      : mSize(size)
    {
      if(content == nullptr)
      {
        throw exceptions::ArgumentNullException("content", "Content may not be null.");
      }

      mContent = static_cast<void*>(new char[size]);
      std::memcpy(mContent, content, size);
    }

    RawBytes::~RawBytes()
    {
      if(mContent != nullptr)
      {
        delete[] static_cast<char*>(mContent);
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
  } // namespace data
} // namespace lp
