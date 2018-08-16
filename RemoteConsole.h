/*
 * =====================================================================================
 *
 *       Filename:  RemoteConsole.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  08/08/2018 19:01:06
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Stuart B. Wilkins (sbw), stuwilkins@mac.com
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef REMOTE_CONSOLE_H
#define REMOTE_CONSOLE_H

#define TELNET_PORT 23

class RemoteConsole: public Print
{
  public:
    bool begin(int baud = 115200, int ser_wait = 0);
    virtual size_t write(uint8_t character);
    bool loop(void);
    bool connect(void);
    RemoteConsole(size_t buffer_size = 1000);
    ~RemoteConsole();
  private:
    uint8_t *_buffer;
    long int _buffer_len;
    unsigned long _last_loop = 0;
    WiFiServer *_server;
    void advance(void);
    long int _pos;
    long int _len;
    long int _start;
    bool _connected;
    int _chunk;
};

#endif
