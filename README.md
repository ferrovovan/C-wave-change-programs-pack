# Info
![LOGO](https://github.com/ferrovovan/C-wave-change-pack/blob/main/Logo.png)

C-wave-change-pack is pack of programs to read, change wave files, on C language.  
  
#### Available programs
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

#### Compile
make <program_name>  
Example:
```
make boosted_wav
```

### Examples (exampleti gratia) Usage
Cut fragment (start 3, end 6 in seconds)
```
./cut_wav.out -i in.wav -st 3 -end 6 -o out.wav
```
Information about wav-file
```
./wav_info.out -i test.wav
```
Create tune (there A tune, 1 second duration)
```
./form_pitch A.wav 440.00 0.5 1
```
