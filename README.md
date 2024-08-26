# react-native-png-encoder

A simple React Native library for encoding RGB buffers into PNG files.

## Installation

```sh
npm install react-native-png-encoder

yarn add react-native-png-encoder
```

## Usage

### Save an RGB buffer to an PNG file.

_Saves an RGB buffer (array of color values) as a PNG file and returns the file path where the image is stored._

```js
import { saveRgbAsPng } from 'react-native-png-encoder';

// ...

const savedFilePath = saveRgbAsPng(RGBBuffer);

// Do something with the returned file path
```

> [!NOTE]
> You can use this function in conjunction with [vision-camera-resize-plugin](https://github.com/mrousavy/vision-camera-resize-plugin)!

### Clear the cache folder containing the previously saved PNG files.

_Clears the cache folder by deleting previously saved PNG files and returns the count of deleted files._

```js
import { deleteCacheFiles } from 'react-native-png-encoder';

// ...

const filesDeletedCount = deleteCacheFiles();
```

> [!NOTE]
> I'd recommend on calling this function regularly (I.E. when the app becomes active), especially if you save a lot of data.

## Contributing

See the [contributing guide](CONTRIBUTING.md) to learn how to contribute to the repository and the development workflow.

## License

MIT

---

Made with [create-react-native-library](https://github.com/callstack/react-native-builder-bob)
