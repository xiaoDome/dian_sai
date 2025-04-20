#ifndef UART_HPP_
#define UART_HPP_
#include<math.h>

#define HeaderShoot1 0x0a
#define ReceiveShoot 0x0b

    /**
     * @brief 发送通讯传输函数
     *
     * @param data:用于向tx发送的消息数组
     * @param length:向tx发送的消息数组的长度
     * @param need_delay:是否需要延时
     *
     * @return None
     */
    void Serial_Send(uint8_t *data, int length)
    {
        for (int i = 0; i < length; i++)
        {
            Serial.write(data[i]);
        }
        delay(5);
    }
    /**
     * @brief 接收通讯回传消息
     *
     * @param buffer:用于接收rx回传的消息数组
     * @param need_delay:是否需要延时
     *
     * @return Uart_Status_e:Uart_Receive_Success | Uart_Receive_False
     */
    Uart_Status_e Serial_Receive(uint8_t *buffer, bool need_delay = true)
    {
        if (need_delay)
            delay(10);
        for (int i = 0; i < Max_Wait_Time; i++)
        {
            if (Serial2.available())
            {

                buffer[0] = Serial2.read();
                if (buffer[0] == ReceiveShoot)
                {
                        for (int j = 1; j < 9; j++)
                            buffer[j] = Serial2.read();
                        // Uart_Show_Data(buffer,9);
                        return Uart_Receive_Success;

                }
            }
            delay(1);
        }
        return Uart_Receive_False;
    } 
uint8_t Buffer[10];
void Uart_Scan_QR()
{
   Serial2.write(HeaderShoot1);
   Buffer[0]=Serial2.read();
   while(Buffer[0]!=ReceiveShoot)
   {
      delay(5);
      Serial2.write(HeaderShoot1);
      Buffer[0]=Serial2.read();
   }
   Buffer[1]=Serial2.read();   
}





#endif