## Performance test of various methods to remove specific characters in a `std::string`

There are many ways to remove specific characters from a `std::string`.
This test compares how following 4 methods perform:
1. naive find and erase   
   ```c++
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
   ```
2. erase-remove idiom for each characters   
   ```c++
   std::string& using_erase_remove_idiom_for_each_char(std::string& aString)
   {
       aString.erase(std::remove(aString.begin(), aString.end(), '\n'), aString.end());
       aString.erase(std::remove(aString.begin(), aString.end(), '\r'), aString.end());
       return aString;
   }
   ```
3. erase-remove_if for all of the characters at once using lambda   
   ```c++
   std::string& using_erase_remove_if(std::string& aString)
   {
       aString.erase(std::remove_if(aString.begin(),
                                    aString.end(),
                                    [](const char c) { return (c == '\n' || c == '\r') ? true : false; }),
                     aString.end());
       return aString;
   }
   ```
4. erase-remove_if with `boost::is_any_of`
   ```c++
   std::string& using_erase_remove_if_with_boostany(std::string& aString)
   {
       aString.erase(std::remove_if(aString.begin(),
                                    aString.end(),
                                    boost::is_any_of("\r\n")), aString.end());
       return aString;
   }
   ```

For comparison, the test generates vector of strings.
The length of the strings that consist the vector varies from `min_len` to `max_len` randomly.
The occurence rate of the characters to remove is provided in the command line.

## Result with gcc
### No optimization
```bash
irteam@cmiddev02.nm:~/shawn/test/remove_char_test$ make
gcc -std=c++17 -I../../index.1/externals/libboost/include -c -g -O0 -Wall -o foo.o foo.cpp
gcc -o foo foo.o -lstdc++ -Wall
irteam@cmiddev02.nm:~/shawn/test/remove_char_test$ ./foo 1000000 5 500 5 2>/dev/null
Generating 1,000,000 strings of length 5 ~ 500 with newline char rate 5%
>>> Round 0
Testing with method original                  --> Took 8,851,361us (8,851ms)
Testing with method erase_remove              --> Took 17,652,058us (17,652ms)
Testing with method erase_remove_if           --> Took 11,490,593us (11,490ms)
Testing with method erase_remove_boost_any    --> Took 25,685,412us (25,685ms)
>>> Round 1
Testing with method original                  --> Took 8,928,885us (8,928ms)
Testing with method erase_remove              --> Took 17,310,450us (17,310ms)
Testing with method erase_remove_if           --> Took 10,638,404us (10,638ms)
Testing with method erase_remove_boost_any    --> Took 23,048,423us (23,048ms)
>>> Round 2
Testing with method original                  --> Took 7,931,405us (7,931ms)
Testing with method erase_remove              --> Took 15,819,357us (15,819ms)
Testing with method erase_remove_if           --> Took 10,050,048us (10,050ms)
Testing with method erase_remove_boost_any    --> Took 23,030,471us (23,030ms)
>>> Round 3
Testing with method original                  --> Took 7,932,825us (7,932ms)
Testing with method erase_remove              --> Took 15,810,920us (15,810ms)
Testing with method erase_remove_if           --> Took 10,048,968us (10,048ms)
Testing with method erase_remove_boost_any    --> Took 23,030,787us (23,030ms)

irteam@cmiddev02.nm:~/shawn/test/remove_char_test$ ./foo 1000000 5 500 15 2>/dev/null
Generating 1,000,000 strings of length 5 ~ 500 with newline char rate 15%
>>> Round 0
Testing with method original                  --> Took 19,203,548us (19,203ms)
Testing with method erase_remove              --> Took 18,565,923us (18,565ms)
Testing with method erase_remove_if           --> Took 12,160,880us (12,160ms)
Testing with method erase_remove_boost_any    --> Took 27,348,996us (27,348ms)
>>> Round 1
Testing with method original                  --> Took 19,590,544us (19,590ms)
Testing with method erase_remove              --> Took 18,548,597us (18,548ms)
Testing with method erase_remove_if           --> Took 12,169,197us (12,169ms)
Testing with method erase_remove_boost_any    --> Took 27,730,838us (27,730ms)
>>> Round 2
Testing with method original                  --> Took 19,659,803us (19,659ms)
Testing with method erase_remove              --> Took 18,323,925us (18,323ms)
Testing with method erase_remove_if           --> Took 12,024,273us (12,024ms)
Testing with method erase_remove_boost_any    --> Took 27,519,646us (27,519ms)
>>> Round 3
Testing with method original                  --> Took 19,256,925us (19,256ms)
Testing with method erase_remove              --> Took 18,749,896us (18,749ms)
Testing with method erase_remove_if           --> Took 12,546,236us (12,546ms)
Testing with method erase_remove_boost_any    --> Took 29,113,302us (29,113ms)

irteam@cmiddev02.nm:~/shawn/test/remove_char_test$ ./foo 1000000 5 500 25 2>/dev/null
Generating 1,000,000 strings of length 5 ~ 500 with newline char rate 25%
>>> Round 0
Testing with method original                  --> Took 29,161,098us (29,161ms)
Testing with method erase_remove              --> Took 18,462,078us (18,462ms)
Testing with method erase_remove_if           --> Took 12,819,577us (12,819ms)
Testing with method erase_remove_boost_any    --> Took 28,448,288us (28,448ms)
>>> Round 1
Testing with method original                  --> Took 28,880,973us (28,880ms)
Testing with method erase_remove              --> Took 18,821,385us (18,821ms)
Testing with method erase_remove_if           --> Took 12,995,210us (12,995ms)
Testing with method erase_remove_boost_any    --> Took 29,414,630us (29,414ms)
>>> Round 2
Testing with method original                  --> Took 29,020,646us (29,020ms)
Testing with method erase_remove              --> Took 19,008,157us (19,008ms)
Testing with method erase_remove_if           --> Took 12,903,553us (12,903ms)
Testing with method erase_remove_boost_any    --> Took 29,533,619us (29,533ms)
>>> Round 3
Testing with method original                  --> Took 29,141,268us (29,141ms)
Testing with method erase_remove              --> Took 18,825,153us (18,825ms)
Testing with method erase_remove_if           --> Took 12,088,616us (12,088ms)
Testing with method erase_remove_boost_any    --> Took 26,774,617us (26,774ms)
```

The performance of naive find and erase is heavily dependent on the occurence rate of the characters to remove.
On the other hand, methods using STL showed a consistent performance regardless of how many characters it should remove.

But, this is without optimization, far from the reallity.

### Optimization level 2
```bash
irteam@cmiddev02.nm:~/shawn/test/remove_char_test$ make clean;
rm -rf foo *.o core*
irteam@cmiddev02.nm:~/shawn/test/remove_char_test$ make release
gcc -std=c++17 -I../../index.1/externals/libboost/include -c -g -O2 -Wall -o foo.o foo.cpp
gcc -o foo foo.o -lstdc++ -Wall

irteam@cmiddev02.nm:~/shawn/test/remove_char_test$ ./foo 1000000 5 500 5 2>/dev/null
Generating 1,000,000 strings of length 5 ~ 500 with newline char rate 5%
>>> Round 0
Testing with method original                  --> Took 4,931,711us (4,931ms)
Testing with method erase_remove              --> Took 3,575,024us (3,575ms)
Testing with method erase_remove_if           --> Took 3,450,444us (3,450ms)
Testing with method erase_remove_boost_any    --> Took 4,508,955us (4,508ms)
>>> Round 1
Testing with method original                  --> Took 4,954,793us (4,954ms)
Testing with method erase_remove              --> Took 3,605,543us (3,605ms)
Testing with method erase_remove_if           --> Took 3,472,859us (3,472ms)
Testing with method erase_remove_boost_any    --> Took 4,539,388us (4,539ms)
>>> Round 2
Testing with method original                  --> Took 4,937,949us (4,937ms)
Testing with method erase_remove              --> Took 3,373,128us (3,373ms)
Testing with method erase_remove_if           --> Took 3,188,154us (3,188ms)
Testing with method erase_remove_boost_any    --> Took 4,086,752us (4,086ms)
>>> Round 3
Testing with method original                  --> Took 4,595,547us (4,595ms)
Testing with method erase_remove              --> Took 3,357,893us (3,357ms)
Testing with method erase_remove_if           --> Took 3,185,814us (3,185ms)
Testing with method erase_remove_boost_any    --> Took 4,085,493us (4,085ms)

irteam@cmiddev02.nm:~/shawn/test/remove_char_test$ ./foo 1000000 5 500 15 2>/dev/null
Generating 1,000,000 strings of length 5 ~ 500 with newline char rate 15%
>>> Round 0
Testing with method original                  --> Took 9,326,412us (9,326ms)
Testing with method erase_remove              --> Took 4,219,023us (4,219ms)
Testing with method erase_remove_if           --> Took 4,124,172us (4,124ms)
Testing with method erase_remove_boost_any    --> Took 5,325,586us (5,325ms)
>>> Round 1
Testing with method original                  --> Took 9,273,824us (9,273ms)
Testing with method erase_remove              --> Took 4,336,788us (4,336ms)
Testing with method erase_remove_if           --> Took 4,078,378us (4,078ms)
Testing with method erase_remove_boost_any    --> Took 5,454,975us (5,454ms)
>>> Round 2
Testing with method original                  --> Took 9,131,749us (9,131ms)
Testing with method erase_remove              --> Took 4,325,969us (4,325ms)
Testing with method erase_remove_if           --> Took 4,167,050us (4,167ms)
Testing with method erase_remove_boost_any    --> Took 5,373,508us (5,373ms)
>>> Round 3
Testing with method original                  --> Took 8,905,841us (8,905ms)
Testing with method erase_remove              --> Took 3,905,645us (3,905ms)
Testing with method erase_remove_if           --> Took 3,702,636us (3,702ms)
Testing with method erase_remove_boost_any    --> Took 4,931,676us (4,931ms)

irteam@cmiddev02.nm:~/shawn/test/remove_char_test$ ./foo 1000000 5 500 25 2>/dev/null
Generating 1,000,000 strings of length 5 ~ 500 with newline char rate 25%
>>> Round 0
Testing with method original                  --> Took 11,726,521us (11,726ms)
Testing with method erase_remove              --> Took 4,402,533us (4,402ms)
Testing with method erase_remove_if           --> Took 4,145,872us (4,145ms)
Testing with method erase_remove_boost_any    --> Took 5,688,418us (5,688ms)
>>> Round 1
Testing with method original                  --> Took 11,707,815us (11,707ms)
Testing with method erase_remove              --> Took 4,376,575us (4,376ms)
Testing with method erase_remove_if           --> Took 4,017,496us (4,017ms)
Testing with method erase_remove_boost_any    --> Took 5,670,985us (5,670ms)
>>> Round 2
Testing with method original                  --> Took 11,846,661us (11,846ms)
Testing with method erase_remove              --> Took 4,344,223us (4,344ms)
Testing with method erase_remove_if           --> Took 3,963,364us (3,963ms)
Testing with method erase_remove_boost_any    --> Took 5,401,672us (5,401ms)
>>> Round 3
Testing with method original                  --> Took 11,123,202us (11,123ms)
Testing with method erase_remove              --> Took 4,159,829us (4,159ms)
Testing with method erase_remove_if           --> Took 3,933,766us (3,933ms)
Testing with method erase_remove_boost_any    --> Took 5,390,121us (5,390ms)
```
The result consistently showed that the `erase_remove_if` method with lambda is the fastest.

However, if the program in compiled with clang, the difference is not that big.

## Result with clang
### No optimization
### Optimize level 2

```
