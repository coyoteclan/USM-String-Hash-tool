// ConsoleApplication2.cpp : Questo file contiene la funzione 'main', in cui inizia e termina l'esecuzione del programma.
//

#include <iostream>

#include <iostream>
#include <cctype>  // for isalpha() and tolower()

unsigned int __cdecl hash_string(const char* a1)
{
    const char* v1; // esi
    unsigned int v2; // edi
    int v3; // eax

    v1 = a1;
    v2 = 0;
    if (*a1)
    {
        do
        {
            if (isalpha(*v1))
                v3 = tolower(*v1);
            else
                v3 = *v1;
            v2 = v3 + 33 * v2;
        } while (*++v1);
    }
    return v2;
}

int main() {
    const char* input_string = "PK_S01_FATHERS_PRIDE";  // Example string to hash
    unsigned int result = hash_string(input_string);
    std::cout << "Hash value: " << result << std::endl;
    return 0;
}
