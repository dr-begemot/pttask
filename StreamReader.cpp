//
//  StreamReader.cpp
//  pttask
//
#include <iostream>
#include <iostream>
#include <stdio.h>
#include <stdint.h>
#include "StreamReader.h"

using namespace std;

// Формируем таблицу стоп-символов
void StreamReader::preBmBc() {
    const size_t sz = sizeof(StrEnd);
    memset(bmBc, sz, sizeof(bmBc));
    for (size_t i = 0; i < sz - 1; ++i)
        bmBc[StrEnd[i]] = sz - i - 1;
}

// Алгоритм Бойера — Мура — Хорспула, упрощенный алгоритм Бойера — Мура
// возвращает итератор на элемент, являющемся посл. байтом пакета с StrEnd (предшевствующий= it)
vector<char>::const_iterator StreamReader::find_horspool(const vector<char> &vec, vector<char>::iterator &it){
    const size_t sz = sizeof(StrEnd);
    char c;

    while (it <= vec.end()-sz) {
        c = *(it+sz-1);// берем посл символ из сравниваемой последовательности
        if (StrEnd[sz-1] == c && memcmp(&*it, StrEnd, sz - 1) == 0){
            it += sz;
            return it-1;
        }
        it += bmBc[c];
    }
    // дошли до конца и ничего не нашли
    return vec.end();//дошли до конца и ничего не нашли
}

// Реализация не экономная к памяти
int StreamReader::receive_fast(const char *stream, size_t bytes_read){
    int npack = 0;

    buffer.insert(buffer.end(), stream, &stream[bytes_read-1]);

    auto oldit(buffer.begin());// vector<char>::iterator
    for (auto it = buffer.begin(); it < buffer.end();) {
        if (*it == StartSymbol) { // обрабатываем бинарный пакет
            uint32_t *packlen;
            if (it+Offset < buffer.end() &&
                    it+Offset+*(packlen=reinterpret_cast<uint32_t*>(&it[1])) < buffer.end()){// проверка что пакет пришел целиком
                handler->handle_binary_data(&it[Offset], static_cast<size_t>(*packlen));// static_cast чтобы избежать C-style cast
                it += Offset + *packlen;
            } else { // пакет пришел частично, остальная часть должна придти со следущим вызовом receive
                break;// прекращаем цикл
            }
        } else { // обрабатываем пакет-строку
            if (find_horspool(buffer, it) < buffer.end()) {
                handler->handle_text_data(&*oldit, distance(oldit, it));
            } else { // пакет пришел частично
                break;// прекращаем цикл
            }
        }
        oldit = it;
        npack++;
    }

    // подчистка памяти
    // удаляем неиспользуемое, при этом зарезервированная память остается неизменной
    if (oldit!=buffer.begin()) {
        buffer.erase(buffer.begin(), oldit);
    }

    return npack;
}

// Реализация экономная к памяти
int StreamReader::receive_memeco(const char *stream, size_t bytes_read){
    int npack = 0;
    return npack;
}

int StreamReader::receive(const char *stream, size_t bytes_read){

    // Я решил сделать два метода - первый, достаточно лакончиный, но при этом не заботящийся
    // о колличестве используемой памяти, а второй, старающийся использовать память по минимуму.
    // Учитывая условия задания (обычный размер потока — 4096 байт) вполне подходит первый вариант
    // он будет немного быстрее, за счет меньшего колличества проверок.
    return receive_fast(stream, bytes_read);

    /*int npack = 0;

    if (!buffer.empty()){
        if (buffer[0] == StartSymbol) {
            //if()

            if (buffer.size() >= offset &&
                    buffer.size()) {

        }

    }

    if(buffer.empty()){
        if (stream[i] == StartSymbol) {
            uint32_t packlen;
            if (i+offset <= bytes_read && // проверка что получен заголовок пакета
                i+offset+(packlen=reinterpret_cast<uint32_t*>(&stream[i+1])) <= bytes_read) {// проверка что пакет пришел целиком
                handler->handle_binary_data(&stream[i+offset], static_cast<size_t>(packlen));// static_cast чтобы избежать C-style cast
                npack++;
                i += offset+static_cast<size_t>(packlen);
            } else {// пакет пришел частично, остальная часть должна придти со следущим вызовом receive
                // сохранение полученой части пакета
                buffer.insert(buffer.end(), &stream[i], &stream[bytes_read-1]);
                return npack;
            }
        }
    }else{
        if(buffer.size()+=)
    }

    return npack;
    std::string s()
    size_t strstart;
    buffer.reserve(buffer.size()+bytes_read);

    s()

    for(size_t i=0; i<bytes_read;){
        if(stream[i]==StartSymbol){
            packlen = reinterpret_cast<uint32_t*>(&stream[i+1]);
            if(i+packlen<=bytes_read){

            }
        }else{
            strstart = 0;

        }
    }//*/
}
