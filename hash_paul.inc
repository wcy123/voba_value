// http://www.azillionmonkeys.com/qed/hash.html
/*
IMPORTANT NOTE: Since there has been a lot of interest for the code below, I have decided to additionally provide it under the LGPL 2.1 license. This provision applies to the code below only and not to any other code including other source archives or listings from this site unless otherwise specified.

The LGPL 2.1 is not necessarily a more liberal license than my derivative license, but this additional licensing makes the code available to more developers. Note that this does not give you multi-licensing rights. You can only use the code under one of the licenses at a time. 

*/
#include <stdio.h>
#include <stdint.h>


#undef get16bits
#if (defined(__GNUC__) && defined(__i386__)) || defined(__WATCOMC__) \
  || defined(_MSC_VER) || defined (__BORLANDC__) || defined (__TURBOC__)
#define get16bits(d) (*((const uint16_t *) (d)))
#endif

#if !defined (get16bits)
#define get16bits(d) ((((uint32_t)(((const uint8_t *)(d))[1])) << 8)\
                      +(uint32_t)(((const uint8_t *)(d))[0]) )
#endif

static uint32_t SuperFastHash (const char * data, int len) {
  uint32_t hash = len, tmp;
  int rem;

  
  if (len <= 0 || data == NULL) return 0;

  if(0){
    rem = (4 - (((intptr_t)data) & 3)) & 3;
    rem = rem > len?len:rem;
  
    switch (rem) {
    case 3: hash += get16bits (data);
      hash ^= hash << 16;
      hash ^= ((signed char)data[sizeof (uint16_t)]) << 18;
      hash += hash >> 11;
      break;
    case 2: hash += get16bits (data);
      hash ^= hash << 11;
      hash += hash >> 17;
      break;
    case 1: hash += (signed char)*data;
      hash ^= hash << 10;
      hash += hash >> 1;
    }
    len -= rem;
    data = data + rem;
  }

  rem = len & 3;
  len >>= 2;
  
  /* Main loop */
  for (;len > 0; len--) {
    hash  += get16bits (data);
    tmp    = (get16bits (data+2) << 11) ^ hash;
    hash   = (hash << 16) ^ tmp;
    data  += 2*sizeof (uint16_t);
    hash  += hash >> 11;
  }

  /* Handle end cases */
  switch (rem) {
  case 3: hash += get16bits (data);
    hash ^= hash << 16;
    hash ^= ((signed char)data[sizeof (uint16_t)]) << 18;
    hash += hash >> 11;
    break;
  case 2: hash += get16bits (data);
    hash ^= hash << 11;
    hash += hash >> 17;
    break;
  case 1: hash += (signed char)*data;
    hash ^= hash << 10;
    hash += hash >> 1;
  }

  /* Force "avalanching" of final 127 bits */
  hash ^= hash << 3;
  hash += hash >> 5;
  hash ^= hash << 4;
  hash += hash >> 17;
  hash ^= hash << 25;
  hash += hash >> 6;

  return hash;
}
