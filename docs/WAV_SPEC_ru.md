Сейчас проект обрабатывает только PCM-совместимые форматы,
то есть *Little-Endian* форматом данных.  
*Пример*: для числа `0x1234` массив `buffer` будет выглядеть так:
```
buffer[0] = 0x34 (LSB)
buffer[1] = 0x12 (MSB)
```
Желательно без дополнительных блоков (в качестве исключения ffmpeg-получаемый формат).

Подробнее:
[WAVE Specifications/rfc2361.txt](../WAVE%20Specifications/rfc2361.txt)  300 строку, 
или [WAVE Specifications/Wave File Specifications.html](../WAVE%20Specifications/Wave%20File%20Specifications.html)
.  
