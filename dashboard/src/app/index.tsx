
import 'react-native-get-random-values';
import 'react-native-url-polyfill/auto';

import { Buffer } from 'buffer';
import mqtt from 'mqtt';
import { useEffect, useRef, useState } from 'react';

import {
  ScrollView,
  StyleSheet,
  Text,
  TextInput,
  TouchableOpacity,
  View
} from 'react-native';

import { SafeAreaView } from 'react-native-safe-area-context';

(global as any).Buffer = (global as any).Buffer || Buffer;

const BROKER = 'ws://broker.hivemq.com:8000/mqtt';

const TOPIC = {
  telemetry: 'smartroom/sr_yudhi_20261107/telemetry',
  status: 'smartroom/sr_yudhi_20261107/status',
  command: 'smartroom/sr_yudhi_20261107/command',
};

type SensorData = {
  temperature: number;
  humidity: number;
  light: number;
  motion: number;
  buzzer: boolean;
};

export default function HomeScreen() {
  const clientRef = useRef<any>(null);

  const [connection, setConnection] = useState('Connecting');
  const [deviceStatus, setDeviceStatus] = useState('Unknown');

  const [thresholdInput, setThresholdInput] = useState('30');
  const [thresholdMessage, setThresholdMessage] = useState('');

  const [data, setData] = useState<SensorData>({
    temperature: 0,
    humidity: 0,
    light: 0,
    motion: 0,
    buzzer: false,
  });

  useEffect(() => {
    const client = mqtt.connect(BROKER, {
      clientId:
        'Dashboard_Yudhi_' +
        Math.random().toString(16).slice(2, 10),
      clean: true,
      connectTimeout: 10000,
      reconnectPeriod: 3000,
    });

    clientRef.current = client;

    client.on('connect', () => {
      setConnection('Connected');

      client.subscribe([
        TOPIC.telemetry,
        TOPIC.status,
      ]);
    });

    client.on('message', (topic, message) => {
      const payload = message.toString();

      if (topic === TOPIC.telemetry) {
        try {
          const json = JSON.parse(payload);

          setData({
            temperature: Number(
              json.temperature ?? 0
            ),
            humidity: Number(
              json.humidity ?? 0
            ),
            light: Number(
              json.light ?? 0
            ),
            motion: Number(
              json.motion ?? 0
            ),
            buzzer: Boolean(
              json.buzzer ?? false
            ),
          });
        } catch (error) {
          console.log('Telemetry JSON error:', error);
        }
      }

      if (topic === TOPIC.status) {
        setDeviceStatus(payload.trim());
      }
    });

    client.on('reconnect', () => {
      setConnection('Reconnecting');
    });

    client.on('offline', () => {
      setConnection('Disconnected');
    });

    client.on('error', error => {
      console.log('MQTT Error:', error);
    });

    return () => {
      clientRef.current = null;
      client.end();
    };
  }, []);

  const updateThreshold = () => {
    const client = clientRef.current;

    if (!client || !client.connected) {
      setThresholdMessage('MQTT belum terhubung!');
      return;
    }

    const value = Number(thresholdInput);

    if (
      thresholdInput.trim() === '' ||
      !Number.isFinite(value) ||
      value <= 0 ||
      value > 100
    ) {
      setThresholdMessage(
        'Masukkan threshold antara 0–100 °C.'
      );
      return;
    }

    const payload = JSON.stringify({
      threshold: value,
    });

    client.publish(
      TOPIC.command,
      payload,
      { qos: 0 },
      (error: Error | undefined) => {
        if (error) {
          setThresholdMessage(
            'Gagal mengirim threshold!'
          );
        } else {
          setThresholdMessage(
            `Perintah threshold ${value} °C terkirim ke broker.`
          );
        }
      }
    );
  };

  const SensorCard = ({
    title,
    value,
    unit,
    color,
  }: {
    title: string;
    value: string | number;
    unit: string;
    color: string;
  }) => (
    <View style={styles.card}>
      <Text style={styles.cardTitle}>{title}</Text>

      <Text style={[styles.cardValue, { color }]}>
        {value}
      </Text>

      <Text style={styles.unit}>{unit}</Text>
    </View>
  );

  return (
    <SafeAreaView style={styles.container}>
      <ScrollView showsVerticalScrollIndicator={false}>
        <Text style={styles.heading}>
          Smart Room Monitor
        </Text>

        <Text style={styles.subtitle}>
          Real-time MQTT Dashboard
        </Text>

        <View style={styles.statusBox}>
          <Text style={styles.sectionTitle}>
            Connection Status
          </Text>

          <Text style={styles.statusText}>
            MQTT: {connection}
          </Text>

          <Text style={styles.statusText}>
            ESP32: {deviceStatus}
          </Text>
        </View>

        <Text style={styles.sectionTitle}>
          Sensor Monitoring
        </Text>

        <View style={styles.grid}>
          <SensorCard
            title="Temperature"
            value={data.temperature.toFixed(1)}
            unit="°C"
            color="#F97316"
          />

          <SensorCard
            title="Humidity"
            value={data.humidity.toFixed(1)}
            unit="%"
            color="#38BDF8"
          />

          <SensorCard
            title="Light Intensity"
            value={data.light}
            unit="LDR"
            color="#FACC15"
          />

          <SensorCard
            title="Motion"
            value={data.motion ? 'Ada Gerakan' : 'Tidak Ada Gerakan'}
            unit="PIR Sensor"
            color="#A78BFA"
          />
        </View>

        <View style={styles.buzzerBox}>
          <Text style={styles.sectionTitle}>
            Alarm Status
          </Text>

          <Text
            style={{
              color: data.buzzer ? '#F87171' : '#4ADE80',
              fontSize: 25,
              fontWeight: 'bold',
            }}
          >
            BUZZER {data.buzzer ? 'ON' : 'OFF'}
          </Text>
        </View>

        <View style={styles.thresholdBox}>
          <Text style={styles.sectionTitle}>
            Temperature Threshold
          </Text>

          <Text style={styles.thresholdDescription}>
            Atur batas suhu untuk mengaktifkan alarm.
          </Text>

          <View style={styles.inputRow}>
            <TextInput
              style={styles.thresholdInput}
              value={thresholdInput}
              onChangeText={setThresholdInput}
              keyboardType="decimal-pad"
              placeholder="Masukkan suhu"
              placeholderTextColor="#64748B"
            />

            <Text style={styles.degree}>°C</Text>
          </View>

          <TouchableOpacity
            style={[
              styles.updateButton,
              connection !== 'Connected' &&
                styles.disabledButton
            ]}
            onPress={updateThreshold}
            disabled={connection !== 'Connected'}
          >
            <Text style={styles.buttonText}>
              Update Threshold
            </Text>
          </TouchableOpacity>

          {thresholdMessage !== '' && (
            <Text style={styles.thresholdMessage}>
              {thresholdMessage}
            </Text>
          )}
        </View>

        <Text style={styles.footer}>
          Smart Room Monitor • MQTT
        </Text>
      </ScrollView>
    </SafeAreaView>
  );
}

const styles = StyleSheet.create({
  container: {
    flex: 1,
    backgroundColor: '#101827',
    paddingHorizontal: 18,
  },
  heading: {
    fontSize: 27,
    fontWeight: 'bold',
    color: '#FFFFFF',
    marginTop: 20,
  },
  subtitle: {
    color: '#94A3B8',
    marginTop: 5,
    marginBottom: 25,
  },
  sectionTitle: {
    color: '#FFFFFF',
    fontSize: 18,
    fontWeight: 'bold',
    marginBottom: 14,
  },
  statusBox: {
    backgroundColor: '#1E293B',
    padding: 18,
    borderRadius: 14,
    marginBottom: 25,
  },
  statusText: {
    color: '#4ADE80',
    fontSize: 15,
    marginTop: 8,
  },
  grid: {
    flexDirection: 'row',
    flexWrap: 'wrap',
    justifyContent: 'space-between',
  },
  card: {
    width: '48%',
    backgroundColor: '#1E293B',
    padding: 18,
    borderRadius: 14,
    marginBottom: 14,
  },
  cardTitle: {
    color: '#94A3B8',
    fontSize: 13,
  },
  cardValue: {
    fontSize: 25,
    fontWeight: 'bold',
    marginTop: 15,
  },
  unit: {
    color: '#94A3B8',
    fontSize: 12,
    marginTop: 5,
  },
  buzzerBox: {
    backgroundColor: '#1E293B',
    padding: 18,
    borderRadius: 14,
    marginTop: 5,
  },
  thresholdBox: {
    backgroundColor: '#1E293B',
    padding: 18,
    borderRadius: 14,
    marginTop: 16,
  },
  thresholdDescription: {
    color: '#94A3B8',
    fontSize: 13,
    marginBottom: 15,
  },
  inputRow: {
    flexDirection: 'row',
    alignItems: 'center',
    marginBottom: 15,
  },
  thresholdInput: {
    flex: 1,
    backgroundColor: '#0F172A',
    borderWidth: 1,
    borderColor: '#475569',
    borderRadius: 10,
    padding: 13,
    color: '#FFFFFF',
    fontSize: 18,
  },
  degree: {
    color: '#FFFFFF',
    fontSize: 18,
    marginLeft: 12,
  },
  updateButton: {
    backgroundColor: '#2563EB',
    padding: 15,
    borderRadius: 10,
    alignItems: 'center',
  },
  disabledButton: {
    opacity: 0.5,
  },
  buttonText: {
    color: '#FFFFFF',
    fontSize: 15,
    fontWeight: 'bold',
  },
  thresholdMessage: {
    color: '#38BDF8',
    fontSize: 13,
    marginTop: 12,
  },
  footer: {
    color: '#64748B',
    textAlign: 'center',
    marginVertical: 25,
  },
});
