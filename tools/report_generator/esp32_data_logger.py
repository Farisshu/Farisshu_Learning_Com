#!/usr/bin/env python3
"""
ESP32 Serial Data Logger
Capture data dari Serial Monitor ESP32 dan save ke format JSON
untuk diproses oleh test_report_generator.py

Fitur:
- Auto-detect COM port
- Real-time monitoring
- Auto-save ke JSON setelah selesai
- Support multiple test sessions
"""

import serial
import serial.tools.list_ports
import json
import time
from datetime import datetime
from pathlib import Path
import re


class ESP32DataLogger:
    """Logger untuk capture data dari ESP32 Serial Monitor"""
    
    def __init__(self, port=None, baudrate=115200, timeout=1):
        self.port = port
        self.baudrate = baudrate
        self.timeout = timeout
        self.ser = None
        self.data_points = []
        self.is_recording = False
        
    def list_available_ports(self):
        """List semua available COM ports"""
        print("\n🔍 Available COM Ports:")
        print("-" * 60)
        ports = serial.tools.list_ports.comports()
        for i, port in enumerate(ports, 1):
            print(f"  [{i}] {port.device} - {port.description}")
        print("-" * 60)
        return ports
    
    def auto_detect_esp32(self):
        """Auto-detect ESP32 port"""
        ports = serial.tools.list_ports.comports()
        esp32_ports = [p for p in ports if 'ESP32' in p.description or 'CP210x' in p.description or 'CH340' in p.description]
        
        if esp32_ports:
            print(f"✅ ESP32 detected on: {esp32_ports[0].device}")
            return esp32_ports[0].device
        
        # Fallback: gunakan port pertama
        if ports:
            print(f"⚠️  ESP32 not auto-detected, using first available: {ports[0].device}")
            return ports[0].device
        
        return None
    
    def connect(self, port=None):
        """Connect ke ESP32"""
        if port is None:
            port = self.auto_detect_esp32()
        
        if port is None:
            print("❌ No COM port found!")
            return False
        
        try:
            self.ser = serial.Serial(port, self.baudrate, timeout=self.timeout)
            time.sleep(2)  # Wait for ESP32 to reset
            print(f"✅ Connected to {port}")
            self.port = port
            return True
        except Exception as e:
            print(f"❌ Connection failed: {e}")
            return False
    
    def disconnect(self):
        """Disconnect dari ESP32"""
        if self.ser and self.ser.is_open:
            self.ser.close()
            print("🔌 Disconnected")
    
    def parse_serial_line(self, line):
        """
        Parse line dari serial monitor
        Expected format: timestamp,rpm,voltage,current,temperature
        atau: RPM:1500,VOLT:12.1,CURR:0.45,TEMP:25.3
        """
        line = line.strip()
        if not line:
            return None
        
        # Format 1: CSV (timestamp,rpm,voltage,current,temp)
        csv_pattern = r'^(\d{2}:\d{2}:\d{2}),?(\d+\.?\d*),?(\d+\.?\d*),?(\d+\.?\d*),?(\d+\.?\d*)$'
        match = re.match(csv_pattern, line)
        if match:
            return {
                'timestamp': match.group(1),
                'rpm': float(match.group(2)),
                'voltage': float(match.group(3)),
                'current': float(match.group(4)),
                'temperature': float(match.group(5))
            }
        
        # Format 2: Key-Value (RPM:1500,VOLT:12.1,...)
        kv_pattern = r'RPM:(\d+\.?\d*).*VOLT:?(\d+\.?\d*).*CURR:?(\d+\.?\d*).*TEMP:?(\d+\.?\d*)'
        match = re.search(kv_pattern, line, re.IGNORECASE)
        if match:
            timestamp = datetime.now().strftime('%H:%M:%S')
            return {
                'timestamp': timestamp,
                'rpm': float(match.group(1)),
                'voltage': float(match.group(2)),
                'current': float(match.group(3)),
                'temperature': float(match.group(4))
            }
        
        # Format 3: Simple CSV tanpa timestamp
        simple_csv = r'^(\d+\.?\d*),?(\d+\.?\d*),?(\d+\.?\d*),?(\d+\.?\d*)$'
        match = re.match(simple_csv, line)
        if match:
            timestamp = datetime.now().strftime('%H:%M:%S')
            return {
                'timestamp': timestamp,
                'rpm': float(match.group(1)),
                'voltage': float(match.group(2)),
                'current': float(match.group(3)),
                'temperature': float(match.group(4))
            }
        
        return None
    
    def start_recording(self, duration_seconds=None, output_file=None):
        """
        Start recording data dari ESP32
        duration_seconds: Recording duration (None = manual stop with Ctrl+C)
        output_file: Output JSON file path
        """
        if not self.ser or not self.ser.is_open:
            print("❌ Not connected! Call connect() first.")
            return False
        
        self.data_points = []
        self.is_recording = True
        
        if output_file is None:
            timestamp = datetime.now().strftime('%Y%m%d_%H%M%S')
            output_file = f"esp32_data_{timestamp}.json"
        
        output_path = Path(output_file)
        
        print(f"\n🎬 Starting recording...")
        print(f"   Port: {self.port}")
        print(f"   Duration: {'Manual (Ctrl+C to stop)' if duration_seconds is None else f'{duration_seconds}s'}")
        print(f"   Output: {output_path}")
        print("\nPress Ctrl+C to stop recording...\n")
        
        start_time = time.time()
        
        try:
            while self.is_recording:
                # Check duration
                if duration_seconds and (time.time() - start_time) > duration_seconds:
                    print(f"\n⏱️  Duration reached ({duration_seconds}s)")
                    break
                
                # Read from serial
                if self.ser.in_waiting > 0:
                    line = self.ser.readline().decode('utf-8', errors='ignore')
                    parsed = self.parse_serial_line(line)
                    
                    if parsed:
                        self.data_points.append(parsed)
                        print(f"📊 [{len(self.data_points)}] RPM:{parsed['rpm']:.0f} | V:{parsed['voltage']:.2f} | A:{parsed['current']:.2f} | T:{parsed['temperature']:.1f}°C")
                
                time.sleep(0.1)
        
        except KeyboardInterrupt:
            print("\n⏹️  Recording stopped by user")
        
        # Save to JSON
        self.save_to_json(output_path)
        return True
    
    def save_to_json(self, output_path, test_name="ESP32 Data Log", device_id="ESP32_001", 
                     operator="Farisshu", target_rpm=1500, tolerance_percent=5):
        """Save recorded data to JSON format"""
        if not self.data_points:
            print("⚠️  No data to save!")
            return False
        
        # Calculate duration
        duration_minutes = len(self.data_points) / 2  # Approximate
        
        data = {
            'test_name': test_name,
            'device_id': device_id,
            'operator': operator,
            'date': datetime.now().strftime('%Y-%m-%d'),
            'duration_minutes': round(duration_minutes, 1),
            'target_rpm': target_rpm,
            'tolerance_percent': tolerance_percent,
            'data_points': self.data_points
        }
        
        # Ensure output directory exists
        output_path = Path(output_path)
        output_path.parent.mkdir(parents=True, exist_ok=True)
        
        with open(output_path, 'w') as f:
            json.dump(data, f, indent=2)
        
        print(f"\n💾 Data saved to: {output_path}")
        print(f"   Total data points: {len(self.data_points)}")
        print(f"   Duration: ~{duration_minutes:.1f} minutes")
        
        return True
    
    def simulate_data(self, num_points=10, noise_level=0.02):
        """Simulate data untuk testing tanpa ESP32"""
        import random
        
        print(f"\n🎭 Simulating {num_points} data points...")
        
        base_rpm = 1500
        base_voltage = 12.0
        base_current = 0.45
        base_temp = 25.0
        
        self.data_points = []
        
        for i in range(num_points):
            timestamp = f"{i//60:02d}:{i%60:02d}:00"
            
            # Add some noise and drift
            rpm = int(base_rpm + random.uniform(-50, 50) + (i * 0.5))
            voltage = round(base_voltage + random.uniform(-0.3, 0.3), 1)
            current = round(base_current + random.uniform(-0.05, 0.05) + (i * 0.001), 2)
            temperature = round(base_temp + (i * 0.3) + random.uniform(-0.5, 0.5), 1)
            
            self.data_points.append({
                'timestamp': timestamp,
                'rpm': rpm,
                'voltage': voltage,
                'current': current,
                'temperature': temperature
            })
        
        print("✅ Simulation complete")
        return self.data_points


def main():
    """Main function dengan interactive menu"""
    import sys
    
    print("=" * 60)
    print("📡 ESP32 Serial Data Logger")
    print("=" * 60)
    
    logger = ESP32DataLogger()
    
    # Mode: simulate, record, atau help
    if len(sys.argv) > 1:
        mode = sys.argv[1].lower()
        
        if mode == 'simulate':
            # Simulation mode
            num_points = int(sys.argv[2]) if len(sys.argv) > 2 else 10
            logger.simulate_data(num_points)
            logger.save_to_json(
                output_path=f"sample_data/simulated_test_{datetime.now().strftime('%Y%m%d_%H%M%S')}.json",
                test_name="Simulated Motor Test",
                device_id="ESP32_SIM"
            )
        
        elif mode == 'record':
            # Record mode
            duration = int(sys.argv[2]) if len(sys.argv) > 2 else None
            port = sys.argv[3] if len(sys.argv) > 3 else None
            
            if logger.connect(port):
                logger.start_recording(duration_seconds=duration)
                logger.disconnect()
        
        elif mode == 'list':
            # List ports
            logger.list_available_ports()
        
        else:
            print(f"❌ Unknown mode: {mode}")
            print("Usage:")
            print("  python esp32_data_logger.py simulate [num_points]")
            print("  python esp32_data_logger.py record [duration] [port]")
            print("  python esp32_data_logger.py list")
    else:
        # Interactive mode
        print("\nSelect mode:")
        print("  [1] 🎭 Simulate data (testing)")
        print("  [2] 📡 Record from ESP32")
        print("  [3] 🔍 List available ports")
        print("  [4] ❌ Exit")
        
        choice = input("\nEnter choice (1-4): ").strip()
        
        if choice == '1':
            num = input("Number of data points (default 10): ").strip()
            num = int(num) if num else 10
            logger.simulate_data(num)
            logger.save_to_json(
                output_path=f"sample_data/simulated_test_{datetime.now().strftime('%Y%m%d_%H%M%S')}.json",
                test_name="Simulated Motor Test"
            )
        
        elif choice == '2':
            logger.list_available_ports()
            port = input("Enter COM port (or press Enter for auto-detect): ").strip()
            port = port if port else None
            
            if logger.connect(port):
                duration = input("Recording duration in seconds (or press Enter for manual): ").strip()
                duration = int(duration) if duration else None
                logger.start_recording(duration_seconds=duration)
                logger.disconnect()
        
        elif choice == '3':
            logger.list_available_ports()
        
        elif choice == '4':
            print("👋 Goodbye!")
        
        else:
            print("❌ Invalid choice")


if __name__ == "__main__":
    main()
