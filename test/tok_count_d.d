#!/usr/bin/rdmd --shebang= -I. -L../libs/libtokenator.a -L-licuuc -L-licui18n -L-lm -L-lstdc++ -w -debug 

import std.stdio;
import std.string;
import std.array;
import std.typecons;

extern (C)
size_t tokenator_count(const char* data, size_t len) pure nothrow;

int main() {

    auto phrases = [
        tuple(4   , "Ddfgdgd"),
        tuple(4   , "dfgdfg"),
        tuple(11  , "df  f dfg dr  rg  d "),
        tuple(2   , "  "),
        tuple(1   , " "),
        tuple(1   , "_"),
        tuple(0   , ""),
        tuple(1   , "_______"),
        tuple(10  , "This is a test, and it's working!"),
        tuple(24  , "This is an example sentence to try encoding out on! &#8211; Привет!"),
        tuple(156 , "Соединив два слова мы получаем «гамарджоба генацвале» გამარჯობა, что на русском языке будет звучать как «здравствуй, добрый друг»"),
        tuple(1   , "This"),
        tuple(27  , "გამარჯობა"),
        tuple(55  , `{"title": string,"totalResults": string,"searchTerms": string,"count": integer,"startIndex": integer,"startPage": integer,"language": string,"inputEncoding": string,"outputEncoding": string,"safe": string,"cx": string}`),
        tuple(58  , "😢😟🤓😎😭😶‍🌫️🥵😦🥶😳✅❤️😳🤞😩😞😢❌🌷➕🇺🇦"),
        tuple(299 , "Приветствую! (русский); Hello! (английский); Bonjour! (французский); Hola! (испанский); Hallo! (немецкий); Ciao! (итальянский); Olá! (португальский); こんにちは！(японский); 안녕하세요! (корейский); 你好！(китайский); прывітанне! (белорусский); Здраво! (сербский); Здраво! (македонский); Здравейте! (болгарский)"),
        tuple(197 , "Hello! Вітаю! Здравствуйте! Bonjour! ¡Hola! Ciao! Olá! こんにちは！(Konnichiwa!) 안녕하세요! (Annyeonghaseyo!) 你好！(Nǐ hǎo!) გამარჯობა! (Gamardjoba!) Բարև Ձեզ! (Barev Dzez!) Γεια σας! (Geia sas!) Merhaba! Исәнмесез! (Isänmesez!)"),
    ];

    foreach (phrase; phrases) {
        size_t count = tokenator_count(phrase[1].ptr, phrase[1].length);
        writeln("phrase (", count, " ):\t", phrase[1]);
        assert(count == phrase[0]);
    }

    return 0;
}