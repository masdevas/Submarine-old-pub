#include <immintrin.h>
#include <inttypes.h>

inline void Load(const uint8_t * src, __m256i a[3])
{
    a[0] = _mm256_loadu_si256((__m128i*)(src - 1)); //загрузка 256 битного вектора по невыровненному по 32 битной границе адресу
    a[1] = _mm256_loadu_si256((__m128i*)(src));
    a[2] = _mm256_loadu_si256((__m128i*)(src + 1));
}

inline void Load2(const uint8_t * src, size_t stride, __m256i a[9])
{
    Load(src - stride, a + 0);
    Load(src, a + 3);
    Load(src + stride, a + 6);
}

inline void Sort(__m256i& a, __m256i& b)
{
    __m256i t = a;
    a = _mm256_min_epu8(t, b); //нахождение минимума 2-х 8 битных беззнаковых чисел для каждого из 32 значений вектора
    b = _mm256_max_epu8(t, b); //нахождение максимума 2-х 8 битных беззнаковых чисел для каждого из 32 значений вектора
}

inline void Sort2(__m256i a[9]) //частично сортирует весь массив
{
    Sort(a[1], a[2]); Sort(a[4], a[5]); Sort(a[7], a[8]); 
    Sort(a[0], a[1]); Sort(a[3], a[4]); Sort(a[6], a[7]);
    Sort(a[1], a[2]); Sort(a[4], a[5]); Sort(a[7], a[8]); 
    Sort(a[0], a[3]); Sort(a[5], a[8]); Sort(a[4], a[7]);
    Sort(a[3], a[6]); Sort(a[1], a[4]); Sort(a[2], a[5]); 
    Sort(a[4], a[7]); Sort(a[4], a[2]); Sort(a[6], a[4]);
    Sort(a[4], a[2]);
}

void MedianFilter(const uint8_t * src, size_t srcStride, size_t width, size_t height, uint8_t * dst, size_t dstStride)
{
    __m256i a[9];
    for(size_t y = 0; y < height; ++y)
    {
        for(size_t x = 0; x < width;  x += sizeof(__m256i))
        {
            Load2(src + x, srcStride, a);
            Sort2(a);
            _mm256_storeu_si256((__m256i*)(dst + x), a[4]); //сохранение 256 битного вектора по невыровненному по 32 битной границе адресу
        }
        src += srcStride;
        dst += dstStride; 
    }
}

int main()
{
    return 0;
}