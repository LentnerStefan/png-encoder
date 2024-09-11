import { useEffect } from 'react';
import { StyleSheet, View } from 'react-native';
import {
  saveRgbAsPng,
  detectFromRgb,
  deleteCacheFiles,
} from 'react-native-png-encoder';
import { useResizePlugin } from 'vision-camera-resize-plugin';
import {
  useCameraPermission,
  useCameraDevice,
  useCameraFormat,
  Camera,
  useFrameProcessor,
  runAtTargetFps,
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
  // const device = useCameraDevice('back', {
  //   physicalDevices: [
  //     'ultra-wide-angle-camera',
  //     'wide-angle-camera',
  //     'telephoto-camera',
  //   ],
  // });
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
      // runAtTargetFps(1, () => {
      //   'worklet';
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
      const detectionResult = detectFromRgb(
        resizedFrame.buffer,
        cropSquare.width,
        cropSquare.height
      );
      const end = performance.now();
      if (detectionResult === 'Found a barcode') {
        console.log(`${detectionResult} in ${Math.round(end - start)}ms`);
      }
      // let pngFilePath: string = saveRgbAsPng(
      //   resizedFrame.buffer,
      //   cropSquare.width,
      //   cropSquare.height
      // );
      // });
    },
    [resize]
  );
  return (
    <View style={styles.container}>
      {permission.hasPermission && !!device && !!format && (
        <Camera
          fps={[format.minFps, format.maxFps]}
          style={styles.camera}
          isActive={appState === 'active'}
          enableFpsGraph
          zoom={device.neutralZoom}
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
