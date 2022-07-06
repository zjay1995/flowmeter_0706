#pragma once

#include <array>
#include "U8g2lib.h"
#define SSD1306_DISPLAYOFF          0xAE

#include "ConfigurationManager.h"

class SleepTimer
{
	//int m_intervalArray[] = {1,2,5,-1};
	std::array <int,5> m_intervalArray{{5,60,120,300,-1}};
	int m_selectedIndex = 0;
	
	unsigned long m_startMillis = 0;
	
	ConfigurationManager* m_configurationManager;
    //U8G2_SSD1327_MIDAS_128X128_F_4W_HW_SPI* m_u8g2;
    U8G2_SSD1327_MIDAS_128X128_F_4W_SW_SPI* m_u8g2;


public:

	SleepTimer()
	{
		m_selectedIndex = 4;
	}
	
	~SleepTimer()=default;
	
	void init(ConfigurationManager* configurationManager, U8G2_SSD1327_MIDAS_128X128_F_4W_SW_SPI* u8g2)
	{
		m_configurationManager = configurationManager;
		m_u8g2 = u8g2;
	}

    void selectIntervalByValueNoEEPROMSave(int interval)
    {
        for(int i=0; i < m_intervalArray.size(); i++)
            if(m_intervalArray[i] == interval)
            {
                m_selectedIndex = i;
                break;
            }

        return;
    }

	void selectIntervalByIndex(int index)
	{	
		if(index >= 0 && index < m_intervalArray.size())
		{
			m_selectedIndex = index;
			m_configurationManager->saveTimerIntervalToEEPROM(m_intervalArray[m_selectedIndex]);
		}
		
		return;
	}

	void selectIntervalByValue(int interval)
	{
		for(int i=0; i < m_intervalArray.size(); i++)
			if(m_intervalArray[i] == interval)
			{
				m_selectedIndex = i;
				m_configurationManager->saveTimerIntervalToEEPROM(5);
				break;
			}
		
		return;
	}
	
	void selectNextInterval()
	{
		m_selectedIndex = (m_selectedIndex + 1) % m_intervalArray.size();
	}
	
	void selectPreviousInterval()
	{
		if(m_selectedIndex == 0)
			m_selectedIndex = m_intervalArray.size() - 1;
		else
			m_selectedIndex = m_selectedIndex - 1;		
	}
	
	void resetIdleCounter()
	{
		m_startMillis = millis();	
	}
	
	int getSelectedInterval() { m_intervalArray[m_selectedIndex]; } 
	
	void handleTick()
	{
		if(m_intervalArray[m_selectedIndex] == -1)
			return;
		
		unsigned long passedMillis = millis() - m_startMillis;
		
		//Serial.println("SleepTimer tick: " + String(passedMillis) );	
		//Serial.flush();
		
		if(passedMillis / 1000 >= m_intervalArray[m_selectedIndex])
		{
	    digitalWrite(5, LOW);
            delay(1000);
	    digitalWrite(5, HIGH);
	    delay(100);
	    digitalWrite(5, LOW);
            //add auto-off here
            digitalWrite(25, LOW);
            //go in to deep sleep
            gpio_hold_en(GPIO_NUM_5);
            gpio_deep_sleep_hold_en();
            Serial.println("SleepTimer deep_sleep!");
			Serial.flush();

			//m_u8g2->sleepOff();
			m_u8g2 -> clearBuffer();
            m_u8g2 -> sendBuffer();

            esp_deep_sleep_start();
		}			
	}
};
