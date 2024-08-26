import { useState, useEffect } from 'react';
import { AppState, type AppStateStatus } from 'react-native';

/** Removes the created temporary files when on first app load and when app goes to the background */
export const useAppState = () => {
  const [appState, setAppState] = useState<AppStateStatus>('unknown');

  useEffect(() => {
    const subscription = AppState.addEventListener('change', (nextAppState) => {
      setAppState(nextAppState);
    });
    return () => {
      subscription.remove();
    };
  }, []);
  return appState;
};
