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

RemoteConsole::RemoteConsole(size_t buffer_size)
{
  _buffer = (uint8_t *)malloc(sizeof(uint8_t) * buffer_size);
  _buffer_len = buffer_size;
  _server = new WiFiServer(TELNET_PORT);
  _pos = 0;
  _len = 0;
  _start = 0;
  _connected = false;
  _chunk = 80;
}

RemoteConsole::~RemoteConsole()
{
  free(_buffer);
  delete _server;
}

bool RemoteConsole::begin(int baud, int ser_wait)
{
  Serial.begin(baud);
  if(ser_wait)
  {
    long int start = millis();
    while(!Serial)
    {
      if((millis() - start) > (ser_wait * 1000L))
      {
        break;
      }
    }
  }

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

  if(client.connected())
  {
    while(client.available())
    {
      char c = client.read();
      //Serial.print(F("Char = "));
      //Serial.print(F(" 0x"));
      //Serial.println((int)c, HEX);
      if(c == 'D')
      {
        client.stop();
      }
    }
  }

  long int _sent = 0;
  long int _target_len = _len;

  while(_sent != _target_len)
  {
    int chunk = (_len > _chunk) ? _chunk : _len;

    int written;
    if((_start + chunk) > _buffer_len)
    {
      int stop = _start + chunk - _buffer_len;
      written = _server->write(_buffer + _start, _buffer_len - _start);
      written += _server->write(_buffer, stop);
    } else {
      written = _server->write(_buffer + _start, chunk);
    }

    if(written == 0)
    {
      // No connection
      _connected = false;
      break;
    }

    _connected = true;
    _len -= written;
    _start += written;
    _sent += written;
    if(_start >= _buffer_len)
    {
      _start -= _buffer_len;
    }
  }

  return true;
}

size_t RemoteConsole::write(uint8_t character)
{
  //Serial.print("_pos = ");
  //Serial.print(_pos);
  //Serial.print(" _len = ");
  //Serial.println(_len);

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

