# plant-care-automation

## Running the serial monitoring for Arduino Uno locally
TERM=xterm-256color screen -L /dev/cu.usbmodem1101 115200

## Using the serial_monitor_parser script for parsing and visualizing the log file for the soil sensor
python3 serial_monitor_parser.py /path/to/logfile