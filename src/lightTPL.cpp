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
  memset(values, 0, sizeof(values));
  memset(externValue, 0, sizeof(externValue));
}
LightTpl::LightTpl(const char *tpl, size_t lng)
{
  this->tpl = tpl;
  maxLng = lng;
  counter = 0;
  memset(keys, 0, sizeof(keys));
  memset(values, 0, sizeof(values));
  memset(externValue, 0, sizeof(externValue));
}

LightTpl::~LightTpl()
{
  memset(keys, 0, sizeof(keys));
  memset(values, 0, sizeof(values));
  memset(externValue, 0, sizeof(externValue));

  counter = 0;
}

void LightTpl::setValue(const char *key, const char *value)
{
  strncpy(keys[counter], key, LTPL_MAX_KEY_LNG);
  externValue[counter] = value;
  counter++;
}

void LightTpl::setValue(const char *key, int value)
{
#if ARDUINO
  itoa(value, values[counter], 10);
#else
  sprintf(values[counter], "%d", value);
#endif

  strncpy(keys[counter], key, LTPL_MAX_KEY_LNG);

  counter++;
}

void LightTpl::setValue(const char *key, unsigned int value)
{

#if ARDUINO
  utoa(value, values[counter], 10);
#else
  sprintf(values[counter], "%u", value);
#endif

  strncpy(keys[counter], key, LTPL_MAX_KEY_LNG);

  counter++;
}

void LightTpl::setValue(const char *key, short value)
{
#if ARDUINO
  itoa(value, values[counter], 10);
#else
  sprintf(values[counter], "%d", value);
#endif

  strncpy(keys[counter], key, LTPL_MAX_KEY_LNG);

  counter++;
}

void LightTpl::setValue(const char *key, unsigned short value)
{
#if ARDUINO
  utoa(value, values[counter], 10);
#else
  sprintf(values[counter], "%u", value);
#endif

  strncpy(keys[counter], key, LTPL_MAX_KEY_LNG);

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
#if ARDUINO
  dtostrf(value, 12, dec, values[counter]);
#else
  sprintf(values[counter], "%f", value);
#endif

  strncpy(keys[counter], key, LTPL_MAX_KEY_LNG);

  counter++;
}

void LightTpl::render(std::function<void(const char)> write)
{
  bool replaceMode{false};
  int_fast16_t keyLng;

  // run for NTS (null terminated string) or until maxLng
  for (size_t i = 0; tpl[i] != 0 && i < maxLng; i++)
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
      {
        if (externValue[idx.result])
          value = externValue[idx.result];
        else
          value = values[idx.result];
      }
      else
        value = "\'ERROR: Not Found\'";

      for (size_t j = 0; value[j] != 0; j++)
        write(value[j]);

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
    {
      Result<int_fast16_t> res{};
      res.ok = true;
      res.result = i;
      return res;
    }
  }

  Result<int_fast16_t> res{};
  res.ok = false;
  res.error = "not found";
  return res;
}
