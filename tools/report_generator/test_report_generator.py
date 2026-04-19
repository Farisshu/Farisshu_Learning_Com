#!/usr/bin/env python3
"""
Test Report Generator for Embedded Systems Portfolio
Generates professional PDF/CSV reports from test data (JSON format)

Fitur:
- Membaca data test dari file JSON
- Menghitung statistik (mean, min, max, std dev)
- Mengevaluasi pass/fail berdasarkan toleransi
- Generate laporan PDF profesional dengan grafik
- Export ke CSV untuk analisis lebih lanjut

Cocok untuk portofolio Embedded Engineer (Horiba, dll)
"""

import json
import csv
import os
from datetime import datetime
from pathlib import Path
from statistics import mean, stdev

# Built-in functions
min = min
max = max

# Cek apakah library yang diperlukan terinstall
try:
    from reportlab.lib.pagesizes import A4
    from reportlab.lib import colors
    from reportlab.lib.styles import getSampleStyleSheet, ParagraphStyle
    from reportlab.platypus import SimpleDocTemplate, Table, TableStyle, Paragraph, Spacer, Image
    from reportlab.lib.units import inch, cm
    from reportlab.graphics.shapes import Drawing
    from reportlab.graphics.charts.linecharts import HorizontalLineChart
    REPORTLAB_AVAILABLE = True
except ImportError:
    REPORTLAB_AVAILABLE = False
    print("⚠️  Warning: reportlab tidak terinstall. Fitur PDF akan dinonaktifkan.")
    print("   Install dengan: pip install reportlab")

try:
    import matplotlib.pyplot as plt
    import matplotlib.dates as mdates
    from matplotlib.backends.backend_pdf import PdfPages
    MATPLOTLIB_AVAILABLE = True
except ImportError:
    MATPLOTLIB_AVAILABLE = False
    print("⚠️  Warning: matplotlib tidak terinstall. Fitur grafik akan dinonaktifkan.")
    print("   Install dengan: pip install matplotlib")


class TestReportGenerator:
    """Generator laporan test profesional untuk proyek embedded"""
    
    def __init__(self, input_json_path: str, output_dir: str = "output"):
        self.input_path = Path(input_json_path)
        self.output_dir = Path(output_dir)
        self.output_dir.mkdir(parents=True, exist_ok=True)
        
        # Load data
        with open(self.input_path, 'r') as f:
            self.data = json.load(f)
        
        self.test_name = self.data.get('test_name', 'Unknown Test')
        self.device_id = self.data.get('device_id', 'N/A')
        self.operator = self.data.get('operator', 'N/A')
        self.date = self.data.get('date', datetime.now().strftime('%Y-%m-%d'))
        self.data_points = self.data.get('data_points', [])
        
        # Extract metrics
        self.rpm_values = [dp['rpm'] for dp in self.data_points]
        self.voltage_values = [dp['voltage'] for dp in self.data_points]
        self.current_values = [dp['current'] for dp in self.data_points]
        self.temp_values = [dp['temperature'] for dp in self.data_points]
        
    def calculate_statistics(self):
        """Hitung statistik untuk setiap metric"""
        stats = {}
        
        # RPM Statistics
        stats['rpm'] = {
            'mean': mean(self.rpm_values),
            'min': min(self.rpm_values),
            'max': max(self.rpm_values),
            'std_dev': stdev(self.rpm_values) if len(self.rpm_values) > 1 else 0,
            'target': self.data.get('target_rpm', 0),
            'tolerance': self.data.get('tolerance_percent', 5)
        }
        
        # Voltage Statistics
        stats['voltage'] = {
            'mean': mean(self.voltage_values),
            'min': min(self.voltage_values),
            'max': max(self.voltage_values),
            'std_dev': stdev(self.voltage_values) if len(self.voltage_values) > 1 else 0
        }
        
        # Current Statistics
        stats['current'] = {
            'mean': mean(self.current_values),
            'min': min(self.current_values),
            'max': max(self.current_values),
            'std_dev': stdev(self.current_values) if len(self.current_values) > 1 else 0
        }
        
        # Temperature Statistics
        stats['temperature'] = {
            'mean': mean(self.temp_values),
            'min': min(self.temp_values),
            'max': max(self.temp_values),
            'std_dev': stdev(self.temp_values) if len(self.temp_values) > 1 else 0
        }
        
        return stats
    
    def evaluate_pass_fail(self, stats):
        """Evaluasi apakah test PASS atau FAIL berdasarkan toleransi"""
        target = stats['rpm']['target']
        tolerance = stats['rpm']['tolerance']
        min_allowed = target * (1 - tolerance/100)
        max_allowed = target * (1 + tolerance/100)
        
        all_within_tolerance = all(
            min_allowed <= rpm <= max_allowed 
            for rpm in self.rpm_values
        )
        
        return {
            'result': 'PASS' if all_within_tolerance else 'FAIL',
            'target': target,
            'tolerance_percent': tolerance,
            'min_allowed': min_allowed,
            'max_allowed': max_allowed,
            'actual_min': stats['rpm']['min'],
            'actual_max': stats['rpm']['max']
        }
    
    def export_to_csv(self):
        """Export data points ke CSV"""
        csv_path = self.output_dir / f"{self.input_path.stem}_report.csv"
        
        with open(csv_path, 'w', newline='') as f:
            writer = csv.writer(f)
            
            # Header
            writer.writerow(['Timestamp', 'RPM', 'Voltage (V)', 'Current (A)', 'Temperature (°C)'])
            
            # Data
            for dp in self.data_points:
                writer.writerow([
                    dp['timestamp'],
                    dp['rpm'],
                    dp['voltage'],
                    dp['current'],
                    dp['temperature']
                ])
            
            # Add statistics section
            writer.writerow([])
            writer.writerow(['=== STATISTICS ==='])
            stats = self.calculate_statistics()
            writer.writerow(['Metric', 'Mean', 'Min', 'Max', 'Std Dev'])
            writer.writerow(['RPM', f"{stats['rpm']['mean']:.2f}", f"{stats['rpm']['min']:.2f}", 
                           f"{stats['rpm']['max']:.2f}", f"{stats['rpm']['std_dev']:.2f}"])
            writer.writerow(['Voltage', f"{stats['voltage']['mean']:.2f}", f"{stats['voltage']['min']:.2f}", 
                           f"{stats['voltage']['max']:.2f}", f"{stats['voltage']['std_dev']:.2f}"])
            writer.writerow(['Current', f"{stats['current']['mean']:.2f}", f"{stats['current']['min']:.2f}", 
                           f"{stats['current']['max']:.2f}", f"{stats['current']['std_dev']:.2f}"])
            writer.writerow(['Temperature', f"{stats['temperature']['mean']:.2f}", f"{stats['temperature']['min']:.2f}", 
                           f"{stats['temperature']['max']:.2f}", f"{stats['temperature']['std_dev']:.2f}"])
        
        print(f"✅ CSV report saved to: {csv_path}")
        return csv_path
    
    def generate_pdf_report(self):
        """Generate PDF report profesional"""
        if not REPORTLAB_AVAILABLE:
            print("❌ Cannot generate PDF: reportlab not installed")
            return None
        
        pdf_path = self.output_dir / f"{self.input_path.stem}_report.pdf"
        doc = SimpleDocTemplate(str(pdf_path), pagesize=A4)
        styles = getSampleStyleSheet()
        elements = []
        
        # Title
        title_style = ParagraphStyle(
            'CustomTitle',
            parent=styles['Heading1'],
            fontSize=18,
            textColor=colors.HexColor('#1a1a2e'),
            spaceAfter=12
        )
        elements.append(Paragraph(f"Test Report: {self.test_name}", title_style))
        
        # Info table
        info_data = [
            ['Device ID', self.device_id],
            ['Operator', self.operator],
            ['Test Date', self.date],
            ['Data Points', str(len(self.data_points))]
        ]
        info_table = Table(info_data, colWidths=[2*inch, 3*inch])
        info_table.setStyle(TableStyle([
            ('BACKGROUND', (0, 0), (0, -1), colors.HexColor('#e8e8e8')),
            ('TEXTCOLOR', (0, 0), (-1, -1), colors.black),
            ('ALIGN', (0, 0), (-1, -1), 'LEFT'),
            ('FONTNAME', (0, 0), (-1, -1), 'Helvetica'),
            ('FONTSIZE', (0, 0), (-1, -1), 10),
            ('BOTTOMPADDING', (0, 0), (-1, -1), 6),
            ('TOPPADDING', (0, 0), (-1, -1), 6),
            ('GRID', (0, 0), (-1, -1), 0.5, colors.grey)
        ]))
        elements.append(info_table)
        elements.append(Spacer(1, 20))
        
        # Statistics
        stats = self.calculate_statistics()
        eval_result = self.evaluate_pass_fail(stats)
        
        # Pass/Fail Badge
        result_color = colors.green if eval_result['result'] == 'PASS' else colors.red
        result_style = ParagraphStyle(
            'ResultBadge',
            parent=styles['Heading2'],
            fontSize=16,
            textColor=result_color,
            spaceAfter=12
        )
        elements.append(Paragraph(f"Test Result: {eval_result['result']}", result_style))
        
        # Stats table
        stats_data = [
            ['Metric', 'Mean', 'Min', 'Max', 'Std Dev'],
            ['RPM', f"{stats['rpm']['mean']:.2f}", f"{stats['rpm']['min']:.2f}", 
             f"{stats['rpm']['max']:.2f}", f"{stats['rpm']['std_dev']:.2f}"],
            ['Voltage (V)', f"{stats['voltage']['mean']:.2f}", f"{stats['voltage']['min']:.2f}", 
             f"{stats['voltage']['max']:.2f}", f"{stats['voltage']['std_dev']:.2f}"],
            ['Current (A)', f"{stats['current']['mean']:.2f}", f"{stats['current']['min']:.2f}", 
             f"{stats['current']['max']:.2f}", f"{stats['current']['std_dev']:.2f}"],
            ['Temp (°C)', f"{stats['temperature']['mean']:.2f}", f"{stats['temperature']['min']:.2f}", 
             f"{stats['temperature']['max']:.2f}", f"{stats['temperature']['std_dev']:.2f}"]
        ]
        stats_table = Table(stats_data, colWidths=[1.2*inch, 1*inch, 1*inch, 1*inch, 1*inch])
        stats_table.setStyle(TableStyle([
            ('BACKGROUND', (0, 0), (-1, 0), colors.HexColor('#1a1a2e')),
            ('TEXTCOLOR', (0, 0), (-1, 0), colors.white),
            ('ALIGN', (0, 0), (-1, -1), 'CENTER'),
            ('FONTNAME', (0, 0), (-1, 0), 'Helvetica-Bold'),
            ('FONTSIZE', (0, 0), (-1, -1), 9),
            ('BOTTOMPADDING', (0, 0), (-1, -1), 6),
            ('TOPPADDING', (0, 0), (-1, -1), 6),
            ('GRID', (0, 0), (-1, -1), 0.5, colors.grey),
            ('ROWBACKGROUNDS', (0, 1), (-1, -1), [colors.white, colors.HexColor('#f5f5f5')])
        ]))
        elements.append(stats_table)
        elements.append(Spacer(1, 20))
        
        # Tolerance evaluation
        tol_data = [
            ['Parameter', 'Value'],
            ['Target RPM', f"{eval_result['target']}"],
            ['Tolerance', f"±{eval_result['tolerance_percent']}%"],
            ['Allowed Range', f"{eval_result['min_allowed']:.1f} - {eval_result['max_allowed']:.1f}"],
            ['Actual Range', f"{eval_result['actual_min']} - {eval_result['actual_max']}"]
        ]
        tol_table = Table(tol_data, colWidths=[2*inch, 2*inch])
        tol_table.setStyle(TableStyle([
            ('BACKGROUND', (0, 0), (-1, 0), colors.HexColor('#16213e')),
            ('TEXTCOLOR', (0, 0), (-1, 0), colors.white),
            ('ALIGN', (0, 0), (-1, -1), 'LEFT'),
            ('FONTNAME', (0, 0), (-1, 0), 'Helvetica-Bold'),
            ('FONTSIZE', (0, 0), (-1, -1), 9),
            ('BOTTOMPADDING', (0, 0), (-1, -1), 6),
            ('TOPPADDING', (0, 0), (-1, -1), 6),
            ('GRID', (0, 0), (-1, -1), 0.5, colors.grey)
        ]))
        elements.append(Paragraph("Tolerance Evaluation:", styles['Heading3']))
        elements.append(tol_table)
        
        # Build PDF
        doc.build(elements)
        print(f"✅ PDF report saved to: {pdf_path}")
        return pdf_path
    
    def generate_plots(self):
        """Generate grafik dengan matplotlib"""
        if not MATPLOTLIB_AVAILABLE:
            print("❌ Cannot generate plots: matplotlib not installed")
            return None
        
        # Create timestamps (dummy, karena data sample hanya string)
        n_points = len(self.data_points)
        x_values = list(range(n_points))
        
        # Create figure with multiple subplots
        fig, axes = plt.subplots(4, 1, figsize=(10, 12))
        fig.suptitle(f'{self.test_name}\nDevice: {self.device_id}', fontsize=14, fontweight='bold')
        
        # RPM Plot
        axes[0].plot(x_values, self.rpm_values, 'b-o', linewidth=2, markersize=4)
        axes[0].axhline(y=self.data.get('target_rpm', 0), color='g', linestyle='--', label='Target')
        tolerance = self.data.get('tolerance_percent', 5)
        target = self.data.get('target_rpm', 0)
        axes[0].axhspan(target*(1-tolerance/100), target*(1+tolerance/100), alpha=0.2, color='green', label='Tolerance Zone')
        axes[0].set_ylabel('RPM')
        axes[0].set_title('Motor Speed')
        axes[0].legend(loc='upper right')
        axes[0].grid(True, alpha=0.3)
        
        # Voltage Plot
        axes[1].plot(x_values, self.voltage_values, 'r-s', linewidth=2, markersize=4)
        axes[1].set_ylabel('Voltage (V)')
        axes[1].set_title('Supply Voltage')
        axes[1].grid(True, alpha=0.3)
        
        # Current Plot
        axes[2].plot(x_values, self.current_values, 'g-^', linewidth=2, markersize=4)
        axes[2].set_ylabel('Current (A)')
        axes[2].set_title('Motor Current')
        axes[2].grid(True, alpha=0.3)
        
        # Temperature Plot
        axes[3].plot(x_values, self.temp_values, 'm-d', linewidth=2, markersize=4)
        axes[3].set_ylabel('Temperature (°C)')
        axes[3].set_xlabel('Sample Index')
        axes[3].set_title('Motor Temperature')
        axes[3].grid(True, alpha=0.3)
        
        plt.tight_layout()
        
        # Save plot
        plot_path = self.output_dir / f"{self.input_path.stem}_plot.png"
        plt.savefig(plot_path, dpi=150, bbox_inches='tight')
        print(f"✅ Plot saved to: {plot_path}")
        
        plt.close()
        return plot_path
    
    def generate_full_report(self):
        """Generate semua format laporan (CSV, PDF, Plot)"""
        print(f"\n📊 Generating report for: {self.test_name}")
        print("=" * 60)
        
        # Calculate and display statistics
        stats = self.calculate_statistics()
        eval_result = self.evaluate_pass_fail(stats)
        
        print(f"\n📈 STATISTICS:")
        print(f"   RPM:     Mean={stats['rpm']['mean']:.2f}, Min={stats['rpm']['min']}, Max={stats['rpm']['max']}, StdDev={stats['rpm']['std_dev']:.2f}")
        print(f"   Voltage: Mean={stats['voltage']['mean']:.2f}V, Range={stats['voltage']['min']}-{stats['voltage']['max']}V")
        print(f"   Current: Mean={stats['current']['mean']:.2f}A, Range={stats['current']['min']}-{stats['current']['max']}A")
        print(f"   Temp:    Mean={stats['temperature']['mean']:.2f}°C, Range={stats['temperature']['min']}-{stats['temperature']['max']}°C")
        
        print(f"\n🎯 EVALUATION:")
        print(f"   Target: {eval_result['target']} RPM ±{eval_result['tolerance_percent']}%")
        print(f"   Allowed Range: {eval_result['min_allowed']:.1f} - {eval_result['max_allowed']:.1f} RPM")
        print(f"   Actual Range: {eval_result['actual_min']} - {eval_result['actual_max']} RPM")
        print(f"   Result: {eval_result['result']} {'✅' if eval_result['result'] == 'PASS' else '❌'}")
        
        print(f"\n💾 EXPORTING:")
        
        # Export to CSV
        self.export_to_csv()
        
        # Generate plots
        self.generate_plots()
        
        # Generate PDF
        self.generate_pdf_report()
        
        print("\n" + "=" * 60)
        print(f"✅ Report generation complete!")
        print(f"   Output directory: {self.output_dir.absolute()}")
        

def main():
    """Main function untuk demo"""
    import sys
    
    # Default: gunakan sample data
    if len(sys.argv) < 2:
        sample_data_path = Path(__file__).parent / "sample_data" / "motor_test_001.json"
        print(f"📝 Using sample data: {sample_data_path}")
    else:
        sample_data_path = Path(sys.argv[1])
    
    if not sample_data_path.exists():
        print(f"❌ File not found: {sample_data_path}")
        sys.exit(1)
    
    # Generate report
    generator = TestReportGenerator(str(sample_data_path))
    generator.generate_full_report()


if __name__ == "__main__":
    main()
