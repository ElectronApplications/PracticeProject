# Задача проекта - многоалфавитный шифр Вижинера
В шифре Цезаря каждая буква алфавита сдвигается на несколько позиций; например в шифре Цезаря при сдвиге +3, A стало бы D, B стало бы E и так далее. Шифр Виженера состоит из последовательности нескольких шифров Цезаря с различными значениями сдвига. Для зашифровывания может использоваться таблица алфавитов, называемая tabula recta или квадрат (таблица) Виженера. Применительно к латинскому алфавиту таблица Виженера составляется из строк по 26 символов, причём каждая следующая строка сдвигается на несколько позиций. Таким образом, в таблице получается 26 различных шифров Цезаря. На каждом этапе шифрования используются различные алфавиты, выбираемые в зависимости от символа ключевого слова. Например, предположим, что исходный текст имеет такой вид:

```
ATTACKATDAWN
```

Человек, посылающий сообщение, записывает ключевое слово («LEMON») циклически до тех пор, пока его длина не будет соответствовать длине исходного текста:

```
LEMONLEMONLE
```

![Таблица Вижинера](/screenshots/vigenere_table.png)\
*Квадрат Виженера, или таблица Виженера, также известная как tabula recta, может быть использована для шифрования и расшифровывания.*

Первый символ исходного текста («A») зашифрован последовательностью L, которая является первым символом ключа. Первый символ зашифрованного текста («L») находится на пересечении строки L и столбца A в таблице Виженера. Точно так же для второго символа исходного текста используется второй символ ключа; то есть второй символ зашифрованного текста («X») получается на пересечении строки E и столбца T. Остальная часть исходного текста шифруется подобным способом.

```
Исходный текст:       ATTACKATDAWN
Ключ:                 LEMONLEMONLE
Зашифрованный текст:  LXFOPVEFRNHR
```

Расшифровывание производится следующим образом: находим в таблице Виженера строку, соответствующую первому символу ключевого слова; в данной строке находим первый символ зашифрованного текста. Столбец, в котором находится данный символ, соответствует первому символу исходного текста. Следующие символы зашифрованного текста расшифровываются подобным образом.

Если n — количество букв в алфавите, m_j — номер буквы открытого текста, k_j — номер буквы ключа в алфавите, то шифрование Виженера можно записать следующим образом:

```
c_j = (m_j + k_j) mod n
```

И расшифровывание:

```
m_j = (c_j − k_j) mod n
```

# Компиляция

## Windows - MSVC
Если на компьютере установлена среда Visual Studio с поддержкой компиляции C++, можно воспользоваться компилятором MSVC.\
Для этого сначала надо открыть консоль разработчика, которая в меню "Пуск" имеет название "Developer Command Prompt for VS", затем перейти в консоли в папку проекта
и ввести команду:
```
.\make.bat
```
В папке проекта появится исполняемый файл vigenere.exe

## Linux, Windows - GCC
Для компиляции с использованием GCC необходимо воспользоваться командой:
```
make
```
Для windows в папке проекта имеется скрипт make.sh, который запускает вышеприведенную команду при помощи набора утилит w64devkit. Данный скрипт может быть запущен через Git bash

# Использование программы
При запуске программы без параметров командной строки будет выведена справка со всеми возможными параметрами.\
Несколько примеров использования:
```sh
./vigenere -i # Запускает программу в интерактивном режиме
./vigenere input.txt key # Шифрует файл input.txt с использованием ключа key и записывает результат в файл input.txt
./vigenere -d output.txt key output1.txt # Расшифровывает файл output.txt и записывает результат в файл output1.txt
./vigenere -e src/main.cpp "encrypt me some source code" src/main.cpp # Шифрует файл src/main.cpp ключом в кавычках
./vigenere entext.txt "LEMON" entext.txt en # Шифрует файл entext.txt с использованием словаря, состоящего только из английских букв
```