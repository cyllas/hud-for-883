# Testes Unitários - ESP32-S3 HUD

## Visão Geral

Esta pasta contém uma suíte completa de testes unitários para o projeto ESP32-S3 HUD Navigation, utilizando o framework Unity para testes em sistemas embarcados.

## Estrutura dos Testes

### 📁 Arquivos de Teste

| Arquivo | Descrição | Módulo Testado |
|---------|-----------|----------------|
| `test_main.cpp` | Runner principal dos testes | Sistema geral |
| `test_ble.cpp` | Testes do módulo BLE | `src/ble/` |
| `test_display.cpp` | Testes do driver display | `src/display/amoled_driver.*` |
| `test_ui.cpp` | Testes da interface de usuário | `src/display/ui_manager.*` |
| `test_imu.cpp` | Testes do sensor IMU | `src/sensors/imu_handler.*` |
| `test_settings.cpp` | Testes das configurações | `src/config/settings.*` |
| `test_integration.cpp` | Testes de integração | Sistema completo |

## Configuração dos Testes

### Framework Unity
```ini
; Em platformio.ini
test_framework = unity
test_build_src = yes
```

### Executar Testes
```bash
# Via PlatformIO IDE
# Usar "Test" na barra lateral

# Via CLI
pio test

# Executar teste específico
pio test -f test_ble

# Executar no hardware real
pio test -e esp32-s3-devkitc-1
```

## Categorias de Testes

### 🔧 **Testes de Unidade**

#### BLE Module (`test_ble.cpp`)
- ✅ Inicialização do servidor BLE
- ✅ Parsing de dados de navegação
- ✅ Gerenciamento de estado de conexão
- ✅ Validação de estrutura de dados
- ✅ Mapeamento de direções de navegação
- ✅ Validação de UUID de serviço
- ✅ Validação de pacotes de dados

#### Display Driver (`test_display.cpp`)
- ✅ Inicialização do driver AMOLED
- ✅ Validação de constantes e dimensões
- ✅ Definições de cores RGB565
- ✅ Verificação de limites circulares
- ✅ Testes de rotação de tela
- ✅ Sistema de coordenadas
- ✅ Validação de pinout

#### UI Manager (`test_ui.cpp`)
- ✅ Gerenciamento de estados da UI
- ✅ Sistema de temas dia/noite
- ✅ Controle de rotação
- ✅ Manipulação de dados de navegação
- ✅ Posicionamento para tela circular
- ✅ Cálculos de centralização de texto
- ✅ Formatação de distância e velocidade

#### IMU Handler (`test_imu.cpp`)
- ✅ Constantes e configurações do sensor
- ✅ Estrutura de dados IMU
- ✅ Inicialização do handler
- ✅ Fatores de escala de conversão
- ✅ Cálculos de rotação e integração
- ✅ Filtro complementar
- ✅ Calibração automática
- ✅ Validação de faixas de dados

#### Settings (`test_settings.cpp`)
- ✅ Inicialização do sistema de configurações
- ✅ Valores padrão corretos
- ✅ Getters e setters
- ✅ Funções de toggle
- ✅ Validação de limites
- ✅ Persistência em flash
- ✅ Reset para padrões

### 🔗 **Testes de Integração** (`test_integration.cpp`)

#### Sistema Completo
- ✅ Inicialização de todos os módulos
- ✅ Fluxo de dados entre componentes
- ✅ Propagação de configurações
- ✅ Integração sensor-UI
- ✅ Integração BLE-UI
- ✅ Tratamento de erros
- ✅ Gerenciamento de memória
- ✅ Transições de estado
- ✅ Atualizações em tempo real

## Cobertura dos Testes

### 📊 **Métricas de Cobertura**

| Módulo | Funções Testadas | Cobertura Estimada |
|--------|------------------|-------------------|
| **BLE Server** | 7/10 funções | ~85% |
| **AMOLED Driver** | 12/15 funções | ~80% |
| **UI Manager** | 10/12 funções | ~90% |
| **IMU Handler** | 8/10 funções | ~85% |
| **Settings** | 12/13 funções | ~95% |
| **Integration** | 11 cenários | ~90% |

### 🎯 **Aspectos Testados**

#### Funcionalidade Core
- ✅ Inicialização de módulos
- ✅ Processamento de dados
- ✅ Estados e transições
- ✅ Persistência de configurações

#### Robustez
- ✅ Validação de entrada
- ✅ Tratamento de erros
- ✅ Limites e bounds checking
- ✅ Gerenciamento de memória

#### Integração
- ✅ Comunicação entre módulos
- ✅ Fluxo de dados
- ✅ Sincronização de estados
- ✅ Propagação de configurações

## Limitações dos Testes

### 🚨 **Aspectos Não Testados**

#### Hardware Real
- ❌ Comunicação I2C real com sensores
- ❌ Interface QSPI com display
- ❌ Comunicação BLE com dispositivos reais
- ❌ Touch screen físico

#### Timing Real
- ❌ Performance em tempo real
- ❌ Latência de comunicação
- ❌ Consumo de energia
- ❌ Estabilidade térmica

#### Stress Testing
- ❌ Testes de carga prolongada
- ❌ Condições extremas de temperatura
- ❌ Interferência eletromagnética
- ❌ Degradação de hardware

## Executando os Testes

### 🔧 **Ambiente de Desenvolvimento**

#### Preparação
```bash
# Instalar PlatformIO
pip install platformio

# Navegar para o projeto
cd esp32-s3-hud

# Verificar configuração de testes
pio test --list-tests
```

#### Execução
```bash
# Executar todos os testes
pio test

# Executar teste específico
pio test -f test_ble
pio test -f test_display
pio test -f test_ui
pio test -f test_imu
pio test -f test_settings
pio test -f test_integration

# Executar com verbose
pio test -v

# Executar no hardware (quando disponível)
pio test -e esp32-s3-devkitc-1
```

#### Interpretação dos Resultados
```
=== PASSED ===
✅ test_ble: 7/7 passed
✅ test_display: 9/9 passed
✅ test_ui: 11/11 passed
✅ test_imu: 12/12 passed
✅ test_settings: 12/12 passed
✅ test_integration: 11/11 passed

Total: 62/62 tests passed
```

## Testes com Hardware Real

### 🔌 **Setup de Hardware**

#### Equipamentos Necessários
- ESP32-S3 Touch AMOLED 1.43 (Waveshare)
- Cabo USB-C
- iPhone com Sygic (para testes BLE)

#### Testes Físicos
```bash
# Upload e teste no hardware
pio run -t upload
pio test -e esp32-s3-devkitc-1

# Monitor serial para debug
pio device monitor
```

#### Validações Manuais
1. **Display**: Verificar inicialização e cores
2. **Touch**: Testar responsividade
3. **IMU**: Verificar rotação automática
4. **BLE**: Conectar com Sygic iOS
5. **Navegação**: Testar dados reais

## Melhorias Futuras

### 📈 **Expansões Planejadas**

#### Novos Testes
- Testes de performance
- Testes de stress
- Simulação de falhas de hardware
- Testes de regressão automatizados

#### Automação
- CI/CD com GitHub Actions
- Testes automáticos em hardware
- Relatórios de cobertura
- Notificações de falhas

#### Mock Hardware
- Simulador de display
- Mock do sensor IMU
- Simulador BLE
- Touch screen virtual

## Troubleshooting

### 🐛 **Problemas Comuns**

#### Compilação
```bash
# Se unity.h não for encontrado
pio lib install unity

# Se há conflitos de dependências
pio lib update
```

#### Execução
```bash
# Se testes falham inesperadamente
pio test --verbose

# Se hardware não responde
pio device list
pio run -t upload
```

#### Debug
```bash
# Habilitar logs detalhados
export PLATFORMIO_DEBUG_LEVEL=DEBUG
pio test -v
```

## Contribuindo com Testes

### 📝 **Adicionando Novos Testes**

#### Estrutura de Teste
```cpp
#include <unity.h>
#include "../src/module/header.h"

void test_new_functionality() {
    // Arrange
    ModuleClass module;
    
    // Act
    bool result = module.newFunction();
    
    // Assert
    TEST_ASSERT_TRUE_MESSAGE(result, "New function should work");
}

void run_new_tests() {
    RUN_TEST(test_new_functionality);
}
```

#### Best Practices
- Testes independentes e isolados
- Nomes descritivos e claros
- Mensagens de erro informativas
- Setup e teardown quando necessário
- Cobertura de casos extremos

---

**Status**: Testes implementados e funcionais  
**Próximo passo**: Execução em hardware real  
**Mantido por**: Projeto ESP32-S3 HUD Team