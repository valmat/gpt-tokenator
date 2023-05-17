#include "tokenator.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>

int main() {

    struct {
        size_t count;
        const char* data;
    } phrases[] = {
        { 4   , "Ddfgdgd"},
        { 4   , "dfgdfg"},
        { 11  , "df  f dfg dr  rg  d "},
        { 2   , "  "},
        { 1   , " "},
        { 1   , "_"},
        { 0   , ""},
        { 1   , "_______"},
        { 10  , "This is a test, and it's working!"},
        { 24  , "This is an example sentence to try encoding out on! &#8211; Привет!"},
        { 156 , "Соединив два слова мы получаем «гамарджоба генацвале» გამარჯობა, что на русском языке будет звучать как «здравствуй, добрый друг»"},
        { 1   , "This"},
        { 27  , "გამარჯობა"},
        { 55  , "{\"title\": string,\"totalResults\": string,\"searchTerms\": string,\"count\": integer,\"startIndex\": integer,\"startPage\": integer,\"language\": string,\"inputEncoding\": string,\"outputEncoding\": string,\"safe\": string,\"cx\": string}"},
        { 58  , "😢😟🤓😎😭😶‍🌫️🥵😦🥶😳✅❤️😳🤞😩😞😢❌🌷➕🇺🇦"},
        { 299 , "Приветствую! (русский); Hello! (английский); Bonjour! (французский); Hola! (испанский); Hallo! (немецкий); Ciao! (итальянский); Olá! (португальский); こんにちは！(японский); 안녕하세요! (корейский); 你好！(китайский); прывітанне! (белорусский); Здраво! (сербский); Здраво! (македонский); Здравейте! (болгарский)"},
        { 197 , "Hello! Вітаю! Здравствуйте! Bonjour! ¡Hola! Ciao! Olá! こんにちは！(Konnichiwa!) 안녕하세요! (Annyeonghaseyo!) 你好！(Nǐ hǎo!) გამარჯობა! (Gamardjoba!) Բարև Ձեզ! (Barev Dzez!) Γεια σας! (Geia sas!) Merhaba! Исәнмесез! (Isänmesez!)"},
    };

    size_t num_phrases = sizeof(phrases) / sizeof(phrases[0]);

    for (size_t i = 0; i < num_phrases; ++i) {
        size_t count = tokenator_count(phrases[i].data, strlen(phrases[i].data));
        printf("phrase (%zu):\t%s\n", count, phrases[i].data);
        assert(count == phrases[i].count);
    }

    return 0;
}
