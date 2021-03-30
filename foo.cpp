#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>
#include <string.h>

#include <algorithm>
#include <vector>
#include <memory>
#include <string>
#include <exception>
#include <functional>
#include <iostream>
#include <chrono>
#include <locale>

static std::string get_newline(const int32_t dice)
{
    if (dice == 0)
    {
        return "\r";
    }
    else if (dice == 1)
    {
        return "\n";
    }
    else
    {
        return "\r\n";
    }
}

static std::string gen_string(const int32_t length, const int32_t newline_rate_percent)
{
    std::string result;
    result.reserve(length);

    for (int32_t i = 0; i < length; i++)
    {
        if (int32_t dice = std::rand() % 100; dice < newline_rate_percent)
        {
            result.append(get_newline(dice % 3));
        }
        else
        {
            char c = std::rand() % (126 - 32); // 126:~ 32:space
            c += 32;
            if (c == '%' || c == '\\')
            {
                c = '_';
            }
            result.push_back(c);
        }
    }

    return result;
}

static std::vector<std::string> gen_strings(const int32_t count,
                                            const int32_t min_len,
                                            const int32_t max_len,
                                            const int32_t newline_rate_percent)
{
    std::vector<std::string> result;
    result.reserve(count);

    for (int32_t i = 0; i < count; i++)
    {
        int32_t length = std::rand() % ((max_len - min_len) + 1);
        length += min_len;

        result.push_back(gen_string(length, newline_rate_percent));
    }

    return result;
}

static void removeStrFromString(std::string& aString, const std::string& aStrToRemove)
{
    size_t sPos = 0;
    while (sPos != std::string::npos)
    {
        sPos = aString.find(aStrToRemove, sPos);
        if (sPos == std::string::npos)
        {
            break;
        }
        aString.erase(sPos, aStrToRemove.size());
    }
}

std::string& removeNewLineChar(std::string& aString)
{
    removeStrFromString(aString, "\n");
    removeStrFromString(aString, "\r");
    return aString;
}

std::string& using_erase_remove_idiom_for_each_char(std::string& aString)
{
    aString.erase(std::remove(aString.begin(), aString.end(), '\n'), aString.end());
    aString.erase(std::remove(aString.begin(), aString.end(), '\r'), aString.end());
    return aString;
}

std::string& using_erase_remove_if(std::string& aString)
{
    aString.erase(std::remove_if(aString.begin(),
                                 aString.end(),
                                 [](const char c) { return (c == '\n' || c == '\r') ? true : false; }),
                  aString.end());
    return aString;
}

typedef std::string& method_func(std::string&);

static void test(method_func* method, const char* method_name, std::vector<std::string> vec)
{
    std::cout << "Testing with method " << method_name;

    auto start_time = std::chrono::steady_clock::now();

    for (auto& str : vec)
    {
        std::cerr << "\n============================================\n";
        std::cerr << "string: \n" << str << "\n";
        std::cerr << str.length() << " --> ";
        method(str);
        std::cerr << str.length() << "\n";
        std::cerr << "result: " << str << "\n";
    }

    auto end_time = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);

    uint64_t elapsed_int = elapsed.count();
    std::cout << "    --> Took " << std::fixed << elapsed_int << "us ";
    std::cout << "(" << elapsed_int / 1000 << "ms)\n";
}

int32_t main(int32_t argc, char* argv[])
{
    std::cout.imbue(std::locale(""));

    std::srand(std::time(0));

    if (argc != 5)
    {
        std::cerr << "Usage: ./foo count min_lenght max_length newline_rate_percent\n";
        return -1;
    }

    const int32_t count = std::atoi(argv[1]);
    int32_t min_len = std::atoi(argv[2]);
    int32_t max_len = std::atoi(argv[3]);
    const int32_t newline_rate_percent = std::atoi(argv[4]);

    if (min_len > max_len)
    {
        std::swap(min_len, max_len);
    }

    std::cout << "Generating " << count << " strings of length " << min_len << " ~ " << max_len 
              << " with newline char rate " << newline_rate_percent << "%\n";
    std::vector<std::string> vec = gen_strings(count, min_len, max_len, newline_rate_percent);

    for (int32_t i = 0; i < 4; i++)
    {
        test(removeNewLineChar,                      "original       ", vec);
        test(using_erase_remove_idiom_for_each_char, "erase_remove   ", vec);
        test(using_erase_remove_if,                  "erase_remove_if", vec);

    }

    return 0;
}
