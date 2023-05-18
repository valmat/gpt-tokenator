using System;
using System.Runtime.InteropServices;
using System.Collections.Generic;
using System.Text;

class Program
{
    [DllImport("libtokenator.so", CallingConvention = CallingConvention.Cdecl, EntryPoint = "tokenator_count")]
    public static extern UIntPtr tokenator_count(byte[] data, UIntPtr len);

    static void Main()
    {
        var phrases = new List<Tuple<int, string>>
        {
            Tuple.Create(4, "Ddfgdgd"),
            Tuple.Create(4, "dfgdfg"),
            Tuple.Create(11, "df  f dfg dr  rg  d "),
            Tuple.Create(2, "  "),
            Tuple.Create(1, " "),
            Tuple.Create(1, "_"),
            Tuple.Create(0, ""),
            Tuple.Create(1, "_______"),
            Tuple.Create(10, "This is a test, and it's working!"),
            Tuple.Create(24, "This is an example sentence to try encoding out on! &#8211; Привет!"),
            Tuple.Create(156, "Соединив два слова мы получаем «гамарджоба генацвале» გამარჯობა, что на русском языке будет звучать как «здравствуй, добрый друг»"),
            Tuple.Create(1, "This"),
            Tuple.Create(27, "გამარჯობა"),
            Tuple.Create(55, "{\"title\": string,\"totalResults\": string,\"searchTerms\": string,\"count\": integer,\"startIndex\": integer,\"startPage\": integer,\"language\": string,\"inputEncoding\": string,\"outputEncoding\": string,\"safe\": string,\"cx\": string}"),
            Tuple.Create(58, "😢😟🤓😎😭😶‍🌫️🥵😦🥶😳✅❤️😳🤞😩😞😢❌🌷➕🇺🇦"),
            Tuple.Create(299, "Приветствую! (русский); Hello! (английский); Bonjour! (французский); Hola! (испанский); Hallo! (немецкий); Ciao! (итальянский); Olá! (португальский); こんにちは！(японский); 안녕하세요! (корейский); 你好！(китайский); прывітанне! (белорусский); Здраво! (сербский); Здраво! (македонский); Здравейте! (болгарский)"),
            Tuple.Create(197, "Hello! Вітаю! Здравствуйте! Bonjour! ¡Hola! Ciao! Olá! こんにちは！(Konnichiwa!) 안녕하세요! (Annyeonghaseyo!) 你好！(Nǐ hǎo!) გამარჯობა! (Gamardjoba!) Բարև Ձեզ! (Barev Dzez!) Γεια σας! (Geia sas!) Merhaba! Исәнмесез! (Isänmesez!)"),
        };

        foreach (var phrase in phrases)
        {
            byte[] dataBytes = Encoding.UTF8.GetBytes(phrase.Item2);
            UIntPtr count = tokenator_count(dataBytes, (UIntPtr)dataBytes.Length);
            
            Console.WriteLine($"phrase ({(int)count}): {phrase.Item2}");
            if ((int)count != phrase.Item1)
            {
                Console.WriteLine($"Error: Expected {phrase.Item1} but got {(int)count}");
                return;
            }            
            
        }
    }
}

// ar -xv ../../libs/libtokenator.a
// gcc -shared -o libtokenator.so ./*.o -licuuc -licui18n -lm -lstdc++
// rm ./*.o
// ldd libtokenator.so

// dotnet new console
// dotnet run
