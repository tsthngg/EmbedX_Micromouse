# List of components

## 2 Encoders w/ motors
- Encoder 1 (left): 3 pin & 2 power pins
    |Pin|GPIO|Note|
    |VCC|5V|Highest posible|
    |GND|GND|Connect w/ others|
    |M1|G26||
    |M2|G27||
    |C1|G34||
    |C2|G35||

- Encoder 2 (right): 3 pin & 2 power pins
    |Pin|GPIO|Note|
    |VCC|5V|Highest posible|
    |GND|GND|Connect w/ others|
    |M1|G12||
    |M2|G13||
    |C1|G32||
    |C2|G33||

## 1 L238V3
|Pin|GPIO|Note|
|ENA|G18||
|ENB|G19||

## 1 MPU6050
|Pin|GPIO|Note|
|VCC|3V3|Connect w/ others|
|GND|GND|Connect w/ others|
|SCL|G22|Can't be change|
|SDA|G21|Can't be change|
|INT|G14||

## 6 IR Sensors
- IR Front Left: 1 pin & 2 power pins
    |Pin|GPIO|Note|
    |VCC|3V3|Connect w/ others|
    |GND|GND|Connect w/ others|
    |OUT|G36|GPIO SP|
- IR Front Right: 1 pin & 2 power pins
    |Pin|GPIO|Note|
    |VCC|3V3|Connect w/ others|
    |GND|GND|Connect w/ others|
    |OUT|G23||
- IR Upper Left: 1 pin & 2 power pins
    |Pin|GPIO|Note|
    |VCC|3V3|Connect w/ others|
    |GND|GND|Connect w/ others|
    |OUT|G39|GPIO SM|
- IR Lower Left: 1 pin & 2 power pins
    |Pin|GPIO|Note|
    |VCC|3V3|Connect w/ others|
    |GND|GND|Connect w/ others|
    |OUT|G25||
- IR Upper Right: 1 pin & 2 power pins
    |Pin|GPIO|Note|
    |VCC|3V3|Connect w/ others|
    |GND|GND|Connect w/ others|
    |OUT|G5||
- IR Lower Right: 1 pin & 2 power pins
    |Pin|GPIO|Note|
    |VCC|3V3|Connect w/ others|
    |GND|GND|Connect w/ others|
    |OUT|G4||