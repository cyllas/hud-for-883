# ESP32-S3 HUD Navigation Display

Sistema de navegação HUD (Head-Up Display) para ESP32-S3 com display AMOLED redondo de 1.43", compatível com o aplicativo Sygic GPS no iPhone.

![ESP32-S3 HUD](https://img.shields.io/badge/ESP32--S3-HUD%20Navigation-blue?style=for-the-badge&logo=espressif)
![Build Status](https://img.shields.io/badge/build-passing-brightgreen?style=for-the-badge)
![License](https://img.shields.io/badge/license-MIT-green?style=for-the-badge)

## Hardware

- **Placa**: ESP32-S3 Touch AMOLED 1.43 (Waveshare)
- **Display**: AMOLED 466×466px, interface QSPI, formato redondo
- **Sensores**: IMU QMI8658 (acelerômetro/giroscópio), RTC PCF85063
- **Conectividade**: BLE 5.0, Wi-Fi 2.4GHz
- **Interface**: Touch screen capacitivo

## Funcionalidades

### Navegação
- Recepção de dados de navegação via BLE do Sygic iOS
- Exibição de instruções de direção
- Indicação de distância até próxima manobra
- Exibição de limite de velocidade
- Setas direcionais para curvas

### Display
- Interface otimizada para tela redonda
- Temas dia/noite
- Rotação automática baseada no sensor IMU
- Controle de brilho

### Conectividade
- Protocolo BLE compatível com Sygic
- Reconexão automática
- Indicador de status de conexão

## Configuração do Sygic iOS

Para habilitar o modo BLE HUD no Sygic:

1. Abra o Sygic GPS Navigation & Maps
2. Vá em Menu → Settings → Info → About
3. Toque 3 vezes em qualquer item (nova linha "About" aparece no topo)
4. Toque em About → BLE HUD → Start

**Importante**: Esta é uma funcionalidade não oficial que pode ser removida pelo Sygic a qualquer momento.

## Estrutura do Projeto

```
src/
├── main.cpp              # Entry point principal
├── ble/
│   ├── ble_server.h      # Header do servidor BLE
│   └── ble_server.cpp    # Implementação BLE (adaptado do projeto alexanderlavrushko)
├── display/
│   ├── amoled_driver.h   # Header do driver AMOLED
│   ├── amoled_driver.cpp # Driver para display 466x466
│   ├── ui_manager.h      # Header do gerenciador de UI
│   └── ui_manager.cpp    # Gerenciamento da interface
├── sensors/
│   ├── imu_handler.h     # Header do sensor IMU
│   └── imu_handler.cpp   # Manipulação do sensor QMI8658
└── config/
    ├── settings.h        # Header das configurações
    └── settings.cpp      # Sistema de configurações persistentes
```

## Dependências

- **Platform**: ESP32 Arduino Framework
- **Libraries**:
  - NimBLE-Arduino (comunicação BLE)
  - Wire (I2C para sensores)
  - SPI (interface do display)
  - Preferences (armazenamento de configurações)

## Compilação

1. Instale o PlatformIO
2. Clone este repositório
3. Abra o projeto no PlatformIO
4. Compile e faça upload para o ESP32-S3

```bash
pio run --target upload
```

## Pinout ESP32-S3 Touch AMOLED 1.43

### Display AMOLED
- CS: GPIO 10
- DC: GPIO 11
- RST: GPIO 12
- SCK: GPIO 14
- MOSI: GPIO 13

### Touch Screen
- SDA: GPIO 6
- SCL: GPIO 7
- INT: GPIO 5
- RST: GPIO 4

### Sensores I2C
- SDA: GPIO 6 (compartilhado com touch)
- SCL: GPIO 7 (compartilhado com touch)

## Uso

1. Conecte a placa à alimentação
2. O display mostrará "HUD Ready" aguardando conexão
3. Abra o Sygic e ative o modo BLE HUD
4. O dispositivo conectará automaticamente
5. Configure uma rota no Sygic para ver as instruções no HUD

## Limitações

- Compatibilidade apenas com Sygic iOS
- Funcionalidade BLE não oficial (pode ser removida)
- Dados de velocidade em km/h, distância pode variar entre metros/pés

## Melhorias Futuras

- [ ] Integração com LVGL para UI mais sofisticada
- [ ] Suporte a touch screen para configurações
- [ ] Modo de economia de energia
- [ ] Suporte a outros apps de navegação
- [ ] Interface web para configuração via Wi-Fi

## Créditos

Baseado no projeto [BLE-HUD-navigation-ESP32](https://github.com/alexanderlavrushko/BLE-HUD-navigation-ESP32) de Alexander Lavrushko.

## Licença

MIT License - veja LICENSE para detalhes.