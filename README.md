# sistema-llenado-automatico-tinaco
Sistema mecatrónico de llenado automático de tinaco con Arduino Uno, sensores ultrasónicos HC-SR04, relé y LED RGB. Simulación en Tinkercad.


# Sistema de Llenado Automático de Tinaco

Sistema mecatrónico diseñado para monitorear y controlar automáticamente 
el nivel de agua en un tinaco elevado de 1,200 litros, alimentado desde 
una cisterna subterránea de 10,000 litros.

## Descripción

El sistema utiliza sensores ultrasónicos para medir el nivel de agua en 
ambos depósitos y toma decisiones automáticas para activar o desactivar 
una bomba de agua. El estado del sistema se indica visualmente mediante 
un LED RGB.

## Estados del sistema

| LED | Color | Descripción |
|-----|-------|-------------|
| Verde | Lleno | Tinaco superior al 90% |
| Amarillo | Normal | Tinaco entre 30% y 90% |
| Rojo | Vacío | Tinaco inferior al 30% |
| Azul | Llenando | Bomba activa, transfiriendo agua |
| Magenta | Falla | Cisterna inferior al 10% |

## Componentes

- Arduino Uno R3 (ATmega328)
- 2x Sensor ultrasónico HC-SR04
- 1x LED RGB
- 1x LED azul (indicador de bomba)
- 1x Relay SPDT 5V
- 1x Transistor NPN 2N2222
- 1x Diodo 1N4007
- 1x Motor DC (representa la bomba)
- 3x Resistencia 220Ω
- 1x Resistencia 1kΩ
- 1x Batería 9V (alimentación del motor)

## Conexiones

| Componente | Pin Arduino |
|------------|-------------|
| TRIG sensor tinaco | D2 |
| ECHO sensor tinaco | D3 |
| TRIG sensor cisterna | D4 |
| ECHO sensor cisterna | D5 |
| Relé (bomba) | D6 |
| LED RGB - Rojo | D9 |
| LED RGB - Verde | D10 |
| LED RGB - Azul | D11 |
| LED azul llenado | D12 |

## Lógica de control

Si cisterna < 10%  → FALLA: bomba OFF, LED magenta
Si tinaco > 90%    → LLENO: bomba OFF, LED verde
Si tinaco < 30%    → LLENANDO: bomba ON, LED rojo + LED azul
Si tinaco 30-90%   → NORMAL: bomba OFF, LED amarillo


## Dimensiones de los depósitos

**Tinaco:**
- Capacidad: 1,200 litros
- Geometría: cilíndrico
- Diámetro: 1.00 m
- Altura: 1.53 m
- Elevación: 5.00 m sobre nivel de suelo

**Cisterna:**
- Capacidad: 10,000 litros
- Geometría: rectangular
- Base: 2.00 m × 1.50 m
- Altura: 3.33 m
- Ubicación: subterránea

