#include "enc/TokCounter.h"
#include <iostream>
#include <string>
#include <initializer_list>
#include <cassert>

int main() {
    TokCounter counter;

    std::initializer_list<std::pair<size_t, std::string>> phrases = {
        { 4   , "Ddfgdgd"},
        { 4   , "dfgdfg"},
        { 11  , "df  f dfg dr  rg  d "},
        { 2   , "  "},
        { 1   , "_"},
        { 0   , ""},
        { 1   , "_______"},
        { 10  , "This is a test, and it's working!"},
        { 24  , "This is an example sentence to try encoding out on! &#8211; Привет!"},
        { 156 , "Соединив два слова мы получаем «гамарджоба генацвале» გამარჯობა, что на русском языке будет звучать как «здравствуй, добрый друг»"},
        { 1   , "This"},
        { 27  , "გამარჯობა"},
        { 55  , R"({"title": string,"totalResults": string,"searchTerms": string,"count": integer,"startIndex": integer,"startPage": integer,"language": string,"inputEncoding": string,"outputEncoding": string,"safe": string,"cx": string})"},
        { 58  , "😢😟🤓😎😭😶‍🌫️🥵😦🥶😳✅❤️😳🤞😩😞😢❌🌷➕🇺🇦"},
        { 299 , "Приветствую! (русский); Hello! (английский); Bonjour! (французский); Hola! (испанский); Hallo! (немецкий); Ciao! (итальянский); Olá! (португальский); こんにちは！(японский); 안녕하세요! (корейский); 你好！(китайский); прывітанне! (белорусский); Здраво! (сербский); Здраво! (македонский); Здравейте! (болгарский)"},
        { 197 , "Hello! Вітаю! Здравствуйте! Bonjour! ¡Hola! Ciao! Olá! こんにちは！(Konnichiwa!) 안녕하세요! (Annyeonghaseyo!) 你好！(Nǐ hǎo!) გამარჯობა! (Gamardjoba!) Բարև Ձեզ! (Barev Dzez!) Γεια σας! (Geia sas!) Merhaba! Исәнмесез! (Isänmesez!)"},
    };
    for (const auto& phrase: phrases) {
        size_t count = counter.count(phrase.second);
        std::cout << "phrase (" << count << " ):\t" << phrase.second <<"\n";
        assert(count == phrase.first);

    }

    return 0;
}
