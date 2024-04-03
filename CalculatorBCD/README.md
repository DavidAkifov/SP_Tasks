**Съдържание**

[1. Не знам какво да правя](#h1)

[2. Не знам какво е BCD](#h2)

[3. Не знам как се събира BCD](#h3)

[4. Не знам как се изважда BCD](#h4)

[5. Реализация на задачата по условие: съхранение на BCD в една променлива](#h5)

[6. Алгоритъм](#h6)

# 1. Не знам какво да правя<a name="h1"></a>
*По условие*, потребителя въвежда две целочислени числа в конзолата. Всяко число да се съхрани като [BCD](#h2) в една променлива, например `unsigned int` или `unsigned long long` (използва се unsigned за да не ви показва отрицателни стойности). После тези BCD числа се събират или изваждат, избор се прави отново от потребителя през конзолата. Резултатът се показва на екрана в BCD формат.

# 2. Не знам какво е BCD<a name="h2"></a>
Обикновено десетичните числа се съхраняват в паметта под двоичната им форма. Тоест десетичното число `179` ще стане `10110011`. BCD форматът също използва двоични числа, само че всяка *цифра* от десетичното число се кодира в двоична, и тя винаги заема точно 4 бита.

Например, за да конвертираме същото десетично число `179` в BCD:

```
1 = 0001
7 = 0111
9 = 1001
```

Тоест `179` десетично = `000101111001` BCD. За да стане малко по-нагледно **в примерите** ще разделяме всяка цифра с разстояние: `0001 0111 1001`. За знак се използва най-лявата (най-значещата) цифра от числото, като 0 значи положително, а 9 значи отрицателно число. В случая трябва да имаме `0000 0001 0111 1001`.

Ако ви трябва по-подробна информация има достатъчно в интернет.

# 3. Не знам как се събира BCD<a name="h3"></a>
Всяка цифра се събира с нейния разряд, започвайки от най-младшия. Нека съберем `13` + `5`:

```
13 = 0001 0011
5 = 0101

0001 0011
0000 0101 +
---------
0001 1000

0001 1000 = 18
```

Това обаче не работи, ако сбора на двете цифри > 9. Например, `5` + `8`:

```
5 = 0101
8 = 1000

0101
1000 +
----
1101
```

Тъй като `1101` не е десетична цифра, *прибавяме 6 към сумата*:

```
     1101
     0110 +
---------
0001 0011
```

Резултатът не може да бъде съхранен в 4 бита. Тогава към следващата по-старша цифра се прибавя единица (т.нар. "едно наум" или "carry").

[Друг пример](https://en.wikipedia.org/wiki/Binary-coded_decimal#Operations_with_BCD)

# 4. Не знам как се изважда BCD<a name="h4"></a>
Използва се допълване до девет (nine's complement) или допълване до десет (ten's complement). Тук трябва да се използва и знака. След това допълненото число се събира.

Пример: `7` - `3`

```
+7 = 0000 0111
+3 = 0000 0011

Допълваме 3:
99 - 3 = 96 + 1 = 97
97 = 1001 0111

Събираме 7 с допълненото:
     0000 0111
     1001 0111 +
--------------
     1001 1110

Добавяме 6 към неправилните цифри:
     1001 1110
          0110 +
--------------
     1010 0100
     0110 +
--------------
0001 0000 0100

Тъй като работим само с два разряда, най-лявото 0001 не ни интересува. Резултаът става 0000 (знак) 0100 (4)
```

[Друг пример](https://en.wikipedia.org/wiki/Binary-coded_decimal#Subtraction)

# 5. Реализация на задачата по условие: съхранение на BCD в една променлива<a name="h5"></a>
Потребителя въвежда десетичното число `2708`. По условие трябва да си направим една променлива, например `unsigned int`, и в нея да съхраним същото число, само че в BCD формат. Тоест трябва *самите битове на този `unsigned int`* да бъдат `0010 0111 0000 1000`. За целта трябва първо да вземем всяка *цифра* от десетичното число и да я конвертираме, а след това да я сложим на правилното място. Конвертирането е лесната част и може да го направите по много различни начини. Хванете някой алгоритъм от интернет или го направете на ръка със сметки - няма значение.

## 5.1. Битова аритметика
За да манипулираме различните битове на дадена променлива използваме битова аритметика. Тя е подобна на логическата (вероятно познавате операторите `&&`, `||` и `!`) само че се прилага *върху битове*.

[Битови оператори](https://en.wikipedia.org/wiki/Bitwise_operations_in_C):

```
И (&)
ИЛИ (|)
НЕ (~)
отместване наляво (<<)
отместване надясно (>>)
изключващо ИЛИ/XOR (^)
```

## 5.2. Пример за записване на BCD число в unsigned int
Инициализираме `unsigned int n = 0;`. **Така всички битове в променливата са 0**.

За да запишем `37` първо конвертираме:
```
3 = 0011
7 = 0111
```
Слагаме седмицата директно чрез побитово ИЛИ:
```
n 0000 0000
7 0000 0111 |
-----------
n 0000 0111
```
За да сложим тройката на правилното място първо я местим наляво четири пъти, а после я добавяме към n чрез побитово ИЛИ:
```
3 0000 0011
          4 <<
-----------
  0011 0000
n 0000 0111 |
-----------
n 0011 0111
```
Написано на код, това ще изглежда така:
```c
unsigned int n = 0;  //initialize everything to 0s
n |= 7;  //add 7's bits
n |= 3 << 4;  //shift 3's bits to the correct place, then add them too
```
Нека добавим и една петица, така че BCD числото ни да стане `537`. Тук вече трябва да преместим 8 бита:
```c
n |= 5 << 8;  //add 5's bits
```

### Как се сменят вече записани битове
Нека имаме следната последователност в един `unsigned char`:
```c
unsigned char n = 0xAA;
//   0xAA = 1010 1010 (binary)
//bit nums: 7654 3210
```
Под битовете с коментар "bit nums:" са означени само номерата на битовете, за да ги разграничаваме по-лесно в следващия пример. Ще се опитаме да запишем последователността `0101` съответно в битове 5, 4, 3 и 2, така че битовете на `n` да станат `1001 0110`. За целта първо зануляваме битовете:
```c
unsigned char n = 0xAA;     //1010 1010
//we want to work on four bits, so we need four 1s:
unsigned char mask = 0xF;     //0000 1111
//then we can shift that two places, to cover bit numbers from 2 to 5:
mask = mask << 2;     //0011 1100
//after that we invert (i.e. change the 1s to 0s and vice versa) the bits with the binary NOT operator (~):
mask = ~mask;     //1100 0011
//and AND them with the target value
n &= mask;     //n = 1000 0010 (binary)

//we can do all that on a single row:
n &= ~(0xF << 2);     //puts 0s on bit positions from 2 to 5
```
След зануляване на дадените битове можем както е показано по-горе да преместим битовете на желаната позиция и да използваме битово ИЛИ за да ги запишем:
```c
unsigned char newValue = 0x5;     //0000 0101
n |= newValue << 2;     //n = 1001 0110 (binary)
```

## 5.3. Пример за извличане на цифри от BCD unsigned int:
Имаме `612` съхранено в битовете на `unsigned int n`. Тоест `0110 0001 0010`.

Да извлечем двойката е най-лесно, тъй като тя е най-младшата цифра. Ако напишем двоично `1111` то е еквивалентно на `0 1111` или `0000 0000 1111` тъй като тези нули не значат нищо. Ще изполваме това свойство в този пример:
```c
unsigned int n = BCD(612);  //0110 0001 0010
int res = n & 15;  //15 decimal = 1111 binary
```
Така `res` става равно на `0010` или десетичното число `2`. За да вземем единицата ще направим същото, само че първо ще преместим битовете надясно:
```
n 0110 0001 0010
               4 >>
----------------
       0110 0001
       0000 1111 &h
----------------
       0000 0001
```
Или на код:
```c
int res = (n >> 4) & 15;
```
От изпълнението имаме `res` = `0001` което е `1` в десетично.

Вместо `x & 15` може да използваме и `x & 0xF` в шестнадесетичен запис, а ако го поддържа компилатора: `0b1111` в двоичен. Няма промяна в резултата, тъй като битовете на тези числа са еднакви.

# 6. Алгоритъм<a name="h6"></a>
1. Решете каква променлива ще ползвате. Обикновено `unsigned int` има 4 байта, тоест имате място за 8 BCD цифри. Само че трябва да съхранявате знак, а и може да се получи резултат с една повече цифра, така че съхранявайте само 6 ако използвате `unsigned int`. Напр. `-543210` събрано с `-543210` става `-1086420`, седем цифри и осмата за знак. Аналогично, 2 цифри за двубайтово число (`unsigned short`) или 14 за 8-байтово (`long long`).
2. Направете конвертирането на числата от десетични в BCD. За да си дебъгвате програмата, използвайте `printf("%0x", n);`. Това ви принтира шестнадесетичното представяне на `n` и ако програмата работи правилно - дирекно ще ви го показва все едно е BCD (напр. `0001 0010` битове ви принтира `12`). Ако не ви върши работа го принтирайте като обикновено десетично число и го конвертирайте в двоично чрез калкулатор, за да видите какво се случва с битовете.
3. Направете функцията за събиране. Същинското събиране на BCD цифри става с обикновен аритметичен оператор `+`, не си правете труда да пипате по битовете тук. Само извличането на цифрите и записването в резултата ползва битови операции. Не забравяйте **преди** да запишете резултата да го коригирате ако е над 9, отново като го съберете с 6 и игнорирате битовете над четвъртия.
4. Изваждането е последно. Може да има малка разлика между `a - (+b)` и `a + (-b)` тъй като съхраняваме знак. Най-лесно е да сведете до един от двата варианта по време на изпълнение. Ще ви трябва и функция за допълване на числото (обикновено се прави върху това след минуса). След допълване само го подавате на функцията за събиране. При мен трябваше и да обърна знака ако изхода от този случай е отрицателен.

# [За повече инфо: интернета или чата. От мене толкова](https://i.imgflip.com/8k1xn4.jpg)