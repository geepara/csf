#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include "fixedpoint.h"

Fixedpoint fixedpoint_create(uint64_t whole)
{
  Fixedpoint num;
  num.whole = whole;
  num.frac = 0UL;
  num.negative = 0;
  num.error = 0;
  num.overflow = 0;
  num.underflow = 0;
  return num;
}

Fixedpoint fixedpoint_create2(uint64_t whole, uint64_t frac)
{
  Fixedpoint num;
  num.whole = whole;
  num.frac = frac;
  num.negative = 0;
  num.error = 0;
  num.overflow = 0;
  num.underflow = 0;
  return num;
}

Fixedpoint fixedpoint_create_from_hex(const char *hex)
{
  Fixedpoint num;
  // check if its error
  int size = strlen(hex);
  num.error = 0;
  int wholePartCounter = 0;
  int fracPartCounter = 0;
  char *currentPart = "whole";
  int numDots = 0;
  if (size > 34)
  {
    num.error = 1;
  }
  else
  {
    for (int i = 0; i < size; i++)
    {
      char c = hex[i];
      if ((c < 'a' || c > 'f') && (c < '0' || c > '9'))
      {
        if (c != '.' && c != '-')
        {
          num.error = 1;
        }
        if (c == '.')
        {
          numDots++;
          currentPart = "frac";
        }
        if (c == '-' && i != 0)
        {
          num.error = 1;
        }
      }
      else
      {
        if (strcmp(currentPart, "whole") == 0)
        {
          wholePartCounter++;
        }
        else
        {
          fracPartCounter++;
        }
      }
    }
  }
  if (wholePartCounter > 16 || fracPartCounter > 16 || numDots > 1)
  {
    num.error = 1;
  }
  // check if its negative and get whole part
  char *endptr;
  if (strchr(hex, '-') != NULL)
  {
    num.negative = 1;
    num.whole = (uint64_t)strtoul(hex + 1, &endptr, 16);
  }
  else
  {
    num.negative = 0;
    num.whole = (uint64_t)strtoul(hex, &endptr, 16);
  }

  // conversion from hex to uint64_t of frac part
  if (*endptr == '.')
  {
    char *fracHex = endptr + 1;
    int sizeFracHex = strlen(fracHex);
    char fracHexConverted[16];
    for (int i = 0; i < 16; i++)
    {
      if (i < sizeFracHex)
      {
        fracHexConverted[i] = fracHex[i];
      }
      else
      {
        fracHexConverted[i] = '0';
      }
    }
    char *endptr2;
    num.frac = (uint64_t)strtoul(fracHexConverted, &endptr2, 16);
  }

  num.overflow = 0;
  num.underflow = 0;

  return num;
}

uint64_t fixedpoint_whole_part(Fixedpoint val)
{
  return val.whole;
}

uint64_t fixedpoint_frac_part(Fixedpoint val)
{
  return val.frac;
}

Fixedpoint fixedpoint_add(Fixedpoint left, Fixedpoint right)
{
  Fixedpoint num;
  num.negative = 0;
  num.error = 0;
  num.overflow = 0;
  num.underflow = 0;

  if (fixedpoint_is_zero(left))
  {
    return right;
  }
  if (fixedpoint_is_zero(right))
  {
    return left;
  }

  if (left.negative == right.negative)
  {
    num.negative = left.negative;
    num.whole = left.whole + right.whole;
    if (num.whole < left.whole && num.whole < right.whole)
    {
      num.overflow = 1;
    }
    num.frac = left.frac + right.frac;
    if (num.frac < left.frac && num.frac < right.frac)
    {
      num.whole++;
    }
  }
  else
  {
    if (right.negative == 1)
    {
      return fixedpoint_sub(left, fixedpoint_negate(right));
    }
    else if (left.negative == 1)
    {
      return fixedpoint_sub(right, fixedpoint_negate(left));
    }
  }

  return num;
}

Fixedpoint fixedpoint_sub(Fixedpoint left, Fixedpoint right)
{
  Fixedpoint num;
  num.negative = 0;
  num.error = 0;
  num.overflow = 0;
  num.underflow = 0;

  if (left.negative == 0 && right.negative == 0)
  {
    if (fixedpoint_is_zero(right))
    {
      return left;
    }
    if (fixedpoint_is_zero(left))
    {
      return fixedpoint_negate(right);
    }

    if (left.whole > right.whole)
    {
      num.whole = left.whole - right.whole;

      if (left.frac > right.frac)
      {
        num.frac = left.frac - right.frac;
      }
      else if (right.frac > left.frac)
      {
        num.frac = left.frac - right.frac;
        num.whole--;
      }
      else
      {
        num.frac = 0;
      }

      //   5.3   5.4   5.1
      // - 3.5   3.7   3.4
      //   1.8   1.7   1.7
    }
    else if (right.whole > left.whole)
    {
      return fixedpoint_negate(fixedpoint_sub(right, left));
      //   3.5   3.7
      // - 5.3   5.4
      //  -1.8  -1.7
      //   num.whole = right.whole - left.whole;
      //   num.negative = 1;

      //   if (left.frac > right.frac)
      //   {
      //     num.frac = left.frac + right.frac;
      //   }
      //   else if (right.frac > left.frac)
      //   {
      //   }
      //   else
      //   {
      //     num.frac = 0;
      //   }
    }
    else
    {
      num.whole = 0;
    }
  }
  else if (left.negative == 0 && right.negative == 1)
  {
    return fixedpoint_add(left, fixedpoint_negate(right));
  }
  else if (left.negative == 1 && right.negative == 0)
  {
    return fixedpoint_negate(fixedpoint_add(fixedpoint_negate(left), right));
  }
  else
  {
    return fixedpoint_sub(fixedpoint_negate(right), fixedpoint_negate(left));
  }
  return num;
  // -a - (-b) = -a + b
}

Fixedpoint fixedpoint_negate(Fixedpoint val)
{
  if (!fixedpoint_is_zero(val))
  {
    if (val.negative == 0)
    {
      val.negative = 1;
    }
    else
    {
      val.negative = 0;
    }
  }
  return val;
}
Fixedpoint fixedpoint_halve(Fixedpoint val)
{
  if (!fixedpoint_is_zero(val))
  {
    Fixedpoint num = val;
    int wholeUnderflow = 0;

    num.whole = val.whole >> 1;
    if (val.whole % 2 != 0)
    {
      wholeUnderflow = 1;
      num.underflow = 1;
    }

    num.frac = val.frac >> 1;
    if (val.frac % 2 != 0)
    {
      num.underflow = 1;
    }

    if (wholeUnderflow == 1)
    {
      num.frac += 0x8000000000000000UL;
    }

    return num;
  }
  return val;
}

Fixedpoint fixedpoint_double(Fixedpoint val)
{
  if (!fixedpoint_is_zero(val))
  {
    Fixedpoint num = val;

    num.whole = val.whole << 1;
    if (num.whole < val.whole)
    {
      num.overflow = 1;
    }

    num.frac = val.frac << 1;
    if (num.frac < val.frac)
    {
      num.whole++;
    }

    return num;
  }

  return val;
}

int fixedpoint_compare(Fixedpoint left, Fixedpoint right)
{
  if (left.negative == 0 && right.negative == 1)
  {
    return 1;
  }
  else if (left.negative == 1 && right.negative == 0)
  {
    return -1;
  }
  else
  {
    int negative = left.negative;
    if (negative == 0)
    {
      if (left.whole > right.whole)
      {
        return 1;
      }
      else if (right.whole > left.whole)
      {
        return -1;
      }
      else
      {
        if (left.frac > right.frac)
        {
          return 1;
        }
        else if (right.frac > left.frac)
        {
          return -1;
        }
        else
        {
          return 0;
        }
      }
    }
    else if (negative == 1)
    {
      if (left.whole > right.whole)
      {
        return -1;
      }
      else if (right.whole > left.whole)
      {
        return 1;
      }
      else
      {
        if (left.frac > right.frac)
        {
          return -1;
        }
        else if (right.frac > left.frac)
        {
          return 1;
        }
        else
        {
          return 0;
        }
      }
    }
  }

  return 0;
}

int fixedpoint_is_zero(Fixedpoint val)
{
  if (fixedpoint_is_valid(val) && val.whole == 0UL && val.frac == 0UL)
  {
    return 1;
  }
  return 0;
}

int fixedpoint_is_err(Fixedpoint val)
{
  return val.error;
}

int fixedpoint_is_neg(Fixedpoint val)
{
  return val.negative;
}

int fixedpoint_is_overflow_neg(Fixedpoint val)
{
  if (val.negative == 1 && val.overflow == 1)
  {
    return 1;
  }
  return 0;
}

int fixedpoint_is_overflow_pos(Fixedpoint val)
{
  if (val.negative == 0 && val.overflow == 1)
  {
    return 1;
  }
  return 0;
}

int fixedpoint_is_underflow_neg(Fixedpoint val)
{
  if (val.negative == 1 && val.underflow == 1)
  {
    return 1;
  }
  return 0;
}

int fixedpoint_is_underflow_pos(Fixedpoint val)
{
  if (val.negative == 0 && val.underflow == 1)
  {
    return 1;
  }
  return 0;
}

int fixedpoint_is_valid(Fixedpoint val)
{
  if (val.error == 0 && val.underflow == 0 && val.overflow == 0)
  {
    return 1;
  }
  return 0;
}

char *fixedpoint_format_as_hex(Fixedpoint val)
{
  char *num = malloc(34);
  uint64_t whole = val.whole;
  uint64_t frac = val.frac;
  int i = 0;
  if (val.negative == 1)
  {
    num[0] = '-';
    i = 1;
  }
  if (fixedpoint_is_zero(val))
  {
    num[0] = '0';
    i++;
  }
  else
  {
    if (whole != 0)
    {
      int numDigits = 0;
      while (whole != 0)
      {
        whole = whole >> 4;
        num[i + numDigits] = '0';
        numDigits++;
      }
      i = i + numDigits - 1;
      whole = val.whole;
      while (whole != 0)
      {
        int temp = whole % 16;
        if (temp > 9)
        {
          num[i] = (char)(temp + 87);
        }
        else
        {
          num[i] = (char)(temp + 48);
        }
        whole = whole >> 4;
        i--;
      }
      i = i + numDigits + 1;
    }
    else
    {
      num[i] = '0';
      i++;
    }
    if (frac != 0)
    {
      num[i] = '.';
      i++;
      int j;
      for (j = i; j < 16 + i; j++)
      {
        num[j] = '0';
      }
      num[j] = '\0';
      i = i + 16 - 1;
      while (frac != 0)
      {
        int temp = frac % 16;
        if (temp > 9)
        {
          num[i] = (char)(temp + 87);
        }
        else
        {
          num[i] = (char)(temp + 48);
        }
        frac = frac >> 4;
        i--;
      }
      i = i + 16 - 1;
      while (num[i] == '0')
      {
        i--;
      }
      i++;
    }
  }
  num[i] = '\0';
  return num;
}
