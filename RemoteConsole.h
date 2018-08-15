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
    bool begin(String hostname);
    virtual size_t write(uint8_t character);
    bool loop(void);
    bool connect(void);
    RemoteConsole();
    ~RemoteConsole();
  private:
    uint8_t *_buffer;
    int _buffer_len;
    unsigned long _last_loop = 0;
    WiFiServer *_server;
    void advance(void);
    int _pos;
    int _len;
    int _start;
    bool _trigger;
};

#endif
