# Описание
![LOGO](https://github.com/ferrovovan/C-wave-change-pack/blob/main/Logo.png)

Набор программ для изменения wav-файлов на Си
  
#### Доступные программы
- `wav_info`
- `concat_wav`
- `boosted_wav`
- `slowed_wav`
- `mult_wav`
- `1_hour_wav`
- `fan_1_hour_wav`

#### Сборка
make <program_name>  
Example:
```
make boosted_wav
```

### Примеры использования
Получить информацию об wave-файле
```
./wav_info.out -i test.wav
```
Использование fan_1_hour_wav
```
./extender_fan.out -i test_in.wav -sr 1 -er 2 -md 0.5 -ed 10 -o test_out.wav
```
