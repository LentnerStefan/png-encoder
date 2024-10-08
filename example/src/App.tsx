import { useEffect } from 'react';
import { StyleSheet, View } from 'react-native';
import { saveRgbAsPng, deleteCacheFiles } from 'react-native-png-encoder';
import { useResizePlugin } from 'vision-camera-resize-plugin';
import {
  useCameraPermission,
  useCameraDevice,
  useCameraFormat,
  Camera,
  useFrameProcessor,
} from 'react-native-vision-camera';
import { useAppState } from './useAppState';

export default function App() {
  // TODO, remove me
  const permission = useCameraPermission();
  const appState = useAppState();

  useEffect(() => {
    if (appState !== 'active') {
      return;
    }
    const deletedFiles = deleteCacheFiles();
    console.log(`Deleted ${deletedFiles} cached files`);
  }, [appState]);

  useEffect(() => {
    permission.requestPermission();
  }, [permission]);

  // Vision camera hooks
  const device = useCameraDevice('back');
  const format = useCameraFormat(device, [
    {
      videoResolution: 'max',
    },
    { fps: 'max' },
  ]);

  const { resize } = useResizePlugin();

  const frameProcessor = useFrameProcessor(
    (frame) => {
      'worklet';

      const side = 504;
      const centerPoint = {
        x: frame.width / 2,
        y: frame.height / 2,
      };
      const cropSquare = {
        x: Math.round(centerPoint.x - side / 2),
        y: Math.round(centerPoint.y - side / 2),
        width: side,
        height: side,
      };

      const resizedFrame = resize(frame, {
        dataType: 'uint8',
        pixelFormat: 'rgb',
        scale: {
          width: cropSquare.width,
          height: cropSquare.height,
        },
        crop: {
          x: cropSquare.x,
          y: cropSquare.y,
          width: cropSquare.width,
          height: cropSquare.height,
        },
      });

      const start = performance.now();
      let pngFilePath: string = saveRgbAsPng(
        resizedFrame.buffer,
        cropSquare.width,
        cropSquare.height
      );
      console.log(
        `Took: ${Math.round(performance.now() - start)}ms to save to PNG at: ${pngFilePath}`
      );
    },
    [resize]
  );
  return (
    <View style={styles.container}>
      {permission.hasPermission && !!device && !!format && (
        <Camera
          style={styles.camera}
          isActive={appState === 'active'}
          enableFpsGraph
          device={device}
          format={format}
          frameProcessor={frameProcessor}
          resizeMode="contain"
        />
      )}
    </View>
  );
}

const styles = StyleSheet.create({
  container: {
    flex: 1,
  },
  camera: {
    flex: 1,
  },
});
