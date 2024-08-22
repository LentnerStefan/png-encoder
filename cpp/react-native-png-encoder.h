#ifndef PNGENCODER_H
#define PNGENCODER_H

#include <jsi/jsi.h>

void installPngEncoder(facebook::jsi::Runtime& jsiRuntime, const std::string& cachePath);

#endif /* PNGENCODER_H */
