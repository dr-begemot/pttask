# pttask - test task for PT

To get the latest version, run:
  git clone https://github.com/dr-begemot/pttask.git

Дано:
поток байт, состоящий из последовательности пакетов двух типов:
бинарные — начинаются с байта 0x24, за которым идёт длина (uint32), за которым собственно данные,
текстовые — состоят из ascii-кодов символов и оканчиваются на «\r\n\r\n»
обработчик данных со следующим интерфейсом:
virtual void handle_binary_data(const char* data, size_t size)
virtual void handle_text_data(const char* data, size_t size)
Требуется написать класс с интерфейсом

class IStreamReader
{
    virtual int receive(const char* stream, size_t bytes_read) = 0;
}

, который бы принимал поток данных, не привязанный к границам пакетов, блоками произвольного размера (обычный размер — 4096, но могут быть и любые другие).  Пакеты надо вычитывать сразу же при получении очередного блока и немедленно передавать в обработчик.

УТОЧНЕНИЯ:

1.  "поток данных, не привязанный к границам пакетов" - означает ли это, что пакеты могут поступать в receive не полностью, например начало пакета в конце первого блока, а окончание этого пакета приходит в следующем блоке данных? Блоками я здесь назвал последовательности байт, которые передаются в метод receive.
>> Да, именно так.

2. Касательно старт-символа 0x24 для бинарных пакетов.
Этот символ в ASCII является '$', то есть вполне возможна ситуация строки "$16 - цена за баррель нефти на сегодня" :)
Это одно из условий задачи? То есть нужно анализировать контекст, чтобы определить точно ли перед нами бинарные данные, а не строка?
>> Рад этому вопросу. Будем считать, что строки никогда не начинаются с '$' (это не означает, что данный символ не может встретиться внутри строки).

3. из п.2 следует вопрос может ли тогда быть мусор между пакетами?
>> Нет, не может. Подразумевается, что как только заканчивается один пакет (не важно, текстовый или бинарный), тут же начинается следующий (опять же, или текстовый или бинарный).

4. В методы handle_binary_data и handle_text_data должны передаваться только полностью сформированные пакеты? 
То есть либо N+1+sizeof(uint32) байт для бинарных данных (где N это uint32 идущий за стартовым символом 0x24), либо полная строка заканчивающаяся «\r\n\r\n».
>> handle_binary_data - передавать только сами данные пакета (без маркера начала пакета и без размера пакета, так как обработчику маркер ни к чему, а размер передаётся и так, учитывая сигнатуру метода handle_binary_data).
handle_text_data - передавать всю строку, но без хвостовых "\r\n\r\n" (опять же, там сигнатура метода уже подразумевает признак конца в виде размера строки). Наличие терминального символа в конце передаваемой строки конечно же тоже не требуется.
"Немедленно передавать" в данном случае означает, что вызов handle_binary_data или handle_text_data должен осуществляться ровно в тот момент, как только "на руках" имеется законченный пакет.

5. Что должен возвращать метод receive? Количество переданных в обработчики байт? Количество переданных пакетов?
>> Количество переданных в обработчики пакетов.
