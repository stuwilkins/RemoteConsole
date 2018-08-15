/*
 * =====================================================================================
 *
 *       Filename:  RemoteConsole.cpp
 *
 *    Description:  Remote Console for Arduino
 *
 *        Version:  1.0
 *        Created:  08/08/2018 18:59:40
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Stuart B. Wilkins (sbw), stuwilkins@mac.com
 *   Organization:  
 *
 * =====================================================================================
 */

#include <WiFi101.h>
#include "RemoteConsole.h"

RemoteConsole::RemoteConsole()
{
  _buffer = (uint8_t *)malloc(sizeof(uint8_t) * 10000);
  _buffer_len = 10000;
  _server = new WiFiServer(TELNET_PORT);
  _pos = 0;
  _len = 0;
  _start = 0;
  _trigger = false;
}

RemoteConsole::~RemoteConsole()
{
  free(_buffer);
  delete _server;
}

bool RemoteConsole::begin(String hostname)
{
  Serial.begin(115200);
  return true;
}

bool RemoteConsole::connect(void)
{
  _server->begin(); 
  return true;
}

bool RemoteConsole::loop(void)
{
  WiFiClient client = _server->available();
  if(client)
  {
    if(client.connected())
    {
      if(client.available())
      {
        if((_start + _len) > _buffer_len)
        {
          int stop = _start + _len - _buffer_len;
          client.write(_buffer + _start, _buffer_len - _start);
          client.write(_buffer, stop);
        } else {
          client.write(_buffer + _start, _len);
        }

        // Now set the length to zero
        _len = 0;
        // Now we set the start position to the current position.
        _start = _pos;
      }
    }
  }

  return true;
}

size_t RemoteConsole::write(uint8_t character)
{
  Serial.write(character);
  _buffer[_pos] = character;
  advance();
  return 1;
}

void RemoteConsole::advance(void)
{
  _pos++;
  if(_pos >= _buffer_len)
  {
    _pos = 0;
  }
  if(_len < _buffer_len)
  {
    _len++;
  }
}

