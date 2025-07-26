PORT="/dev/ttyACM0"

# arduino --board arduino:avr:nano:cpu=atmega328old --port $PORT --verbose-build --verbose-upload --upload $1
arduino --board arduino:avr:megaADK --port $PORT --verbose-build --verbose-upload --upload $1
