import argparse
import datetime
import re
import matplotlib.pyplot as plt
import numpy as np

def parse_timestamp(line):
    match = re.search(r'Date and Time \(PST\): (\d{4}/\d{1,2}/\d{1,2} \d{1,2}:\d{1,2}:\d{1,2})', line)
    return datetime.datetime.strptime(match.group(1), '%Y/%m/%d %H:%M:%S') if match else None

def parse_temperature(line):
    match = re.search(r'Temperature: (\d+\.\d+)\*C', line)
    return float(match.group(1)) if match else None

def parse_capacitive(line):
    match = re.search(r'Capacitive: (\d+)', line)
    return int(match.group(1)) if match else None

def parse_log(file_path):
    """Parses a log file for timestamp, temperature, and capacitive data.

    Args:
        logfile_path (str): Path to the log file.

    Returns:
        list of tuples: Parsed data as a list of (timestamp, temperature, capacitive) tuples.
    """
    
    data = []
    try:
        with open(file_path, 'r') as file:
            lines = file.readlines()
    except FileNotFoundError:
        print(f"Error: File '{file_path}' not found.", file=sys.stderr)
        return []

    i = 0
    while i < len(lines):
        # check if enough lines in the file for a complete data point/tuple
        if i + 2 < len(lines):
            timestamp_match = re.search(r'Date and Time \(PST\): (\d{4}/\d{1,2}/\d{1,2} \d{1,2}:\d{1,2}:\d{1,2})', lines[i])
            temp_match = re.search(r'Temperature: (\d+\.\d+)\*C', lines[i + 1])
            cap_match = re.search(r'Capacitive: (\d+)', lines[i + 2])

            if timestamp_match and temp_match and cap_match:
                timestamp_str = timestamp_match.group(1)
                temperature = float(temp_match.group(1))
                capacitive = float(cap_match.group(1))

                timestamp = datetime.datetime.strptime(timestamp_str, '%Y/%m/%d %H:%M:%S')
                data.append((timestamp, temperature, capacitive))
            
            i += 3  # move to the next triplet of measurements
        else:
            break

    return data

def visualize_data(data):
    """Generates a plot of temperature and capacitive data over time.

    Args:
        data (list of tuples): List of (timestamp, temperature, capacitive) tuples.
    """

    timestamps, temperatures, capacitives = zip(*data)
    
    fig, ax1 = plt.subplots(figsize=(12, 6))
    ax1.plot(timestamps, temperatures, color='red', label='Temperature')
    ax1.set_xlabel('Time')
    ax1.set_ylabel('Temperature (°C)', color='red')
    ax1.tick_params('y', colors='red')

    ax2 = ax1.twinx()
    ax2.plot(timestamps, capacitives, color='blue', label='Capacitive')
    ax2.set_ylabel('Capacitive', color='blue')
    ax2.tick_params('y', colors='blue')

    fig.legend(loc='upper right')
    fig.suptitle('Soil Sensor Log Data')
    plt.show()

def main():
    parser = argparse.ArgumentParser(description="Parse and visualize the soil sensor log data.")
    parser.add_argument("logfile_path", type=str, help="Path to the log file")
    args = parser.parse_args()

    data = parse_log(args.logfile_path)
    visualize_data(data)

if __name__ == "__main__":
    main()