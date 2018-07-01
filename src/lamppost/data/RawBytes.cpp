#include <lamppost/data/RawBytes.h>


namespace lp
{
  namespace data
  {
    RawBytes::RawBytes()
    : mContent(nullptr),
      mSize(0)
    {
    }

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

    void RawBytes::Append(const void* data, std::size_t size)
    {
      if(mContent == nullptr)
      {
        mContent = static_cast<void*>(new char[size]);
        memcpy(mContent, data, size);
        mSize = size;
      }
      else
      {
        char* swapBuffer = new char[mSize + size];
        memcpy(swapBuffer, mContent, mSize);
        memcpy(&swapBuffer[mSize], data, size);

        delete[] static_cast<char*>(mContent);
        mContent = swapBuffer;
        mSize += size;
      }
    }
  } // namespace data
} // namespace lp
