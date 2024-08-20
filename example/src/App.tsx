import { useEffect } from 'react';
import { StyleSheet, View } from 'react-native';
import { multiply } from 'react-native-png-encoder';
import {
  useCameraPermission,
  useCameraDevice,
  useCameraFormat,
  Camera,
} from 'react-native-vision-camera';

export default function App() {
  // TODO, remove me
  console.log(multiply);
  const permission = useCameraPermission();
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

  return (
    <View style={styles.container}>
      {permission.hasPermission && !!device && !!format && (
        <Camera
          style={styles.camera}
          isActive
          enableFpsGraph
          device={device}
          format={format}
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
