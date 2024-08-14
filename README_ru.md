# Описание
Набор программ для изменения wav-файлов на Си
![LOGO](https://github.com/ferrovovan/C-wave-change-pack/blob/main/Logo.png)

  
#### Доступные программы
- `wav_info`
- `concat_wav`
- `boosted_wav`
- `slowed_wav`
- `mult_wav`
- `cut_wav`
- `two_channels_wav`
- `form_pitch_wav`
- `1_hour_wav`
- `fan_1_hour_wav`

#### Сборка
make <program_name>  
Пример:
```
make boosted_wav
```

### Примеры использования
Вырезать фрагмент (start 3, end 6 in seconds)
```
./cut_wav.out -i in.wav -st 3 -end 6 -o out.wav
```
Получить информацию об wave-файле
```
./wav_info.out -i test.wav
```
Создать звуковую волну (Нота Ля, длительностью в 1 секунду)
```
./form_pitch A.wav 440.00 0.5 1
```
