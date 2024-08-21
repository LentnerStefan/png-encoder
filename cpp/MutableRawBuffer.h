//
//  MutableRawBuffer.h
//  Taken from https://github.com/mrousavy/react-native-vision-camera/blob/edacc7b2f6ca0ddbfe1b245e0ca6d0e36d11d437/package/ios/FrameProcessors/MutableRawBuffer.h
//
#pragma once

#include <jsi/jsi.h>
#include <memory>

namespace vision {

using namespace facebook;

class MutableRawBuffer : public jsi::MutableBuffer {

public:
  explicit MutableRawBuffer(uint8_t* data, size_t size, bool freeOnDealloc) {
    _size = size;
    _data = data;
    _freeOnDealloc = freeOnDealloc;
  }
  explicit MutableRawBuffer(size_t size) : MutableRawBuffer(new uint8_t[size], size, true) {}

  ~MutableRawBuffer() {
    if (_freeOnDealloc) {
      delete[] _data;
    }
  }

public:
  uint8_t* data() override {
    return _data;
  }
  size_t size() const override {
    return _size;
  }

private:
  uint8_t* _data;
  size_t _size;
  bool _freeOnDealloc;
};

} // namespace vision
