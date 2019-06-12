# 2018 Pusan National University Hardware Term Project

## Korean(한국어) ##

### 기반 장치
- STM32F107 (https://www.icbanq.com/P005669760)

### 사용되는 모듈
- HC-SR04P Ultrasonic Module(oscillator 없는 장치)
- FB755AC Bluetooth Module
- Relay Module
- Servo Motor
- Laser Module
- PIR Sensor
- Buzzer

### 프로젝트 구성 절차

- 요구되는 프로그램: Eclipse DS-5
- 작업 환경 구축 절차
  1. C 프로젝트를 생성(생성 시 실행 설정을 Bare-metal Executable → ARM Compiler 5)
  2. PSU_DB, Libraries 폴더와 `flashclear.axf, stm32-test.sct`파일을 한 곳에 모아주도록 합니다.
  3. 프로젝트가 생성되면 Project → Properties에서 아래를 수행해주시길 바랍니다.
     - ARM C Compiler 5 → Code Generation → cortex-m3 기입
     - ARM Assembler 5 → Code  Generation → cortex-m3 기입
     - ARM Linker 5 → General → cortex-m3 기입
  4. C/C++ Build Setting에서 ARM Compiler 5의 Includes에 아래의 폴더 리스트를 기입하도록 합니다.
     - CoreSupport
     - DeviceSupport
     - DeviceSupport/Startup
     - STM32F10xStdPeriph_Drice_v3.5/inc
     - STM32F10xStdPeriph_Drice_v3.5/src
  5. ARM Linker 5 → Image Layout에서 `stm32-test.sct` 파일을 기입해주도록 합니다.
  6. 프로젝트 설정이 끝나면 Windows → Preferences의 DS-5탭 아래 Configuration Database를 클릭해서 PSU_DB를 넣어주도록 합니다.
  7. 위 과정까지 끝나면 Debug → Debug Configuration을 설정하도록 해줍니다.
  8. 연결 목표 설정을 PSU_DB의 Debug Cortex-M3로 지정해주도록 합니다.
  9. USB 포트를 잡아주도록 합니다.
  10. File 탭에 내용이 아무것도 없는 지 확인을 해주도록 합니다.
  11. Debug 탭에 connection only로 하도록 합니다.
  12. 위 과정을 다 거치면 남은 소스 코드를 넣고 빌드를 수행해주도록 합니다.
  13. 빌드가 완료되면 장치를 연결하고, `flash load (파일 위치)/*.axf`를 명령어 창에 쳐주도록 합니다.
  14. 장치를 껐다가 켜주도록 합니다.

  
