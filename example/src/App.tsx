import { useEffect } from 'react';
import { StyleSheet, View, TouchableOpacity, Text } from 'react-native';
import { multiplyA } from 'react-native-png-encoder';
import {
  useCameraPermission,
  useCameraDevice,
  useCameraFormat,
  Camera,
} from 'react-native-vision-camera';

export default function App() {
  // TODO, remove me
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

  const handlePress = async () => {
    const result = multiplyA(6, 2);
    console.log('multiply result:', result);
  };

  return (
    <View style={styles.container}>
      <TouchableOpacity onPress={handlePress}>
        <Text style={{ marginTop: 64, textAlign: 'center', color: 'black' }}>
          Click me
        </Text>
      </TouchableOpacity>
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
