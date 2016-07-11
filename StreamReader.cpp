//
//  StreamReader.cpp
//  pttask
//

#include <iostream>
#include <stdio.h>
#include <stdint.h>

#include "StreamReader.h"

using namespace std;

// Метод распределяет пакеты по обработчикам и возвращает общее колличество отправленных пакетов
int StreamReader::receive(const char *stream, size_t bytes_read){
    vector<char>::iterator it;

    if (buffer.size()+bytes_read == 0)// логика работы с итераторами ниже сломается, если будет reсeive без данных
        return 0;
        
    buffer.reserve(buffer.size()+bytes_read);// проверим память и если надо выделим новую (для корректности итераторов)
    if (buffer.size() >= sizeof(StrEnd)-1 &&  buffer[0] != StartSymbol) {// если буфер не пуст и там часть строкового пакета,
        // то не имеет смысла искать стоп-строку в первой части пакета
        // установим итератор в максимально крайнее положение, когда может встретится стоп-строка
        it = buffer.end() - (sizeof(StrEnd)-1);
    } else
        it = buffer.begin();

    buffer.insert(buffer.end(), stream, stream+bytes_read);

    auto oldit(buffer.begin());// начало какого-либо пакета
    int npack = 0;

    while(it < buffer.end()) {
        if (*oldit == StartSymbol) { // обрабатываем бинарный пакет
            uint32_t packlen;
            if (it+Offset < buffer.end() &&
                    it+Offset+(packlen=*reinterpret_cast<uint32_t*>(&it[1])) <= buffer.end()){// проверка что пакет пришел целиком
                handler->handle_binary_data(&it[Offset], static_cast<size_t>(packlen));// static_cast чтобы избежать C-style cast
                it += Offset + packlen;
            } else // пакет пришел частично, остальная часть должна придти со следущим вызовом receive
                break;// прекращаем цикл
        } else { // обрабатываем пакет-строку
            vector<char>::const_iterator curend = find_horspool(buffer, it);
            if (curend < buffer.end()) {
                handler->handle_text_data(&*oldit, static_cast<size_t>(distance<vector<char>::const_iterator>(oldit, curend)));
            } else // пакет пришел частично
                break;// прекращаем цикл
        }
        oldit = it;
        npack++;
    }

    // подчистка памяти
    // удаляем уже отправленные неактуальные пакеты, при этом зарезервированная память остается неизменной
    if (oldit!=buffer.begin())
        buffer.erase(buffer.begin(), oldit);

    return npack;
}

// Формируем таблицу стоп-символов
void StreamReader::preBmBc() {
    const size_t sz = sizeof(StrEnd);
    memset(bmBc, sz, sizeof(bmBc));
    for (size_t i = 0; i < sz - 1; ++i)
        bmBc[StrEnd[i]] = static_cast<char>(sz - i - 1);
}

// Алгоритм Бойера — Мура — Хорспула, упрощенный алгоритм Бойера — Мура
// возвращает итератор на элемент, являющемся первым байтом стоп-строки пакета (StrEnd)
vector<char>::const_iterator StreamReader::find_horspool(const vector<char> &vec, vector<char>::iterator &it){
    const size_t sz = sizeof(StrEnd);
    char c;

    while (it <= vec.end()-sz) {
        c = *(it+sz-1);// берем посл. символ из сравниваемой последовательности
        if (StrEnd[sz-1] == c && memcmp(&*it, StrEnd, sz - 1) == 0) {
            vector<char>::const_iterator oldit = it;
            it += sz;
            return oldit;
        }
        it += bmBc[c];
    }

    return vec.end();//дошли до конца и ничего не нашли
}
