package main

/*
#cgo CFLAGS: -I../include
#cgo LDFLAGS: -L../libs -ltokenator -licuuc -licui18n -lm -lstdc++
#include "tokenator.h"
#include <stdlib.h>
*/
import "C"

import (
	"fmt"
	"unsafe"
)

func main() {
	phrases := []struct {
		count int
		text  string
	}{
		{4, "Ddfgdgd"},
		{4, "dfgdfg"},
		{11, "df  f dfg dr  rg  d "},
		{2, "  "},
		{1, " "},
		{1, "_"},
		{0, ""},
		{1, "_______"},
		{10, "This is a test, and it's working!"},
		{24, "This is an example sentence to try encoding out on! &#8211; Привет!"},
		{156, "Соединив два слова мы получаем «гамарджоба генацвале» გამარჯობა, что на русском языке будет звучать как «здравствуй, добрый друг»"},
		{1, "This"},
		{27, "გამარჯობა"},
		{55, "{\"title\": string,\"totalResults\": string,\"searchTerms\": string,\"count\": integer,\"startIndex\": integer,\"startPage\": integer,\"language\": string,\"inputEncoding\": string,\"outputEncoding\": string,\"safe\": string,\"cx\": string}"},
		{58, "😢😟🤓😎😭😶‍🌫️🥵😦🥶😳✅❤️😳🤞😩😞😢❌🌷➕🇺🇦"},
		{299, "Приветствую! (русский); Hello! (английский); Bonjour! (французский); Hola! (испанский); Hallo! (немецкий); Ciao! (итальянский); Olá! (португальский); こんにちは！(японский); 안녕하세요! (корейский); 你好！(китайский); прывітанне! (белорусский); Здраво! (сербский); Здраво! (македонский); Здравейте! (болгарский)"},
		{197, "Hello! Вітаю! Здравствуйте! Bonjour! ¡Hola! Ciao! Olá! こんにちは！(Konnichiwa!) 안녕하세요! (Annyeonghaseyo!) 你好！(Nǐ hǎo!) გამარჯობა! (Gamardjoba!) Բարև Ձեզ! (Barev Dzez!) Γεια σας! (Geia sas!) Merhaba! Исәнмесез! (Isänmesez!)"},
	}

	for _, phrase := range phrases {
		cstr := C.CString(phrase.text)
		defer C.free(unsafe.Pointer(cstr))

		count := int(C.tokenator_count(cstr, C.size_t(len(phrase.text))))
		// count := int(C.tokenator_count(&phrase.text, C.size_t(len(phrase.text))))
		fmt.Printf("phrase (%d):\t%s\n", count, phrase.text)

		if count != phrase.count {
			panic(fmt.Sprintf("Expected %d, got %d", phrase.count, count))
		}
	}
}

// To execute this code launch the command below:
// go run tok_count_go.go
