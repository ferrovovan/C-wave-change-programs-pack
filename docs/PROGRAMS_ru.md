## Доступные программы
- `wav_info`
- `concat_wav`
- `concat_upgrade_wav`
- `mult_wav`
- `cut_in_wav`
- `cut_out_wav`
- `insert_wav`
- `two_channels_wav`
- `form_pitch`
- `zero_wav (null_wav)`
- `1_hour_wav`
- `fan_1_hour_wav`

# Описание программ

## Простые
- wav_info  
Выводит информацию об wav-файле.

- concat_wav  
Конкатенирует 2 схожих по формату wav-файла.

- mult_wav  
Конкатенирует wav-файл с самим собой.

- two_channels_wav  
Конвертирует моно стерео (дублирует единственный канал).

- zero_wav (синоним null_wav)  
Создаёт пустой wav-файл заданной длительностью.


## Составные

- concat_upgrade_wav  
конкатенация, с возможностью плавного перехода (параметр -md).

- cut_in_wav  
Вырезает промежуток заданной длины.

- cut_out_wav  
Вырезает внутренний фрагмент, с возможностью плавного перехода (параметр -m).  

- insert_wav  
Вставляет *второе* в *первое* аудио.  

- form_pitch  
Записывает звуковую волну нужной частоты и амплитуды

- 1_hour_wav  
Растягивает аудио до заданной длительности, путём копирования выбранного фрагмента.  

- fan_1_hour_wav  
Тоже что и 1_hour_wav, но с неправильной расшифровкой байтов, приводящей к странным результатам.
