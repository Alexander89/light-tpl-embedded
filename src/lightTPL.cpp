#ifdef ARDUINO
#include <Arduino.h>
#else
#include <cstdio>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#endif
#include "lightTPL.h"

LightTpl::LightTpl(const char *tpl)
{
  this->tpl = tpl;
  counter = 0;
  memset(keys, 0, sizeof(keys));
  memset(owns, false, sizeof(owns));
  memset(values, 0, sizeof(values));
}
LightTpl::~LightTpl()
{
  for (int_fast16_t i = 0; i < counter; i++)
    if (owns[i] == true)
      free((void *)values[i]);

  memset(keys, 0, sizeof(keys));
  memset(owns, false, sizeof(owns));
  memset(values, 0, sizeof(values));

  counter = 0;
}

void LightTpl::setValue(const char *key, const char *value)
{
  owns[counter] = false;
  keys[counter] = key;
  values[counter] = value;
  counter++;
}

void LightTpl::setValue(const char *key, int value)
{
  auto ownValue = (char *)malloc(sizeof(char) * 12);

#if ARDUINO
  itoa(value, ownValue, 10);
#else
  sprintf(ownValue, "%d", value);
#endif

  owns[counter] = true;
  keys[counter] = key;
  values[counter] = ownValue;

  counter++;
}

void LightTpl::setValue(const char *key, unsigned int value)
{
  auto ownValue = (char *)malloc(sizeof(char) * 12);

#if ARDUINO
  utoa(value, ownValue, 10);
#else
  sprintf(ownValue, "%u", value);
#endif

  owns[counter] = true;
  keys[counter] = key;
  values[counter] = ownValue;

  counter++;
}

void LightTpl::setValue(const char *key, short value)
{
  auto ownValue = (char *)malloc(sizeof(char) * 6);
#if ARDUINO
  itoa(value, ownValue, 10);
#else
  sprintf(ownValue, "%d", value);
#endif

  owns[counter] = true;
  keys[counter] = key;
  values[counter] = ownValue;

  counter++;
}

void LightTpl::setValue(const char *key, unsigned short value)
{
  auto ownValue = (char *)malloc(sizeof(char) * 6);
#if ARDUINO
  utoa(value, ownValue, 10);
#else
  sprintf(ownValue, "%u", value);
#endif

  owns[counter] = true;
  keys[counter] = key;
  values[counter] = ownValue;

  counter++;
}
///
/// dec: number of digits after the "."
void LightTpl::setValue(const char *key, float value, char dec)
{
  setValue(key, (double)value, dec);
}

void LightTpl::setValue(const char *key, double value, char dec)
{
  auto ownValue = (char *)malloc(sizeof(char) * 12);
#if ARDUINO
  dtostrf(value, 12, dec, ownValue);
#else
  sprintf(ownValue, "%f", value);
#endif

  owns[counter] = true;
  keys[counter] = key;
  values[counter] = ownValue;

  counter++;
}

void LightTpl::render(std::function<void(const char)> write)
{
  bool replaceMode{false};
  int_fast16_t keyLng;

  // run for NTS (null terminated string)
  for (size_t i = 0; tpl[i] != 0; i++)
  {
    // +1 is ok, because of the 0 terminated string. It could be 0 on the end
    if (tpl[i] == '{' && tpl[i + 1] == '{')
    {
      keyLng = 0;
      replaceMode = true;
      i++;
    }
    else if (replaceMode && tpl[i] == '}' && tpl[i + 1] == '}')
    {
      const char *key = (tpl + i - keyLng);
      auto idx = findKeys(key, keyLng);

      const char *value;
      if (idx.isOk())
        value = values[idx.result];
      else
        value = "\'ERROR: Not Found\'";

      for (size_t i = 0; value[i] != 0; i++)
        write(value[i]);

      replaceMode = false;
      i++;
    }
    else if (replaceMode)
    {
      if (keyLng > LTPL_MAX_KEY_LNG)
      {
        const char *value = "\'ERROR: Key doesn't end or to long\' {{";
        for (size_t i = 0; value[i] != 0; i++)
          write(value[i]);
        i -= LTPL_MAX_KEY_LNG;
        replaceMode = false;
      }
      else
      {
        keyLng++;
      }
    }
    else
    {
      write(tpl[i]);
    }
  }
}

int compKey(const char *key, size_t lng, const char *nts)
{
  size_t i{0};
  for (; i < lng; i++)
  {
    char res = key[i] - nts[i];
    if (res != 0)
      return res;
  }
  if (nts[i] != 0)
    return -1;
  else
    return 0;
}

Result<int_fast16_t> LightTpl::findKeys(const char *key, size_t lng)
{
  for (int_fast16_t i = 0; i < counter; i++)
  {
    if (compKey(key, lng, keys[i]) == 0)
      return ok(int_fast16_t, i);
  }
  return err(int_fast16_t, "not found");
}
